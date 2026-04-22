#ifndef REGISTER_H
#define REGISTER_H

#include <iostream>
#include <string>
#include <vector>
#include "DbConnection.h"
#include "Menu.h"
using namespace std;


class Register
{
public:
	database db;
	Menu Menu;
	string  usertype, username, password, password1, password2, ReferralCode, staffID, adminID;
	
	Register();
	bool prompt();
	void registerAdmin();
	void registerStaff();

	~Register();
};

#endif