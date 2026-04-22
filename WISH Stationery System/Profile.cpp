#include "Profile.h"

#include <iostream>
#include <conio.h>
#include <iomanip>
#include <string>
#include <vector>
#include <algorithm>
using namespace std;


Profile::Profile()
{
	username = "";
	handphoneNumber = "0123456789";
	age = "18";
	marriageStatus = "S";
	password = "";
	ReferralCode = "";
	userID = "";
	Menu.menuWidth = 128;
	Menu.outside_left_padding = 20;
	error.ErrorMenu.menuWidth = 128;
	error.ErrorMenu.outside_left_padding = 20;
}

void Profile::viewProfile() {

	vector <string> displayColumns;

	if (usertype == "admin")
	{
		displayColumns = { "Admin ID" ,"Admin Name" ,"Password", "Handphone Number", "Age","Marriage Status", "Referral Code" };
		db.column_list = "*";
		db.table_name = "admin";
		db.condition = "adminID = '" + userID + "' ";
	}

	else if (usertype == "staff")
	{
		displayColumns = { "Staff ID" ,"Staff Name" ,"Password", "Handphone Number", "Age","Marriage Status", "Admin ID" };
		db.column_list = "*";
		db.table_name = "staff";
		db.condition = "staffID = '" + userID + "' ";
	}

	//Display Header
	cout << Menu.FormattedRow();
	cout << Menu.formatMenu("Profile Information");
	cout << endl << Menu.FormattedRow() << Menu.FormattedRow();


	//Display column name
	vector <int> displayColumnsSize = { 10,40,12,18,5,20,17 };
	


	//Retrieve result
	vector<vector<string>> result = {};
	db.select();
	db.result(result);

	//Format the age to - if it is -1 which is the default value
	for (int i = 0; i < result.size(); i++)
	{
		if (result[i][4] == "-1")
		{
			result[i][4] = "-";
		}
	}

	//Format the name with all capital letter
	for (int i = 0; i < result.size(); i++)
	{
		for (char& c : result[i][1])
		{
			c = toupper(c);
		}
	}

	//Store password for the viewPassword function
	password = result[0][2];

	//Hide the password 
	result[0][2] = string(8, '*');




	





	string temp = string(20, ' ');
	for (int i = 0; i < displayColumns.size(); i++)
	{
		temp += Menu.centerText(displayColumns[i], displayColumnsSize[i]);
	}

	temp = temp + '|';
	cout << temp << endl;
	cout << Menu.FormattedRow();
	//Display Information
	for (int i = 0; i < result.size(); i++)
	{
		temp = {};
		temp = string(Menu.outside_left_padding, ' ') ;


		for (int j = 0; j < result[i].size(); j++)
		{
			temp += Menu.centerText(result[i][j], displayColumnsSize[j]);
			cout << temp;
			temp = {};
		}

		cout << '|';
		cout << endl;
	}

	cout << Menu.FormattedRow();

}

void Profile::viewPassword() {

	string temp = "Password: " + password + string(20, ' ');
	cout << endl<< Menu.formatInput(temp);
	cout << Menu.formatInput("Press any key to exit this page. " + string(8, ' '));
	_getch();

}

