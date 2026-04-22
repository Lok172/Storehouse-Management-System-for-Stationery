#ifndef STAFFMANAGEMENT_H
#define STAFFMANAGEMENT_H

#include <iostream>
#include <string>

#include "Menu.h"
#include "DbConnection.h"
#include "ErrorDetection.h"

using namespace std;

class Staff
{
public:
	ErrorDetection error;
	database db;
	Menu Menu;
	
	string username, reset, handphoneNumber, age, marriageStatus, staffID, adminID;
	

	Staff();
	void viewStaff();
	bool updateStaff();
	bool deleteStaff();
	~Staff();
};


#endif