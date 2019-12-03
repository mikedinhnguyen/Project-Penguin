#ifndef DATABASEINFO_H
#define DATABASEINFO_H

#include <iostream>
#include <string>
#include <pqxx/pqxx>

class DatabaseInfo
{
	private:
		//connection *conn;
		std::string databaseAddress;
		std::string databasePort;
		std::string databaseName;
		std::string databaseConnect;
		
		bool connected;
		
		int getNewID();
		bool getCurrentCount(int id, int *count);
	public:
		DatabaseInfo();
		DatabaseInfo(std::string databaseAddress, std::string databasePort, std::string databaseName, std::string userName, std::string password);
		
		void setDatabaseAddress(std::string databaseAddress);
		void setDatabasePort(std::string databasePort);
		void setDatabaseName(std::string databaseName);
		
		bool connectDatabase();
		bool addDevice(std::string idName);
		bool updateCurInfo();
		bool addTimeInfo();
		
		bool addNewInfo(int id, int moveIn, int moveOut, std::string date);
		bool updateCurInfo(int id, int moveIn, int moveOut);
};

#endif
