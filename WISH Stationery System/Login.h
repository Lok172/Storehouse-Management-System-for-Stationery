#ifndef LOGIN_H
#define LOGIN_H

#include <iostream>
#include <string>

#include "DbConnection.h"
#include "ErrorDetection.h"
#include "Menu.h"

using namespace std;

class Login
{
public:
	ErrorDetection error;
	database db;
	Menu Menu;
	string  usertype, userID, password, staffID, adminID;
	bool loginstatus;

	Login();
	bool prompt();
	void loginAdmin();
	void loginStaff();
	bool logout();
	~Login();
};


#endif