bool Profile::addInformation() {

	viewProfile();
	vector<vector<string>> result = {};
	

	db.values_list = "";
	if (usertype == "admin")
	{
		db.column_list = "HandphoneNo, Age, MarriageStatus";
		db.table_name = "admin";
		db.condition = "adminID = '" + userID + "' ";
	}
	else if (usertype == "staff")
	{
		db.column_list = "HandphoneNo, Age, MarriageStatus";
		db.table_name = "staff";
		db.condition = "staffID = '" + userID + "' ";
	}

	db.select();
	db.result(result);
		
	vector <string> promptLine = {
		"Handphone Number : " ,
		"Age (18-80): ",
		"Marriage Status (S/M): ",
	};
	
	Menu.formatVectorInput(promptLine);
	cout << Menu.formatInput("You can add information to the columns for Handphone Number, Age, Marriage Status if they are not contained value.");
	cout << Menu.formatInput("0 to not change.\n");


	//Check whether the columns contained values or not
	int containedValues = 0;
	if (result[0][0] != "")
	{
		containedValues++;
	}

	if (result[0][1] != "-1")
	{
		containedValues++;
	}

	if (result[0][2] != "-")
	{
		containedValues++;
	}

	if (containedValues == 3)
	{
		cout << Menu.formatInput("All three columns are contained values.");
		cout << Menu.formatInput("The addition was canceled.");
		cout << Menu.formatInput("Please go to update information page if you wish to change them.") << endl;
		cout << Menu.formatInput("Press any key to exit this page...");
		_getch();
		return false;
	}

	else if (containedValues != 3)
	{
		bool inputError;

		//Enter handphone number if no information available
		if (result[0][0] == "")
		{
				inputError = true;
				while (inputError == true)
			{
				cout << promptLine[0];
				cin >> handphoneNumber;
				
				error.data = handphoneNumber;


				if (handphoneNumber != "0")
				{

					if (error.checkSpace() || error.checkDigit())
					{
						continue;
					}

					if ((handphoneNumber.length() < 10) || (handphoneNumber.length() > 11))
					{
						cout << Menu.formatInput("Handphone Number should be 10 or 11 digits only, enter again ");
						continue;
					}

					inputError = false;

					if (db.values_list == "")
						db.values_list += "HandphoneNo = '" + handphoneNumber + "' ";
					else
						db.values_list += ", HandphoneNo = '" + handphoneNumber + "' ";
				}


				else if (handphoneNumber == "0")
				{
					inputError = false;
				}
			}
		}
		else
		{
			cout << Menu.formatInput("The handphone number contained value, please go to update information page if you wish to change it\n");
		}

		//Enter age if no information available
		if (result[0][1] == "-1")
		{
			inputError = true;
			while (inputError == true)
			{
				cout << promptLine[1];
				cin >> age;
				error.data = age;

				if (age != "0")
				{

					if (error.checkDigit() || error.checkSpace())
					{
						continue;
					}
					if (stoi(age) < 18 || stoi(age) > 80)
					{
						cout << Menu.formatInput("Age should be in between 18 and 80, enter again ");
						continue;
					}

					inputError = false;

					if (db.values_list == "")
						db.values_list += "Age = '" + age + "' ";
					else
						db.values_list += ", Age = '" + age + "' ";

				}

				else if (age == "0")
				{
					inputError = false;
				}
			}
		}
		else
		{
			cout << Menu.formatInput("The age contained value, please go to update information page if you wish to change it\n");
		}


		//Enter Marriage Status if no information available
		if (result[0][2] == "-")
		{
			inputError = true;
			while (inputError == true)
			{
				cout << promptLine[2];
				cin >> marriageStatus;

				for (char& c : marriageStatus)
				{
					c = toupper(c);
				}

				error.data = marriageStatus;

				if (error.checkSpace())
				{
					continue;
				}

				if (marriageStatus != "0")
				{

					if (error.checkOption("S", "M"))
					{
						continue;
					}


					inputError = false;

					if (db.values_list == "")
						db.values_list += "MarriageStatus = '" + marriageStatus + "' ";
					else
						db.values_list += ", MarriageStatus = '" + marriageStatus + "' ";
				}

				else if (marriageStatus == "0")
				{
					inputError = false;
				}
			}
		}
		else
		{
			cout << Menu.formatInput("The marriage status contained value, please go to update information page if you wish to change it\n");
		}

		bool temp = true;
		string confirmation = {};
		if (db.values_list != "")
		{
			while (temp == true)
			{
				cout << Menu.formatInput("Confirm to add information? (Y/N) : ");

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
			
					if (usertype == "admin")
					{
						db.table_name = "admin";
						db.condition = "adminID = '" + userID + "'";
					}
					else if (usertype == "staff")
					{
						db.table_name = "staff";
						db.condition = "staffID = '" + userID + "'";
					}

					if (!db.update())
					{
						cout << Menu.formatInput("The data is added\n");
					}

					else
					{
						cout << Menu.formatInput("The data is not added\n");
						temp = true;
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
							return true;
						}
						else if (again == "N")
						{
							cout << Menu.formatInput("Press any key to exit this page...");
							_getch();
							return false;
						}
					}
				

			}

			else if (confirmation == "N")
			{
				cout << Menu.formatInput("The addition is canceled.\n");
				cout << Menu.formatInput("Press any key to exit this page...");
				_getch();
				return false;
			}
		}
		else if (db.values_list == "")
		{
			cout << Menu.formatInput("No data needs to be added");
			cout << Menu.formatInput("Press any key to exit this page...");
			_getch();
			return false;
		}


		temp = true;
		string again;
		while (temp == true)
		{
			cout << Menu.formatInput("Do you want to add again? (Y/N) : ");

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
			cout << Menu.formatInput("Press any key to exit this page...");
			_getch();
			return false;
		}
	}
	
}

