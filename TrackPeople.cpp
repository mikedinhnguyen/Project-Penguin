#include "Tracker.h"
#include "ConfigFile.h"
#include "DatabaseInfo.h"

int main() {
	// Video file location. Later on switch to camera feed.
	std::string videoPath = "../town_center.avi";

	// Create a video reader interface
	VideoCapture vid(videoPath);
	Mat img;

	// Grab the first frame
	vid >> img;

	// Set up the tracker
	int leftCount = 0, rightCount = 0;
	double scaleFactor = 0.5;
	int height = img.rows * scaleFactor;
	int width = img.cols * scaleFactor;
	int maxMissingFrames = 10;
	double maxDistance = 50.0;		// Increase this number to increase sensitivity
	Tracker tracker(height, width, maxMissingFrames, maxDistance);

	// Set up the database connection
	const int NUM_CONFIG = 7;
	const std::string fileName = "../src/configFile.txt";

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

	ConfigFile *file = new ConfigFile(fileName);
	DatabaseInfo *database = nullptr;

	//Try to read the configfile with names and get the values
	if(file->readConfig(configNames, values, NUM_CONFIG))
	{
//		for(int i = 0 ; i < NUM_CONFIG; i++)
//			std::cout << values[i] << std::endl;

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
	int updateInterval = 5;  // update interval in seconds
	time_t startTime = time(NULL);
	time_t timeSinceLastUpdate = startTime;

	// Set up backup
	std::string backupFileName = "../src/backupFile.txt";
	ConfigFile *backupFile = new ConfigFile(backupFileName);
	std::vector<std::string> backupData;

	// Start tracking loop
	while (true) {
		// Scale the image
		resize(img, img, Size(width, height));

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
		time_t currentTime = time(NULL);
		if (currentTime - timeSinceLastUpdate >= updateInterval) {
			std::string date(ctime(&currentTime));
			// how to insert entry into the database?
			if (database->addNewInfo(deviceNumber, leftCount, rightCount, date)) {
				std::cout << "a\n";
				leftCount = 0;
				rightCount = 0;
			}
			else {
				std::cout << "Failed to insert to database, trying to insert to the backup.\n";
				backupFile->backupWrite(deviceNumber, leftCount, rightCount, date);
			}
			timeSinceLastUpdate = currentTime;

			// Make a backup
			backupData.clear();
			backupFile->backupRead(&backupData);
			std::cout << backupData.size() << "\n";
			for(std::vector<std::string>::iterator it = backupData.begin(); it != backupData.end(); ++it)
			{
				std::cout << "test\n";
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

				//cout << tempDeviceID << " " << tempMoveIn << " " << tempMoveOut << endl;
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

		// Grab the next frame
		vid >> img;

		// Check if the video has ended
		if (img.empty()) {
			std::cout << "Video has ended.\n";
			break;
		}
	}

	return 0;
}
