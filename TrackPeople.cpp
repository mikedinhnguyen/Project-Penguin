#include "Tracker.h"
#include "ConfigFile.h"
#include "DatabaseInfo.h"
#include <fstream>
#include <cstdio>

// Function to convert a string, the name of an image file, into a timestamp
time_t asTime(std::string str) {
	// Remove suffix
	for (int i = 0; i < int(str.length()); i++) {
		if (str[i] == '-') {
			str.erase(i, str.length());
			break;
		}
	}

	return time_t(std::stoi(str));
}

int main(int argc, char *argv[]) {
	// Verify command line arguments are correct.
	if (argc < 2) {
		std::cout << "Missing command line arguments.\n";
		std::cout << "Use argument 'record' to start recording images.\n";
		std::cout << "Use argument 'track' to start tracking from recorded images.\n";
		std::cout << "Use argument 'clear' to clear all images in the directory.\n";
		return 0;
	}

	// Get the command line arguments.
	std::string mode = std::string(argv[1]);

	// Path to image directory.
	std::string imageList = "list.txt";
	std::string extension = ".jpg";

	if (mode == "record" || mode == "r") {
		std::ofstream outFile(imageList);

		// Video file location. Use a video file for now. Later on switch to camera feed.
		std::string videoPath = "../library3.mp4";

		// Create a video reader interface
		VideoCapture vid(videoPath);
		Mat img;
		int height = 600;
		int width = 800;

		// Grab the first frame
		vid >> img;

		// Set up time variables and recording length
		time_t startTime = time(NULL), currentTime = time(NULL), prevTime = 0;
		int recordingLength = 60;		// recording length in seconds
		int suffixCount = 0;

		// Check if recording length is provided as an argument
		if (argc > 2) {
			recordingLength = std::stoi(std::string(argv[2]));
		}

		// Start recording
		while (currentTime - startTime < recordingLength) {
			// Use the timestamp as the name of the file and add a suffix
			if (currentTime != prevTime) {
				suffixCount = 0;
			}
			std::string timeStr = std::to_string(currentTime) + "-" + std::to_string(suffixCount++) + extension;

			// Scale the image
			resize(img, img, Size(width, height));

			// Store the frame as an image file
			imwrite(timeStr, img);

			// Store the name in the list file
			outFile << timeStr << "\n";

			// Grab the next frame
			vid >> img;
			if (img.empty()) {
				// Video has ended
				break;
			}

			// Update current time
			prevTime = currentTime;
			currentTime = time(NULL);
		}

		outFile.close();

		std::cout << "Finished recording.\n";
	}
	else if (mode == "track" || mode == "t") {
		// Open text file containing list of images to be processed.
		std::ifstream inFile(imageList);
		std::string imageFile;

		// Set up the tracker
		int leftCount = 0, rightCount = 0;
		int height = 600;
		int width = 800;
		int maxMissingFrames = 20;
		double maxDistance = 100.0;		// Increase this number to increase sensitivity
		Tracker tracker(height, width, maxMissingFrames, maxDistance);

		// Set up the database connection
		const int NUM_CONFIG = 7;
		const std::string configFileName = "../src/configFile.txt";

		//array with all the configfile names
		std::string configNames[] = {"databaseAddress",
									 "databasePort",
									 "databaseMain",
									 "databaseUser",
									 "databasePassword",
									 "deviceID",
									 "deviceName"};

		//array to store all configfile values. index matches with configNames[]
		std::string values[NUM_CONFIG];
		int deviceNumber = -1;

		ConfigFile *file = new ConfigFile(configFileName);
		DatabaseInfo *database = nullptr;

		//Try to read the configfile with names and get the values
		if(file->readConfig(configNames, values, NUM_CONFIG))
		{
			//Make sure device isn't already added by checking if a deviceID exists.
			if(values[5].empty())
			{
				std::cout << "Error: configFile has no deviceID." << std::endl;
				std::cout << "Run AddDevice first." << std::endl;
			}
			else
			{
				//Create and connect to database
				database = new DatabaseInfo(values[0], values[1], values[2], values[3], values[4]);
				deviceNumber = std::stoi(values[5]);
			}
		}

		// Set up update interval
		int updateInterval = 60;  // update interval in seconds
		time_t startTime = 0;
		time_t timeOfLastUpdate = startTime;

		// Set up backup file
		std::string backupFileName = "../src/backupFile.txt";
		ConfigFile *backupFile = new ConfigFile(backupFileName);
		std::vector<std::string> backupData;

		// Start tracking
		while (inFile >> imageFile) {
			if (imageFile == "") {
				std::cout << "Found an empty line in the list file.\n";
				break;
			}

			// Convert name of file to timestamp
			if (startTime == 0) {
				startTime = asTime(imageFile);
				timeOfLastUpdate = startTime;
			}

			// Open the image file
			Mat img = imread(imageFile);

			// Detect people
			std::vector<Rect> rects;
			std::vector<double> weights;
			tracker.detect(img, rects, weights);

			// Draw line right at the middle of the screen
			line(img, Point(width/2, 0), Point(width/2, height), Scalar(0,0,255), 2);

			// Draw rectangles
			for( size_t i = 0; i < rects.size(); i++ )
			{
				rectangle(img, rects[i], cv::Scalar(0,0,255), 3);

				// Display counts
				std::stringstream text;
				text << "Left: " << leftCount;
				putText(img, text.str(),Point(width/2-150, height-50), FONT_HERSHEY_SIMPLEX, 1, Scalar(0,0,0), 2);

				text.str("");
				text << "Right: " << rightCount;
				putText(img, text.str(), Point(width/2+25, height-50), FONT_HERSHEY_SIMPLEX, 1, Scalar(0,0,0), 2);
			}

			// Render frame
			imshow("detected person", img);
			waitKey(1);

			// Update tracker and get number of people who have crossed the middle line
			int count = tracker.track(rects);

			if (count != 0) {	// Someone just passed by
				if (count > 0) {
					rightCount += count;
				}
				else {
					leftCount += -count;
				}
			}

			// Check if it's time to update the database
			time_t currentTime = asTime(imageFile);
			if (currentTime - timeOfLastUpdate >= updateInterval) {
				// Get the current timestamp as a string
				std::string date(ctime(&currentTime));

				// Insert new entry to the database
				if (database->addNewInfo(deviceNumber, leftCount, rightCount, date)) {
					// Insertion successful, reset counters
					leftCount = 0;
					rightCount = 0;
				}
				else {
					// Insertion failed, try inserting to the backup instead
					std::cout << "Failed to insert to database, trying to insert to the backup.\n";
					backupFile->backupWrite(deviceNumber, leftCount, rightCount, date);
				}
				timeOfLastUpdate = currentTime;

				// Make a backup
				backupData.clear();
				backupFile->backupRead(&backupData);
				for(std::vector<std::string>::iterator it = backupData.begin(); it != backupData.end(); ++it)
				{
					if (*it == "") {	// ignore empty string
						continue;
					}
					std::cout << *it << std::endl;
					int firstPos = (*it).find(' ');
					int secondPos = (*it).find(' ', firstPos + 1);
					int thirdPos = (*it).find(' ', secondPos + 1);
					std::cout << (*it).substr(0, firstPos) << " ";
					int tempDeviceID = stoi((*it).substr(0, firstPos));
					std::cout << (*it).substr(firstPos + 1, secondPos - firstPos) << std::endl;
					int tempMoveIn = stoi((*it).substr(firstPos + 1, secondPos - firstPos));
					int tempMoveOut = stoi((*it).substr(secondPos + 1, thirdPos - secondPos));
					std::string timeInfo = (*it).substr(thirdPos + 1);

					if(database->addNewInfo(tempDeviceID, tempMoveIn, tempMoveOut, date))
					{
						std::cout << "Success: " << tempDeviceID << " " << tempMoveIn << " " << tempMoveOut << std::endl;
					}
					else {
						backupFile->backupWrite(deviceNumber, leftCount, rightCount, date);
					}
					std::cout << "--------------" << std::endl;
				}
			}
		}

		inFile.close();

		std::cout << "Finished tracking.\n";
	}
	else if (mode == "clear" || mode == "c") {
		std::ifstream inFile(imageList);
		std::string imageFile;

		while (inFile >> imageFile) {
			const char *fileName = imageFile.c_str();
			remove(fileName);
		}

		inFile.close();
	}
	else {
		std::cout << "Invalid arguments.\n";
	}

	return 0;
}
