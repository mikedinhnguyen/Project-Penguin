#ifndef DATABASECOMMUNICATOR_H
#define DATABASECOMMUNICATOR_H

#include <iostream>
#include <string>
#include <ctime>

using namespace std;

class DatabaseCommunicator {
private:

public:
	/**
	 * Establish connection with the database in the constructor
	 */
	DatabaseCommunicator();

	/**
	 * Insert new entry into the database
	 *
	 * @param timeStamp a string representing the current timestamp (is there a better data type to represent this?)
	 * @param deviceName the name of the device
	 * @param count the number of people detected, a positive number indicates net number of people walking to the
	 * right of the image, a negative number indicates net number of people walking to the left of the image
	 */
	void insert(string timeStamp, string deviceName, int count);
};

#endif
