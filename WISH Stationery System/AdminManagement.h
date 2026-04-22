#ifndef ADMINMANAGEMENT_H
#define ADMINMANAGEMENT_H

#include <iostream>
#include <string>


#include "Menu.h"
#include "DbConnection.h"
#include "ErrorDetection.h"

using namespace std;

class Admin
{
public:
	ErrorDetection error;
	database db;
	Menu Menu;

	string userID, username, reset, handphoneNumber, age, marriageStatus, adminID, ReferralCode;


	Admin();
	void viewAdmin();
	bool updateAdmin();
	bool deleteAdmin();
	~Admin();
};


#endif