#ifndef CENTROID_H
#define CENTROID_H

#include <iostream>
#include <cmath>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace cv::ml;

struct Centroid {
	Point location;
	int missingFramesCount;

	/**
	 * Constructor that takes in the coordinate of the centroid
	 *
	 * @param x the x-coordinate (horizontal position) of the centroid
	 * @param y the y-coordinate (vertical position) of the centroid
	 */
	Centroid(int x, int y);

	/**
	 * Get Euclidean distance to another centroid
	 *
	 * @param other the other centroid
	 * @return the Euclidean distance to the other centroid
	 */
	double getDistance(Centroid other);
};

#endif
