#include "register.h"
#include "DbConnection.h"
#include "Menu.h"
#include "ErrorDetection.h"

#include <iostream>
#include <conio.h>
#include <cstdlib>
#include <stdlib.h> 
#include <ctime>
#include <string>
#include <algorithm>



using namespace std;
ErrorDetection error;

Register::Register(){
	usertype = "1";
	username = "";
	password = "";
	ReferralCode = "";
	staffID = "";
	adminID = "";
	
}

bool Register::prompt(){
	vector <string> promptLine = { "Type of user (Admin = 1, Staff = 2): " ,
					"Name : ",
					"Password (<= 10 characters and/or digits) : ",
					"Re-enter password again :",
					"Please enter referral code: ",
	};
	
	Menu.formatVectorInput(promptLine);

	// Enter usertype
	bool inputError = true;
	while (inputError == true)
	{
		cout << promptLine[0]; 
		cin >> usertype;
		
		error.data = usertype;
		
		if (error.checkDigit() || error.checkSpace())
		{
			continue;
		}
		
		if(error.checkOption("1", "2")) 
		{
			continue;
		}
		inputError = false;

	}
	cin.ignore();

	
	// Enter username
	inputError = true;
	while (inputError == true)
	{
		cout << promptLine[1];
		getline(cin, username);

		error.data = username;

		if (error.checkEmpty())
		{
			continue;
		}
		inputError = false;
	}

	//Enter password
	inputError = true;
	while (inputError == true)
	{
		inputError = true;
		while (inputError == true)
		{
			// Enter password first time
			cout << endl << Menu.formatInput("Press TAB to show password");
			cout << promptLine[2];
			password1 = Menu.hideInput();
			cout << endl;

			error.data = password1;

			if (error.checkEmpty())
			{
				continue;
			}

			if (password1.length() > 10)
			{
				
				cout << Menu.formatInput("Password should not longer than 10 character, enter again.\n ");
				continue;
			}
			inputError = false;
		}

		// Enter password second time
		inputError = true;
		while (inputError == true)
		{
			cout << promptLine[3];
			password2 = Menu.hideInput();
			cout << endl;

			error.data = password2;

			if (error.checkEmpty())
			{
				continue;
			}

			if (password2.length() > 10)
			{
				cout << endl;
				cout << Menu.formatInput("Password should not longer than 10 character, enter again ");
				continue;
			}
			inputError = false;
		}

		if (password1 != password2)
		{
			cout << Menu.formatInput("Your password is not the same, enter again ");
			inputError = true;
			continue;
		}
		
		inputError = false;
	}
	password = password1;

	// Enter refferalCode
	bool exist = false;
	while (!exist)
	{
		cout << promptLine[4];
		cin >> ReferralCode;

		error.data = ReferralCode;

		for (char& c : ReferralCode)
		{
			c = toupper(c);
		}

		if (error.checkSpace())
		{
			continue;
		}

		db.column_list = "adminID";
		db.table_name = "admin";
		db.condition = "ReferralCode = '" + ReferralCode + "'";

		if (db.select() == true)
		{
			cout << Menu.formatInput("Referral code " + ReferralCode + " exists in the database.");
			
			exist = true;
		}
		else
		{
			cout << Menu.formatInput("Referral code " + ReferralCode + " does not exists in the database. Please enter again.\n");
			
			bool temp = true;
			exist = false;

			string again;
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
				continue;
			}
			else if (again == "N")
			{
				cout << Menu.formatInput("The registration has been canceled.");
				cout << Menu.formatInput("Press any key to exit this page...");
				_getch();
				return false;
			}
		}
	}

	
	if(usertype == "1")
	{
		registerAdmin();
	}
	else if (usertype == "2")
	{
		registerStaff();
	}
}