bool Profile::updateInformation()
{
	viewProfile();
	vector<vector<string>> result = {};

	db.values_list = ""; 
	if (usertype == "admin")
	{
		db.column_list = "HandphoneNo, Age, MarriageStatus";
		db.table_name = "admin";
		db.condition = "adminID = '" + userID + "' ";
	}
	else if (usertype == "staff")
	{
		db.column_list = "HandphoneNo, Age, MarriageStatus";
		db.table_name = "staff";
		db.condition = "staffID = '" + userID + "' ";
	}

	db.select();
	db.result(result);

	vector <string> promptLine = {
		"Name : " ,
		"Password (<= 10 characters and/or digits): ",
		"Handphone Number : ",
		"Age (18-80): ",
		"Marriage Status (S/M): ",
	};


	Menu.formatVectorInput(promptLine);
	cout << Menu.formatInput("You can update information to the columns for Name, Password, ");
	cout << Menu.formatInput("Handphone Number, Age, Marriage Status if they are contained values.");
	cout << endl;
	cout << Menu.formatInput("0 to not change.\n");




	//Enter Name
	bool inputError = true;
	while (inputError == true)
	{
		cout << promptLine[0];
		getline(cin, username);

		error.data = username;

		if (error.checkEmpty())
		{
			continue;
		}

		if (username != "0")
		{
			if (usertype == "admin")
			{
				if (db.values_list == "")
					db.values_list += "AdminName = '" + username + "' ";
			}
			else if (usertype == "staff")
			{
				if (db.values_list == "")
					db.values_list += "StaffName = '" + username + "' ";
			}
		}
		inputError = false;
	}

	//Enter password
	inputError = true;
	while (inputError == true)
	{
		cout << Menu.formatInput("Press TAB to show password");
		cout << promptLine[1];
		password = Menu.hideInput();
		cout << endl;

		if (password != "0")
		{
			error.data = password;
			if (error.checkEmpty())
			{
				continue;
			}

			if (password.length() > 10)
			{
				cout << Menu.formatInput("Password should not longer than 10 character, enter again\n ");
				continue;
			}
			inputError = false;

			if (db.values_list == "")
				db.values_list += "Password = '" + password + "' ";
			else
				db.values_list += ", Password = '" + password + "' ";
		}

		else if (password == "0")
		{
			inputError = false;
		}
	}

	int containedValues = 0;
	if (result[0][0] == "")
	{
		containedValues++;
	}
	if (result[0][1] == "-1")
	{
		containedValues++;
	}
	if (result[0][2] == "-")
	{
		containedValues++;
	}


	if (containedValues == 3)
	{
		cout << Menu.formatInput("Handphone Number, Age and Marriage Status are not contained values.");
		cout << Menu.formatInput("Please go to add information page if you wish to add them.\n");
	}

	else if (containedValues != 3)
	{
		//Enter handphone number if the information available
		if (result[0][0] != "")
		{
			inputError = true;
			while (inputError == true)
			{
				cout << promptLine[2];
				cin >> handphoneNumber;
				
				if (handphoneNumber != "0")
				{
					error.data = handphoneNumber;
					if (error.checkDigit())
					{
						continue;
					}
					if ((handphoneNumber.length() < 10) || (handphoneNumber.length() > 11))
					{
						cout << Menu.formatInput("Handphone Number should be 10 or 11 digits only, enter again ");
						continue;
					}
					

					inputError = false;

					if (db.values_list == "")
						db.values_list += "HandphoneNo = '" + handphoneNumber + "' ";
					else
						db.values_list += ", HandphoneNo = '" + handphoneNumber + "' ";
				}


				else if (handphoneNumber == "0")
				{
					inputError = false;
				}
			}
		}
		else
		{
			cout << Menu.formatInput("The handphone number does not contained value, please go to add information page to add it\n");
		}

		//Enter age if the information available
		if (result[0][1] != "-1")
		{
			inputError = true;
			while (inputError == true)
			{
				cout << promptLine[3];
				cin >> age;
				error.data = age;

				if (age != "0")
				{
					if (error.checkDigit() || error.checkSpace())
					{
						continue;
					}
					if (stoi(age) < 18 || stoi(age) > 80)
					{
						cout << Menu.formatInput("Age should be in between 18 and 80, enter again ");
						continue;
					}

					inputError = false;

					if (db.values_list == "")
						db.values_list += "Age = '" + age + "' ";
					else
						db.values_list += ", Age = '" + age + "' ";

				}

				else if (age == "0")
				{
					inputError = false;
				}
			}
		}
		else
		{
			cout << Menu.formatInput("The age does not contained value, please go to add information page to add it\n");
		}

		//Enter marriage status if the information available
		if (result[0][2] != "-")
		{
			inputError = true;
			while (inputError == true)
			{
				cout << promptLine[4];
				cin >> marriageStatus;

				for (char& c : marriageStatus)
				{
					c = toupper(c);
				}

				error.data = marriageStatus;

				if (error.checkSpace())
				{
					continue;
				}

				if (marriageStatus != "0")
				{

					if (error.checkOption("S", "M"))
					{
						continue;
					}


					inputError = false;

					if (db.values_list == "")
						db.values_list += "MarriageStatus = '" + marriageStatus + "' ";
					else
						db.values_list += ", MarriageStatus = '" + marriageStatus + "' ";
				}

				else if (marriageStatus == "0")
				{
					inputError = false;
				}
			}
		}
		else
		{
			cout << Menu.formatInput("The marriage status does not contained value, please go to add information page to add it\n");
		}
	}


	bool temp ;
	string confirmation;
	if (db.values_list != "")
	{
		temp = true;
		confirmation = {};
		while (temp == true)
		{
			cout << Menu.formatInput("Confirm to update? (Y/N) : ");

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
			if (usertype == "admin")
			{
				db.table_name = "admin";
				db.condition = "adminID = '" + userID + "'";
			}
			else if (usertype == "staff")
			{
				db.table_name = "staff";
				db.condition = "staffID = '" + userID + "'";
			}

			if (!db.update())
			{
				cout << Menu.formatInput("The data is updated\n");
			}
			else
			{
				cout << Menu.formatInput("The data is not updated\n");
			}
		}
		else if (confirmation == "N")
		{
			cout << Menu.formatInput("The update has been canceled.\n");
		}
	}
	else if (db.values_list == "")
	{
		cout << Menu.formatInput("No data needs to be updated\n");
	}

	temp = true;
	string again;
	while (temp == true)
	{
		cout << Menu.formatInput("Do you want to update again? (Y/N) : ");

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
		cout << Menu.formatInput("Press any key to exit this page...");
		_getch();
		return false;
	}
}

