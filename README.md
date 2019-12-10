#Database:

	##Postgresql:
	
		Uses Postgressql database. Can be installed on linux (sudo apt-get install postgresql)
		#Setup:
			-Connect to database in terminal(default: sudo -u postgres psql)
			-Create database (May need to type each line):
				CREATE DATABASE Library;
        
				CREATE TABLE username(name TEXT NOT NULL, password TEXT NOT NULL);
				CREATE TABLE locationID(ID INT NOT NULL, NAME TEXT NOT NULL);
				CREATE TABLE locationCurInfo(ID INT NOT NULL, moveIn INT, moveOut INT);
				CREATE TABLE locationTimeInfo(ID INT NOT NULL, moveIn INT, moveOut INT, TIMESTAMP TEXT); 
    
	##Libpqxx:
		Used for connecting to the database
		Installing libpqxx:
			install libpq (apt-get install libpq-dev)
			download libpqxx (7.0.0)
			./configure --disable-documentation
			make
			sudo make install
