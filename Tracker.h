#ifndef TRACKER_H
#define TRACKER_H

#include "Centroid.h"
#include <vector>

class Tracker {
private:
	int frameHeight, frameWidth, maxMissingFrames;
	double maxDistance;
	vector<Centroid> currentCentroids;
	HOGDescriptor hog;

public:
	/**
	 * Constructor that takes in the size of image to be detected, the missing frames threshold,
	 * and the reasonable distance threshold
	 *
	 * @param frameHeight the height of the image
	 * @param frameWidth the width of the image
	 * @param maxMissingFrames the missing frames threshold. The tracker will keep tracking an object
	 * for some time when it goes out of frame. This is because the detection algorithm isn't perfect,
	 * an object can be detected in one frame but not in the next one.
	 * @param maxDistance the reasonable distance threshold. When old objects are  being paired up with
	 * new objects, only pair them up if they are not too far apart from one another.
	 */
	Tracker(int _frameHeight, int _frameWidth, int _maxMissingFrames, double _maxDistance);

	/**
	 * Detect people in an image
	 *
	 * @param img Mat object representing a single frame of the video
	 * @param rects a vector that will store all detected objects in the form of rectangles
	 * @param weights a vector that will store the weights of the detected objects
	 */
	void detect(Mat img, vector<Rect> &rects, vector<double> &weights);

	/**
	 * Track objects and return the number of people walking left and right
	 *
	 * @param rects a vector of rectangles that represents the objects
	 * @return the number of people walking to the right of the screen (if positive), or the number of
	 * people walking to the left of the screen (if negative)
	 */
	int track(vector<Rect> rects);

	/**
	 * Convert a vector of rectangles to a vector of centroids
	 *
	 * @param rects a vector of rectangles
	 * @return a vector of centroids
	 */
	vector<Centroid> toCentroids(vector<Rect> rects);
};

#endif
