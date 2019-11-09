#include "ConfigFile.h"

using namespace std;

ConfigFile::ConfigFile()
{
	fileName = "";
	fileText = "";
	readFile = false;
}

ConfigFile::ConfigFile(string fileName)
{
	this->fileName = fileName;
	fileText = "";
	readFile = false;
	getConfigText();
}

void ConfigFile::changeFileName(string fileName)
{
	this->fileName = fileName;
	fileText = "";
	readFile = false;
}

void ConfigFile::fileTextReset()
{
	fileText = "";
	readFile = false;
}

bool ConfigFile::fileRead()
{
	return readFile;
}

bool ConfigFile::getConfigText()
{
	file.open(fileName);
	
	//If we're unable to open config file, print error
	//Return false could not open
	if(!file) 
	{
		std::cout << "Unable to locate and open file" << std::endl;
		return false;
	}
	else
	{
		fileTextReset();
		//Go through entire file and get the text to store into string
		string temp;
		while(getline(file, temp))
		{
			//cout << temp << endl;
			fileText += temp + '\n';
		}
	}
	file.close();
	readFile = true;
	return true;
}

bool ConfigFile::readConfig(string *configNames, string *values, int len)
{
	//Make sure that we have read the file first
	if(!readFile)
	{
		std::cout << "Error: File has not been read yet. Please read file first" << endl;
		return false;
	}
	
	string *valueBackup = values; //Backup incase we run into an error. Allows for return of original values content
	string *attrValues = new string[len]; //Store all the values we get
	
	int startPos = 0;
	int curPos = 0;
	int endPos = 0;
	
	string fileAttr; //Config attribute to look for
	string attrValue; //Actual value_comp
	
	//Loop through all the config attributes we're looking for
	for(int i = 0; i < len; i++)
	{
		fileAttr = *(configNames + i); //Attribute name
		curPos = fileText.find(fileAttr); //Find it in our stored filetext
		if(curPos == string::npos) //If we couldn't find the attribute, print error and exit.
		{
			std::cout << "Error: cannot find config file attribute" << std::endl;
			delete []attrValues;
			return false;
		}
		startPos = fileText.find('=', curPos); //Find the end of the attribute
		endPos = fileText.find('\n', curPos); //Find the end of the attribute value
		attrValue = fileText.substr(startPos + 1, endPos - startPos - 1); //Get the attribute value
		//attrValue.erase(remove_if(attrValue.begin(), attrValue.end(), isspace), attrvalue.end()); //Remove whitespace
		*(values + i) = attrValue; //Store value for return
	}
	//values = attrValues;
	//delete []valueBackup; //need to fix deleting of old values pointer(maybe just don't bother with old)
	return true;
}