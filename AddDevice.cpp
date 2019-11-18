#include "ConfigFile.h"
#include "DatabaseInfo.h"

using namespace std;

int main()
{
	const int NUM_CONFIG = 7;
	const string fileName = "configFile.txt";
	
	//array with all the configfile names
	string configNames[] = {"databaseAddress",
					       "databasePort",
					       "databaseMain",
					       "databaseUser",
					       "databasePassword",
					       "deviceID",
						   "deviceName"};
	
	//array to store all configfile values. index matches with configNames[]
	string values[NUM_CONFIG];
	int deviceNumber = -1;
	
	ConfigFile *file = new ConfigFile(fileName);
	
	//Try to read the configfile with names and get the values
	if(file->readConfig(configNames, values, NUM_CONFIG))
	{
		/*
		for(int i = 0 ; i < NUM_CONFIG; i++)
			cout << values[i] << endl;
		*/
		
		//Make sure device isn't already added by checking if a deviceID exists.
		if(!values[5].empty())
		{
			cout << "Error: configFile currently has a deviceID and is added." << endl;
			cout << "Delete deviceID value to readd device." << endl;
		}
		else
		{
			//Create and connect to database
			DatabaseInfo *database = new DatabaseInfo(values[0], values[1], values[2], values[3], values[4]);
			
			//Try to add the device
			deviceNumber = database->addDevice(values[6]);
			if(deviceNumber != -1)
			{
				if(file->changeValue("deviceID", to_string(deviceNumber)))
				{
					cout << "Successfully added device to database." << endl;
				}
				else
				{
					cout << "Error changing deviceID in configFile." << endl;
					cout << "Manual edit of deviceID to " + to_string(deviceNumber) + " may be needed." << endl;
				}
			}
			else
			{
				cout << "Failed adding device to database." << endl;
			}
		}
	}
	
	return 0;
}