void Register::registerAdmin() {

	db.table_name = "admin";
	db.column_list = "adminID";
	db.condition = "adminID = (SELECT MAX(adminID) FROM admin)";
	db.select();
	db.row = mysql_fetch_row(db.res);

	//Set Admin ID
	if (db.row == nullptr)
		adminID = "A001";
	else
	{
		adminID = db.row[0];
		if (!adminID.empty() && adminID[0] == 'A') {
			adminID.erase(0, 1); // Remove character A at index 0 
		}

		int num = stoi(adminID);
		num += 1;

		adminID = to_string(num);
		while (adminID.length() < 3)
		{
			adminID = "0" + adminID;
		}
		adminID = "A" + adminID;
	}
	

	//Set random referral code
	srand(time(0)); // Seed the random number generator with the current time 
	int random_number;
	bool exist = true;
	while (exist == true)
	{
		random_number = rand() % 1000; // Generate a random number between 0 and 999 
		ReferralCode = to_string(random_number);
		while (ReferralCode.length() < 3)
		{
			ReferralCode = "0" + ReferralCode;
		}
		ReferralCode = "R" + ReferralCode;
		db.column_list = "ReferralCode";
		db.table_name = "admin";
		db.condition = "ReferralCode = '" + ReferralCode + "'";

		if (db.select() == true) // The generated referral code existed in database
		{
			exist = true;
		}
		else
		{
			exist = false;
		}
	}

	db.table_name = "admin";
	db.column_list = "";
	db.values_list = "'" + adminID + "'" + ", " + "'" + username + "'" + ", " + "'" + password + "'" + ", '' , DEFAULT, DEFAULT, " + "'" + ReferralCode + "'";

	if (!db.add())
	{
		vector <string> adminRegisterPrompt = {
					"Your admin ID will be " + adminID + ". This will use for your login. \n" ,
					"Your refferal code will be " + ReferralCode + ". ",
					"This will used to introduce new staff or new admin during their registration. \n",
					"Press any key to return to the main page... ",
		};
		Menu.formatVectorInput(adminRegisterPrompt);

		cout << endl << endl << endl;
		for (int i = 0; i < adminRegisterPrompt.size(); i++)
		{
			cout << adminRegisterPrompt[i];
		}
		_getch();
	}
	else
	{
		cout << Menu.formatInput("The registration is unsuccessful, please try again");
		_getch();
	}
	
}

void Register::registerStaff() {
	db.row = mysql_fetch_row(db.res);
	adminID = db.row[0];

	db.table_name = "staff_backup";
	db.column_list = "staffID";
	db.condition = "staffID = (SELECT MAX(staffID) FROM staff_backup)";
	db.select();



	db.row = mysql_fetch_row(db.res);


	//Set Staff ID
	if (db.row == nullptr) // no staff in staff table
		staffID = "S001";
	else
	{
		staffID= db.row[0];
		if (!staffID.empty() && staffID[0] == 'S') {
			staffID.erase(0, 1); // Remove character S at index 0 
		}

		int num = stoi(staffID);
		num += 1;

		staffID = to_string(num);
		while (staffID.length() < 3)
		{
			staffID = "0" + staffID;
		}
		staffID = "S" + staffID;
	}
	

	db.table_name = "staff";
	db.column_list = "";
	db.values_list = "'" + staffID + "'" + ", " + "'" + username + "'" + ", " + "'" + password + "'" + ", '' , DEFAULT, DEFAULT, " + "'" + adminID + "'";
	


	if (!db.add())
	{
		vector <string> staffRegisterPrompt = {
					"Based on the referral code, you will under the admin with ID " + adminID + ". \n",
					"Your staff ID will be " + staffID + ". This will use for the login.\n",
					"Press any key to return to the main page... ",
		};
		Menu.formatVectorInput(staffRegisterPrompt);

		cout << endl << endl << endl;
		for (int i = 0; i < staffRegisterPrompt.size(); i++)
		{
			cout << staffRegisterPrompt[i];
		}
	}
	else
	{
		cout << Menu.formatInput("The registration unsuccessful, please try again");
	}
	_getch();
}

Register::~Register() {
	
}
