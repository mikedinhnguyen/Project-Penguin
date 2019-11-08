#include "Tracker.h"
#include "DatabaseCommunicator.h"

int main() {
	// Video file location. Later on switch to camera feed.
	string videoPath = "../town_center.avi";

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

	// Set up the database communicator
	DatabaseCommunicator database;
	string deviceName = "device0"; // have 1 device for now
	int updateInterval = 60;  // update interval in seconds
	time_t startTime = time(NULL);
	time_t timeSinceLastUpdate = startTime;

	// Start tracking loop
	while (true) {
		// Scale the image
		resize(img, img, Size(width, height));

		// Detect people
		vector<Rect> rects;
		vector<double> weights;
		tracker.detect(img, rects, weights);

		// Draw line right at the middle of the screen
		line(img, Point(width/2, 0), Point(width/2, height), Scalar(0,0,255), 2);

		// Draw rectangles
		for( size_t i = 0; i < rects.size(); i++ )
		{
			rectangle(img, rects[i], cv::Scalar(0,0,255), 3);

			// Display counts
			stringstream text;
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
			cout << "test\n";
			string date(ctime(&currentTime));
			database.insert(date, deviceName, count);
			timeSinceLastUpdate = currentTime;
		}

		// Grab the next frame
		vid >> img;

		// Check if the video has ended
		if (img.empty()) {
			cout << "Video has ended.\n";
			break;
		}
	}

	return 0;
}
