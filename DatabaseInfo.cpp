#include "DatabaseInfo.h"

using namespace std;
using namespace pqxx;

DatabaseInfo::DatabaseInfo()
{
	databaseAddress = "";
	databasePort = "";
	databaseName = "";
	databaseConnect = "host= " + databaseAddress + " port= " + databasePort + " dbname=" + databaseName + " user=postgres password=library2";
	
	connected = false;
}

DatabaseInfo::DatabaseInfo(string databaseAddress, string databasePort, string databaseName, string userName, string password)
{
	this->databaseAddress = databaseAddress;
	this->databasePort = databasePort;
	this->databaseName = databaseName;
	
	databaseConnect = "host= " + databaseAddress + " port= " + databasePort + " dbname=" + databaseName + " user=" + userName + " password=" + password;	
	
	connected = false;
}

bool DatabaseInfo::connectDatabase()
{
	connection c(databaseConnect);
	if(c.is_open())
	{
		connected = true;
		return true;
	}
	else
	{
		connected = false;
		return false;
	}
}

int DatabaseInfo::getNewID()
{
	string sql = "SELECT MAX(id) FROM locationID;";
	try{
		connection conn(databaseConnect);
		if(!conn.is_open()){
			cout << "Failed connection" << endl;
			return -1;
		}
		
		nontransaction N(conn);
		
		result R(N.exec(sql));
			
		for(result::const_iterator i = R.begin(); i != R.end(); ++i) {
			return i[0].as<int>();
		}
	}
	catch (const exception &e) {
		cout << "Error: " << e.what() << endl;
	}
}

bool DatabaseInfo::addDevice(string idName)
{
	int newID = getNewID();
	if(newID == -1)
		return false;
	else
	{
		newID++;
		
		string sql = "INSERT INTO locationID(id, name) VALUES (";
		sql += to_string(newID) + ", '" + idName + "');";
		
		try{
			connection conn(databaseConnect);
			if(!conn.is_open()){
				cout << "Failed open" << endl;
				return false;
			}
			
			work N(conn);
			N.exec(sql);
			N.commit();
			return true;
		}
		catch (const exception &e) {
			cout << "Error: " << e.what() << endl;
			return false;
		}
	}
}

bool DatabaseInfo::addNewInfo(int id, int moveIn, int moveOut)
{
	string squery = "INSERT INTO locationTimeInfo(id, moveIn, moveOut, time) VALUES (";
	squery += to_string(id) + ", " + to_string(moveIn) + ", " + to_string(moveOut) + ", clock_timestamp());";
	
	try{
		connection conn(databaseConnect);
		if(!conn.is_open()){
			cout << "Failed open" << endl;
			return false;
		}
	
		work N(conn);
		N.exec(squery);
		N.commit();
		//conn.disconnect();
	}
	catch (const exception &e) {
		cout << "Failed to addNewInfo: " << e.what() << endl;
		//backupInfo(id, moveIn, moveOut);
	}
	
	updateCurInfo(id, moveIn, moveOut);
}
bool DatabaseInfo::updateCurInfo(int id, int moveIn, int moveOut)
{
	int *count = new int[2];
	if(getCurrentCount(id, count))
	{
		*(count) += moveIn;
		*(count + 1) += moveOut;
		string sql = "UPDATE locationCurInfo SET moveIn = " + to_string(*(count)) + ", moveOut = " + to_string(*(count + 1)) + "WHERE id = " + to_string(id) + ";";
		try
		{
			connection conn(databaseConnect);
			if(!conn.is_open()){
				cout << "Failed open" << endl;
				return false;
			}
			
			work W(conn);
			W.exec(sql);
			W.commit();
		}
		catch (const exception &e) 
		{
			cout << "Error: " << e.what() << endl;
			delete []count;
			return false;
		}
	}
	else
	{
		cout << "Error: Failed to getCurrentCount" << endl;
		delete []count;
		return false;
	}
	delete []count;
}

bool DatabaseInfo::getCurrentCount(int id, int *count)
{
	/*
	string subsql = "SELECT MAX(time) FROM locationTimeInfo WHERE id = " + id + ";";
	string sql = "SELECT moveIn, moveOut FROM locationTimeInfo WHERE id = " + id + " AND time = " + subsql + ";";
	*/
	string sql = "SELECT moveIn, moveOut FROM locationCurInfo WHERE id = " + to_string(id) + ";";
	try
	{
		connection conn(databaseConnect);
		if(!conn.is_open()){
			cout << "Failed open" << endl;
			return false;
		}
		
		nontransaction N(conn);
		
		result R(N.exec(sql));
			
		for(result::const_iterator i = R.begin(); i != R.end(); ++i) {
			//cout << "MoveIn: " << i[0].as<int>() << endl;
			//cout << "MoveOut: " << i[1].as<int>() << endl;
			
			count[0] = i[0].as<int>();
			count[1] = i[1].as<int>();
		}
		return true;
	}
	catch (const exception &e) {
		cout << "Error: " << e.what() << endl;
		return false;
	}
}