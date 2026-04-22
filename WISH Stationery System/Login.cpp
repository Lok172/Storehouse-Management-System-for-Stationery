#include "Login.h"

#include <iostream>
#include <conio.h>

using namespace std;


Login::Login()
{
	usertype = ""; 
	userID = "";
	password = "";
	staffID = "";
	adminID = "";
	loginstatus = false;
}

bool Login::prompt() {
	vector <string> promptLine = { "User ID : " ,
					"Password : "
	};
	string again = "Y";
	error.data = again;

	Menu.formatVectorInput(promptLine);
	
	cout << promptLine[0];
	cin >> userID;


	cout << Menu.formatInput("Press TAB to show password");
	cout << promptLine[1];
	password = Menu.hideInput();

	char fristchar = toupper(userID[0]);
	if (fristchar == 'A')
	{
		usertype = "admin";
		loginAdmin();
	}

	else if (fristchar == 'S')
	{
		usertype = "staff";
		loginStaff();
	}

	//Incorrect format of userID
	else
	{
		loginstatus = false;
	}


	if (loginstatus == false)
	{
		cout << endl << Menu.formatInput("Incorrect user ID or password. ");
		bool temp = true;

		while (temp == true)
		{
			cout << Menu.formatInput("Do you want to try again? (Y/N) : ");
			cin >> again;
			for (char& c : again)
			{
				c = toupper(c);
			}
			error.data = again;
			temp = (error.checkOption("Y", "N") || error.checkSpace());
		}

		if (again == "Y")
		{
			return true;
		}

		else if (again == "N")
		{
			cout << endl << Menu.formatInput("The login has been canceled.");
			cout << Menu.formatInput("Press any key to exit this page...");
			_getch();
			return false;
		}
	}
}

void  Login::loginAdmin()
{
	db.column_list = "password";
	db.table_name = "admin";
	db.condition = "adminID = '" + userID + "'";
	db.select();
	db.row = mysql_fetch_row(db.res);

	if (db.row == nullptr) // no password detected means the userID incorrect
	{
		loginstatus = false;
	}

	else
	{
		if (password != db.row[0]) // The password incorrect
		{
			loginstatus = false;
		}

		else
		{
			cout << endl << Menu.formatInput("Login success. Please press any key to redirect to user menu...");
			_getch();
			loginstatus = true;
		}
	}
}

void  Login::loginStaff()
{
	db.column_list = "password";
	db.table_name = "staff";
	db.condition = "staffID = '" + userID + "'";
	db.select();
	db.row = mysql_fetch_row(db.res);

	if (db.row == nullptr) // no password detected means the userID incorrect
	{
		loginstatus = false;
	}

	else
	{
		if (password != db.row[0]) // The password incorrect
		{
			loginstatus = false;
		}

		else
		{
			cout << endl << Menu.formatInput("Login success. Please press any key to redirect to user menu...");
			_getch();
			loginstatus = true;
		}
	}
}

bool  Login::logout()
{
	bool temp = true;
	string confirmation;

	while (temp == true)
	{
		cout << Menu.formatInput("Confirm to logout? (Y/N) : ");

		cin >> confirmation;
		for (char& c : confirmation)
		{
			c = toupper(c);
		}
		error.data = confirmation;


		temp = (error.checkOption("Y", "N") || error.checkSpace());
	}

	if (confirmation == "Y")
	{
		return true;
	}

	else if (confirmation == "N")
	{
		return false;
	}


}

Login::~Login()
{

}