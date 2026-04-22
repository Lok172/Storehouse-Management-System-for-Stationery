#include "AdminManagement.h"

#include <iostream>
#include <conio.h>
#include <iomanip>
#include <string>
#include <vector>
#include <algorithm>
using namespace std;


Admin::Admin()
{
	userID = "";
	username = "";
	handphoneNumber = "0123456789";
	age = "18";
	marriageStatus = "S";
	reset = "";
	adminID = "";
	ReferralCode = "";
	Menu.menuWidth = 132;
	Menu.outside_left_padding = 15;
	error.ErrorMenu.menuWidth = 132;
	error.ErrorMenu.outside_left_padding = 15;
}

void Admin::viewAdmin() {


	cout << Menu.FormattedRow();
	cout << Menu.formatMenu("Employed Admin Menu");

	cout << endl << Menu.FormattedRow()  << Menu.FormattedRow();

	vector <string> displayColumns = { "No","Admin ID" ,"Admin Name" ,"Password", "Handphone Number", "Age","Marriage Status", "Referral Code" };
	vector <int> displayColumnsSize = { 3,10,40,12,18,5,20,17 };
	string temp = string(15, ' ');

	

	vector<vector<string>> result = {};
	db.column_list = "*";
	db.table_name = "admin";
	db.condition = "adminID LIKE '%'";
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

	//Check the default password in the database
	vector<vector<string>> defaultPassword = {};

	db.column_list = "COLUMN_DEFAULT";
	db.table_name = "INFORMATION_SCHEMA.COLUMNS";
	db.condition = "TABLE_NAME = 'admin' AND COLUMN_NAME = 'password'";
	db.select();
	db.result(defaultPassword);

	defaultPassword[0][0].erase(0, 1);
	defaultPassword[0][0].erase(defaultPassword[0][0].length() - 1, 1);


	//Hide the password if the password is not the default password
	for (int i = 0; i < result.size(); i++)
	{
		if (result[i][2] != defaultPassword[0][0])
		{
			result[i][2] = string(8, '*');
		}
	}


	Menu.displayVectorWrapText(displayColumns, displayColumnsSize, result);


}

