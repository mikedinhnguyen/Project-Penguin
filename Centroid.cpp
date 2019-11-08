#include "Centroid.h"

// Constructor that takes in the coordinate of the centroid
Centroid::Centroid(int x, int y) {
	location = Point(x, y);
	missingFramesCount = 0;
}

// Get Euclidean distance to another centroid
double Centroid::getDistance(Centroid other) {
	int xDiff = location.x - other.location.x;
	int yDiff = location.y - other.location.y;
	return sqrt((double) xDiff * xDiff + yDiff * yDiff);
}
