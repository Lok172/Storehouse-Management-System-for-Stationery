#ifndef PROFILE_H
#define PROFILE_H

#include <iostream>
#include <string>


#include "Menu.h"
#include "DbConnection.h"
#include "ErrorDetection.h"

using namespace std;

class Profile
{
public:
	ErrorDetection error;
	database db;
	Menu Menu;

	string usertype, userID, username, password, handphoneNumber, age, marriageStatus, ReferralCode;


	Profile();
	void viewProfile();
	void viewPassword();
	bool addInformation();
	bool updateInformation();
	bool deleteInformation();


	~Profile();
};


#endif