#ifndef CONFIG_H
#define CONFIG_H

#include <iostream>
#include <fstream>
#include <string>

class ConfigFile
{
	private:
		std::ifstream file;
		std::string fileName;
		std::string fileText;
		bool readFile;
	public:
		//Constructors
		ConfigFile();
		ConfigFile(std::string fileName);
		
		void changeFileName(std::string fileName);
		void fileTextReset();
		
		//Gets all the text in the config file
		bool getConfigText();
		
		bool fileRead();
		
		//Reads config file attributes based off configNames. Stores all config values in values
		//Returns true with correctly able to get ALL the attributes
		//Returns false if one or more of the attributes were not able to be read.
		bool readConfig(std::string *configNames, std::string *values, int len); 
};

#endif