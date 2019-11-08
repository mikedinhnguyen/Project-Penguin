#include "Tracker.h"

// Constructor that takes in the size of image to be detected, the missing frames threshold,
// and the reasonable distance threshold
Tracker::Tracker(int _frameHeight, int _frameWidth, int _maxMissingFrames, double _maxDistance) {
	frameHeight = _frameHeight;
	frameWidth = _frameWidth;
	maxMissingFrames = _maxMissingFrames;
	maxDistance = _maxDistance;
	hog.setSVMDetector(HOGDescriptor::getDefaultPeopleDetector());
}

// Detect people in an image
void Tracker::detect(Mat img, vector<Rect> &rects, vector<double> &weights) {
	hog.detectMultiScale(img, rects, weights);
}

// Track objects and return the number of people walking left and right
int Tracker::track(vector<Rect> rects) {
	int crossCount = 0;
	vector<Centroid> newCentroids = toCentroids(rects);
	vector<Centroid> updatedCentroids;

	// Match current centroids with new centroids
	while (true) {
		int mini = -1, minj = -1;
		double minDistance = maxDistance;

		// Get the closest pair of centroids
		for (int i = 0; i < (int) currentCentroids.size(); i++) {
			for (int j = 0; j < (int) newCentroids.size(); j++) {
				double distance = currentCentroids[i].getDistance(newCentroids[j]);
				if (distance < minDistance) {
					minDistance = distance;
					mini = i;
					minj = j;
				}
			}
		}

		// Match this pair
		if (mini != -1) {
			// If the centroid passes through the middle line, increment count
			int oldx = currentCentroids[mini].location.x, newx = newCentroids[minj].location.x;
			if (oldx <= frameWidth/2 && newx > frameWidth/2) {
				crossCount++;
			}
			else if (oldx >= frameWidth/2 && newx < frameWidth/2) {
				crossCount--;
			}
			updatedCentroids.push_back(newCentroids[minj]);
			currentCentroids.erase(currentCentroids.begin()+mini);
			newCentroids.erase(newCentroids.begin()+minj);
		}
		else {
			break;
		}
	}

	// Track unpaired centroids
	for (int i = 0; i < (int) currentCentroids.size(); i++) {
		currentCentroids[i].missingFramesCount++;
		if (currentCentroids[i].missingFramesCount <= maxMissingFrames) {
			updatedCentroids.push_back(currentCentroids[i]);
		}
	}
	for (int i = 0; i < (int) newCentroids.size(); i++) {
		updatedCentroids.push_back(newCentroids[i]);
	}

	currentCentroids = updatedCentroids;

	return crossCount;
}

// Convert a vector of rectangles to a vector of centroids
vector<Centroid> Tracker::toCentroids(vector<Rect> rects) {
	vector<Centroid> centroids;
	for (int i = 0; i < (int) rects.size(); i++) {
		centroids.push_back(Centroid(rects[i].x+rects[i].width/2, rects[i].y+rects[i].height/2));
	}
	return centroids;
}
