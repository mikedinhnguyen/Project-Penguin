#include <ctime>
#include <iostream>
#include <string>
#include <raspicam/raspicam_cv.h>
#include "Tracker.h"
using namespace std;

int main(int argc, char ** argv){
	
    time_t timer_begin, timer_end;
    time_t timeDate_start, timeDate_end;
    struct tm* timeinfo;
    // time(&timeDate_start);
    // timeinfo = localtime(&timeDate_start);
    
    raspicam::RaspiCam_Cv Camera;
    cv::Mat image;

    Camera.set(CV_CPU_FMA3, CV_8UC1);
    cout << "Opening camera... " << endl;
    
    if(!Camera.open()){ cerr << "Error opening Camera" << endl; return -1;}
    time(&timer_begin);//gets time since January 1, 1970 in seconds
    int numPicturesSaved = 0;
    int frameCount = 100; // 100 frames ~= 10s
    for(int i = 0; i <= frameCount; i++){
		Camera.grab();
		Camera.retrieve(image);
		cout << "\r Number of Frames: " << i << flush;
		string fileName, fileNumber;
		
		int imageLabelFix = i / (frameCount / 4);
		/* every 20 frames (2.0s), save the picture in the format 'image-XX.jpg' 
		where XX represents a two-digit integer */
		if(i % 20 == 0){
			numPicturesSaved++;
			cout << "Picture number " << numPicturesSaved << " has been saved." << endl;
			if(imageLabelFix < 10){
				fileNumber = "0" + to_string(imageLabelFix);
			}else{
				fileNumber = to_string(imageLabelFix);
			}
			fileName = "image-" + fileNumber + ".jpg";
			cv::imwrite(fileName, image);
		}
	}
	cout << "Beginning processing..." <<endl;
	Camera.release();
	
	string imagesInFile[numPicturesSaved];
	for(int i = 0; i < numPicturesSaved - 1; i++){
	   string imageFileName = "image-";
	   if(i < 10){ 
	    imageFileName += "0" + to_string(i);
	    }else{
		 imageFileName += to_string(i);
	    }
	    imageFileName += ".jpg";
	    imagesInFile[i] = imageFileName;
	    //cout << currentPicture[i] << endl;
	    cout << "Processing " << imagesInFile[i] << "..." <<endl;
	}
	 //   image = imread(currentPicture[i], IMREAD_GRAYSCALE);
		image = imread(imagesInFile[0], IMREAD_GRAYSCALE);
	    // Detect people
		std::vector<Rect> rects;
		std::vector<double> weights;
		double scaleFactor = 0.5;
		int height = image.rows * scaleFactor;
		int width = image.cols * scaleFactor;
		int maxMissingFrames = 10;
		double maxDistance = 50.0;		// Increase this number to increase sensitivity
		Tracker tracker(height, width, maxMissingFrames, maxDistance);
	
		tracker.detect(image, rects, weights);

		// Draw line right at the middle of the screen
		line(image, Point(width/2, 0), Point(width/2, height), Scalar(0,0,255), 2);
		int rightCount = 0, leftCount = 0;
		// Draw rectangles
		for( size_t i = 0; i < rects.size(); i++ )
		{
			rectangle(image, rects[i], cv::Scalar(0,0,255), 3);

			// Display counts
			std::stringstream text;
			text << "Left: " << leftCount;
			putText(image, text.str(),Point(width/2-150, height-50), FONT_HERSHEY_SIMPLEX, 1, Scalar(0,0,0), 2);

			text.str("");
			text << "Right: " << rightCount;
			putText(image, text.str(), Point(width/2+25, height-50), FONT_HERSHEY_SIMPLEX, 1, Scalar(0,0,0), 2);
		}
	    
	imshow("detected person", image);
        waitKey(1);
	
	time(&timer_end);
	double elapsedTime = difftime(timer_end, timer_begin);
	cout << "Time taken to complete: " << elapsedTime << endl;
	
}
				
    
      
