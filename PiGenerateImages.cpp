#include <ctime>
#include <iostream>
#include <raspicam/raspicam_cv.h>
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
    int frameCount = 500; // 500 frames ~= 50s
    for(int i = 0; i < frameCount; i++){
		Camera.grab();
		Camera.retrieve(image);
		cout << "\r Captured " << i << " images" << flush;
		string fileName, fileNumber;
		
		int imageLabelFix = i / (frameCount / 20);
		/* every 25 frames (2.5s), save the picture in the format 'image-XX.jpg' 
		where XX represents a two-digit integer */
		if(i % 25 == 0){
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
	cout << "Closing camera ... " <<endl;
	Camera.release();
	time(&timer_end);
	// time(&timeDate_end);
	// timeinfo = localtime(&timeDate_end);
	// cout << "End time and date: " << asctime(timeinfo) << endl;
	double elapsedTime = difftime(timer_end, timer_begin);
	cout << "Time taken to complete: " << elaspedTime << endl;
}
				
    
      