bool Profile::deleteInformation()
{
	viewProfile();
	vector<vector<string>> result = {};
	string option;
	bool inputError, temp;
	db.values_list = "";

	if (usertype == "admin")
	{
		db.column_list = "HandphoneNo, Age, MarriageStatus";
		db.table_name = "admin";
		db.condition = "adminID = '" + userID + "' ";
	}
	else if (usertype == "staff")
	{
		db.column_list = "HandphoneNo, Age, MarriageStatus";
		db.table_name = "staff";
		db.condition = "staffID = '" + userID + "' ";
	}

	db.select();
	db.result(result);

	
	cout << Menu.formatInput("You can delete information for the columns Handphone Number, Age, Marriage Status");
	cout << Menu.formatInput("if they are contained values.");
	cout << endl;
	cout << Menu.formatInput("0 to not change.\n");

	vector <string> optionList = {
			"1) Handphone Number",
			"2) Age",
			"3) Marriage Status",
	};



	int containedValues = 0;
	if (result[0][0] == "")
	{
		containedValues++;
	}
	if (result[0][1] == "-1")
	{
		containedValues++;
	}
	if (result[0][2] == "-")
	{
		containedValues++;
	}


	if (containedValues == 3)
	{
		cout << Menu.formatInput("Handphone Number, Age and Marriage Status are not contained values.");
		cout << Menu.formatInput("Please go to add information page if you wish to add them.\n");
		cout << Menu.formatInput("Press any key to exit this page...");
		_getch();
	}
	else if (containedValues != 3)
	{	
		cout << Menu.formatInput("Option List");
		Menu.formatVectorInput(optionList);
		
		for (int i = 0; i < optionList.size(); i++)
		{
			cout << optionList[i];
		}

		inputError = true;
		while (inputError == true)
		{
			option = {};
			cout << endl<<  Menu.formatInput("Option: ");
			cin >> option;
			error.data = option;

			if (error.checkDigit() || error.checkSpace())
			{
				continue;
			}

			if (option.length() != 1)
			{
				cout << Menu.formatInput("Enter exactly one character only. Enter again.\n");
				continue;
			}

			if (option != "0" && option != "1" && option != "2" && option != "3")
			{
				cout << Menu.formatInput("Enter according to the instruction\n");
				continue;
			}
			inputError = false;
		}


		if (option == "0")
		{
			db.values_list == "";
		}

		//Check if the handphone number empty
		else if (option == "1")
		{
			if (result[0][0] != "")
			{
				handphoneNumber = "";
				if (db.values_list == "")
					db.values_list += "HandphoneNo = '" + handphoneNumber + "' ";
				else
					db.values_list += ", HandphoneNo = '" + handphoneNumber + "' ";
			}

			else if (result[0][0] == "")
			{
				cout << Menu.formatInput("Handphone Number is empty and therefore cannot be deleted.\n");
			}
		}

		//Check if the age empty
		else if (option == "2")
		{
			if (result[0][1] != "-1")
			{
				age = "-1";
				if (db.values_list == "")
					db.values_list += "Age = '" + age + "' ";
				else
					db.values_list += ", Age = '" + age + "' ";
			}

			else if (result[0][1] == "-1")
			{
				cout << Menu.formatInput("Age is empty and therefore cannot be deleted.\n");
			}
		}

		//Check if the Marriage Status empty
		else if (option == "3")
		{
			if (result[0][2] != "-")
			{
				marriageStatus = "-";
				if (db.values_list == "")
					db.values_list += "MarriageStatus = '" + marriageStatus + "' ";
				else
					db.values_list += ", MarriageStatus = '" + marriageStatus + "' ";
			}
			else if (result[0][2] == "-")
			{
				cout << Menu.formatInput("Marriage Status is empty and therefore cannot be deleted.\n");
			}
		}

		
		string confirmation;
		if (db.values_list == "")
		{
			cout << Menu.formatInput("No data needs to be deleted\n");
		}

		else if (db.values_list != "")
		{
			temp = true;
			confirmation = {};
			while (temp == true)
			{
				cout << Menu.formatInput("Confirm to delete? (Y/N) : ");

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
				if (usertype == "admin")
				{
					db.table_name = "admin";
					db.condition = "adminID = '" + userID + "'";
				}
				else if (usertype == "staff")
				{
					db.table_name = "staff";
					db.condition = "staffID = '" + userID + "'";
				}

				if (!db.update())
				{
					cout << Menu.formatInput("The information deleted\n");
				}
				else
				{
					cout << Menu.formatInput("The information is not deleted\n");
				}
			}
			else if (confirmation == "N")
			{
				cout << Menu.formatInput("The deletion has been canceled.\n");
			}
		}



		temp = true;
		string again;
		while (temp == true)
		{
			cout << Menu.formatInput("Do you want to delete again? (Y/N) : ");

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
			cout << Menu.formatInput("Press any key to exit this page...");
			_getch();
			return false;
		}
	}
}


Profile::~Profile()
{

}