bool Admin::updateAdmin() {

	viewAdmin();
	
	db.values_list = "";
	bool inputError = true;

	vector <string> promptLine = {
		"Admin ID (Press X to exit) : " ,
		"Name : " ,
		"Reset Password? (Y/0): ",
		"Handphone Number (X to empty it): ",
		"Age (18-80/-): ",
		"Marriage Status (S/M/-): ",
		"Referral Code (RXXX, X is digits): "
	};

	Menu.formatVectorInput(promptLine);

	bool exist = false;

	// Enter adminID
	while (!exist)
	{
		cout << promptLine[0];
		cin >> adminID;

		for (char& c : adminID)
		{
			c = toupper(c);
		}

		if (adminID != "X")
		{

			error.data = adminID;

			if (error.checkSpace())
			{
				continue;
			}


			db.column_list = "adminID";
			db.table_name = "admin";
			db.condition = "adminID = '" + adminID + "'";

			if (db.select() == true)
			{
				cout << Menu.formatInput("Admin ID " + adminID + " exists in the database.\n");

				exist = true;
			}

			else
			{
				cout << Menu.formatInput("Admin ID " + adminID + " does not exists in the database. Please enter again.\n");

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
					return true;
				}
				else if (again == "N")
				{
					cout << Menu.formatInput("The update has been canceled.");
					cout << Menu.formatInput("Press any key to exit this page...");
					_getch();
					return false;
				}
			}
		}

		else
		{
			exist = true;
		}
	}

	cout << Menu.formatInput("Please follow the instruction to fill in, 0 to not change\n");
	cin.ignore();

	//If the user does not want to exit
	if (adminID != "X")
	{

		//Enter Name
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

			if (username != "0")
			{
				if (db.values_list == "")
					db.values_list += "AdminName = '" + username + "' ";
			}

			inputError = false;
		}

		//Enter reset password
		inputError = true;
		while (inputError == true)
		{
			cout << promptLine[2];
			cin >> reset;

			for (char& c : reset)
			{
				c = toupper(c);
			}

			error.data = reset;

			if (error.checkOption("Y", "0"))
			{
				continue;
			}

			if (reset == "0")
			{
				inputError = false;
			}

			if (reset == "Y")
			{

				inputError = false;

				if (db.values_list == "")
					db.values_list += " Password = DEFAULT(Password) ";
				else
					db.values_list += ", Password = DEFAULT(Password) ";
			}

		}

		//Enter handphone number
		inputError = true;
		while (inputError == true)
		{
			cout << promptLine[3];
			cin >> handphoneNumber;

			for (char& c : handphoneNumber)
			{
				c = toupper(c);
			}

			if (handphoneNumber != "0")
			{
				if (handphoneNumber != "X")
				{
					error.data = handphoneNumber;

					if (error.checkSpace() || error.checkDigit())
					{
						continue;
					}

					if ((handphoneNumber.length() < 10) || (handphoneNumber.length() > 11))
					{
						cout << Menu.formatInput("Handphone Number should be 10 or 11 digits only, enter again ");
						continue;
					}
				}

				else if (handphoneNumber == "X")
				{
					handphoneNumber = "";
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

		//Enter age
		inputError = true;
		while (inputError == true)
		{
			cout << promptLine[4];
			cin >> age;
			error.data = age;

			if (age != "0")
			{
				if (age != "-")
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
				}

				else if (age == "-")
				{
					age = "-1";
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

		//Enter Marriage Status 
		inputError = true;
		while (inputError == true)
		{
			cout << promptLine[5];
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
				if (marriageStatus != "-")
				{
					if (error.checkOption("S", "M"))
					{
						continue;
					}
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

		// Enter ReferralCode
		exist = false;
		while (!exist)
		{
			cout << promptLine[6];
			cin >> ReferralCode;
			for (char& c : ReferralCode)
			{
				c = toupper(c);
			}

			error.data = ReferralCode;

			if (ReferralCode != "0")
			{
				if (error.checkSpace())
				{
					continue;
				}

				else if (ReferralCode.size() != 4)
				{
					cout << Menu.formatInput("The referral code should be exactly 4 characters, please enter again.");
					continue;
				}

				else if (ReferralCode[0] != 'R')
				{
					cout << Menu.formatInput("Please follow the format, please enter again           ");
					continue;
				}


				else if (ReferralCode[0] == 'R')
				{
					string temp = "";
					for (int i = 1; i < 4; i++)
					{
						temp += ReferralCode[i];
					}

					if (!(all_of(temp.begin(), temp.end(), isdigit)))
					{
						cin.clear();
						cin.ignore(1000, '\n');
						cout << Menu.formatInput("Please follow the format, please enter again           ");
						continue;
					}
				}
				db.column_list = "ReferralCode";
				db.table_name = "admin";
				db.condition = "ReferralCode = '" + ReferralCode + "'";

				if (db.select() == true)
				{
					cout << Menu.formatInput("Referral Code " + ReferralCode + " exists in the database. \n");
					cout << Menu.formatInput("The Referral Code cannot be updated. Please enter again. \n");
					exist = false;
				}

				else
				{
					cout << Menu.formatInput("Referral Code " + ReferralCode + " does not exists in the database. \n");
					cout << Menu.formatInput("The Referral Code can be updated\n");

					if (db.values_list == "")
						db.values_list += "ReferralCode = '" + ReferralCode + "' ";
					else
						db.values_list += ", ReferralCode = '" + ReferralCode + "' ";

					exist = true;
				}
			}
			else if (ReferralCode == "0")
			{
				exist = true;
			}
		}

		bool temp;
		string confirmation;

		//Update admin information
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
				db.table_name = "admin";
				db.condition = "adminID = '" + adminID + "'";

				if (!db.update())
				{
					cout << Menu.formatInput("The data updated\n");
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
			cout << Menu.formatInput("Do you want to update another? (Y/N) : ");

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

bool Admin::deleteAdmin() {
	viewAdmin();
	
	
	bool exist = false;

	// Enter adminID
	while (!exist)
	{
		cout << Menu.formatInput("Admin ID (Press X to exit) : ");
		cin >> adminID;

		for (char& c : adminID)
		{
			c = toupper(c);
		}


		if (adminID != "X")
		{
			error.data = adminID;

			if (error.checkSpace())
			{
				continue;
			}

			if (adminID == userID)
			{
				cout << Menu.formatInput("Admin ID matches User ID.");
				cout << Menu.formatInput("You cannot delete your own. Please enter again.\n");
				continue;
			}


			db.column_list = "adminID";
			db.table_name = "admin";
			db.condition = "adminID = '" + adminID + "'";

			if (db.select() == true)
			{
				db.column_list = "StaffID";
				db.table_name = "Staff";
				db.condition = "AdminID = '" + adminID + "'";


				bool test1 = db.select();

				db.column_list = "SupplierID";
				db.table_name = "Supplier";
				db.condition = "AdminID = '" + adminID + "'";

				bool test2 = db.select();


				//If the admin is the  the supervisor of the staff(s)
				if (test1 == true || test2 == true)
				{
					cout << Menu.formatInput("The admin with ID " + adminID + " is the supervisor of the staff(s) and/or supplier(s).");
					cout << Menu.formatInput("The admin cannot be deleted\n");

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
						return true;
					}
					else if (again == "N")
					{
						cout << Menu.formatInput("The deletion has been canceled.");
						cout << Menu.formatInput("Press any key to exit this page...");
						_getch();
						return false;
					}
				}

				else
				{
					cout << Menu.formatInput("Admin ID " + adminID + " exists in the database.\n");
						exist = true;
				}
			}

			else
			{
				cout << Menu.formatInput("Admin ID " + adminID + " does not exists in the database. Please enter again.\n");

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
					return true;
				}
				else if (again == "N")
				{
					cout << Menu.formatInput("The deletion has been canceled.");
					cout << Menu.formatInput("Press any key to exit this page...");
					_getch();
					return false;
				}
			}
		}
		else
		{
			exist = true;
		}
	}

	//If the user does not want to exit
	if (adminID != "X")
	{

		bool temp;
		string confirmation;


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
			db.table_name = "Admin";
			db.condition = "AdminID = '" + adminID + "'";

			if (!db.Delete())
			{
				cout << Menu.formatInput("The admin with admin ID " + adminID + " is successfuly deleted\n");
			}
			else
			{
				cout << Menu.formatInput("The admin with admin ID " + adminID + " is unsuccessfully delete\n");
			}
		}

		else if (confirmation == "N")
		{
			cout << Menu.formatInput("The deletion has been canceled.\n");
		}



		temp = true;
		string again;
		while (temp == true)
		{
			cout << Menu.formatInput("Do you want to delete another? (Y/N) : ");

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

Admin::~Admin()
{

}