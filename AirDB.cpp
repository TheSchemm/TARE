#include "AirDB.h"
#include <iostream>


using namespace std;

AirDB::AirDB(){
	cout << "Here2" << endl;
	int rc = sqlite3_open("air", &db);

   	if( rc ){
    	cout << "Can't open database: " << sqlite3_errmsg(db) << endl;

   	}else{
    	cout << "Opened database successfully" << endl;
   	}
}





AirDB::~AirDB(){
	sqlite3_close(db);
}
