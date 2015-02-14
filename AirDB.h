#ifndef AIR_DB_H
#define AIR_DB_H

#include <sqlite3.h>




class AirDB{
	sqlite3 * db;
	
public:
	AirDB();


	~AirDB();
};

#endif