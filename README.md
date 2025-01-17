# Database:

## Postgresql:

Uses Postgressql database. Can be installed on linux (sudo apt-get install postgresql)
### Setup:

- Connect to database in terminal(default: sudo -u postgres psql)
- Create database (May need to type each line):

```
CREATE DATABASE Library;

CREATE TABLE username(name TEXT NOT NULL, password TEXT NOT NULL);

CREATE TABLE locationID(ID INT NOT NULL, NAME TEXT NOT NULL);

CREATE TABLE locationCurInfo(ID INT NOT NULL, moveIn INT, moveOut INT);

CREATE TABLE locationTimeInfo(ID INT NOT NULL, moveIn INT, moveOut INT, TIMESTAMP TEXT);
```

## Libpqxx:

Used for connecting to the database
### Installing libpqxx:
- install libpq (apt-get install libpq-dev)
- download [libpqxx (6.4.5)](https://github.com/jtv/libpqxx/releases/)
- ./configure --disable-documentation
- make
- sudo make install

# Adding a new device:

- Download all files

- Edit configFile.txt (except for deviceId)

- Compile the AddDevice Code

`(g++ -std=c++11 AddDevice.cpp DatabaseInfo.cpp ConfigFile.cpp -o AddDevice -lpqxx -lpq)`

- Run AddDevice(./AddDevice)
 
configFile deviceID should be updated with an ID. Manually change deviceID if writing failed.

DO NOT CHANGE ID. This is how the database identifies your device. You can delete deviceID and rerun this to add device again.

# Running the program:

Do some stuff
