#include "Supplier.h"

#include <iostream>
#include <conio.h>
#include <iomanip>
#include <string>
#include <vector>
#include <algorithm>
using namespace std;


Supplier::Supplier()
{
	supplierID = "";
	supplierName = "";
	address = "";
	telephoneNumber = "";
	adminID = "";


	Menu.menuWidth = 130;
	Menu.outside_left_padding = 15;
	error.ErrorMenu.menuWidth = 130;
	error.ErrorMenu.outside_left_padding = 15;
}

void Supplier::viewSupplier() {

	cout << Menu.FormattedRow();
	cout << Menu.formatMenu("Supplier Menu");

	cout << endl << Menu.FormattedRow() << Menu.FormattedRow();

	//Format and display the columns
	vector <string> displayColumns = {"No", "Supplier ID" ,"Supplier Name" ,"Address", "Telephone Number", "Admin ID" };
	vector <int> displayColumnsSize = {3, 14, 40, 40, 18, 10};
	

	vector<vector<string>> result = {};
	db.column_list = "*";
	db.table_name = "supplier";
	db.condition = "SupplierID LIKE '%' ";
	db.select();
	db.result(result);




	//Make all the text capital letters
	Menu.capitalizedVectorVectorText(result);

	//Display Information
	Menu.displayVectorWrapText(displayColumns, displayColumnsSize, result);
	
}

bool Supplier::addSupplier() {

	viewSupplier();
	vector <string> promptLine = { "Supplier Name : " ,
					"Address : ",
					"Telephone Number : ",
					"Admin ID : ",
					string(110,' ')
	};

	Menu.formatVectorInput(promptLine);


	
	cout << Menu.formatInput("Please follow the instruction to fill in. ");

	// Enter Supplier Name
	bool inputError = true;
	while (inputError == true)
	{
		cout << promptLine[0];
		getline(cin, supplierName);

		error.data = supplierName;

		if (error.checkEmpty())
		{
			continue;
		}
		inputError = false;
	}

	// Enter Address
	inputError = true;
	while (inputError == true)
	{
		cout << promptLine[1];
		getline(cin, address);

		error.data = address;

		if (error.checkEmpty())
		{
			continue;
		}
		inputError = false;
	}

	// Enter Telephone Number
	inputError = true;
	while (inputError == true)
	{
		cout << promptLine[2];
		cin >> telephoneNumber;

		error.data = telephoneNumber;

		if (error.checkSpace() || error.checkDigit())
		{
			continue;
		}

		if ((telephoneNumber.length() < 9) || (telephoneNumber.length() > 13))
		{
			cout << Menu.formatInput("Telephone number should be 9 to 13 digits only, enter again.\n");
			continue;
		}

		inputError = false;

	}

	// Enter adminID
	bool exist = false;
	while (!exist)
	{
		cout << promptLine[3];
		cin >> adminID;

		for (char& c : adminID)
		{
			c = toupper(c);
		}

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
			cout << Menu.formatInput("The Admin ID can be added\n");

			if (db.values_list == "")
				db.values_list += "adminID = '" + adminID + "' ";
			else
				db.values_list += ", adminID = '" + adminID + "' ";

			exist = true;
		}

		else
		{
			cout << Menu.formatInput("Admin ID " + adminID + " does not exists in the database. The Admin ID cannot be added.\n");

			//Ask repeat again or not
			bool temp = true;
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


				if (again == "Y")
				{
					exist = false;
				}

				else if (again == "N")
				{
					cout << endl << Menu.formatInput("The addition is canceled.\n");
					cout << Menu.formatInput("Press any key to exit this page...");
					_getch();
					return false;
				}
			}

		}

		
	}

	//Ask Confirmation
	bool temp = true;
	string confirmation = {};
	while (temp == true)
	{
		cout << Menu.formatInput("Confirm to add supplier? (Y/N) : ");

		cin >> confirmation;
		for (char& c : confirmation)
		{
			c = toupper(c);
		}
		error.data = confirmation;

		temp = (error.checkOption("Y", "N") || error.checkSpace());
	}

	//Add supplier to database
	if (confirmation == "Y")
	{
		db.table_name = "supplier_backup";
		db.column_list = "supplierID";
		db.condition = "supplierID = (SELECT MAX(supplierID) FROM supplier_backup)";
		db.select();
		db.row = mysql_fetch_row(db.res);

		//Set supplier ID
		if (db.row == nullptr)
			supplierID = "C001";
		else
		{
			supplierID = db.row[0];
			if (!supplierID.empty() && supplierID[0] == 'C') {
				supplierID.erase(0, 1); // Remove character C at index 0 
			}

			int num = stoi(supplierID);
			num += 1;

			supplierID = to_string(num);
			while (supplierID.length() < 3)
			{
				supplierID = "0" + supplierID;
			}
			supplierID = "C" + supplierID;
		}

		db.table_name = "supplier";
		db.column_list = "";
		db.values_list = "'" + supplierID +"' , '" + supplierName + "' , '" + address + "' , '" + telephoneNumber + "' , '" + adminID + "'";

		if (!db.add())
		{
			vector <string> supplierPrompt = {
						"The addition is successful. ",
						"The new supplier will have supplier ID " + supplierID + ". \n" 
			};
			Menu.formatVectorInput(supplierPrompt);

			cout << endl ;
			for (int i = 0; i < supplierPrompt.size(); i++)
			{
				cout << supplierPrompt[i];
			}
		}
		else
		{
			cout << Menu.formatInput("The addition is unsuccessful, please try again.\n");
		}

	}

	else if (confirmation == "N")
	{
		cout << Menu.formatInput("The addition is canceled.\n");
	}

	//Ask repeat again or not
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

bool Supplier::updateSupplier() {

	viewSupplier();

	db.values_list = "";

	vector <string> promptLine = {
		"Supplier ID (Press X to exit) : " ,
		"Supplier Name : " ,
		"Address : ",
		"Telephone Number : ",
		"Admin ID : ",
		string(110,' ')
	};

	Menu.formatVectorInput(promptLine);


	// Enter supplierID
	bool exist = false;
	while (!exist)
	{
		cout << promptLine[0];
		cin >> supplierID;

		for (char& c : supplierID)
		{
			c = toupper(c);
		}

		if (supplierID != "X")
		{

			error.data = supplierID;

			if (error.checkSpace())
			{
				continue;
			}


			db.column_list = "supplierID";
			db.table_name = "supplier";
			db.condition = "supplierID = '" + supplierID + "'";

			if (db.select() == true)
			{
				cout << Menu.formatInput("Supplier ID " + supplierID + " exists in the database.\n");

				exist = true;
			}

			else
			{
				cout << Menu.formatInput("Supplier ID " + supplierID + " does not exists in the database. Please enter again.\n");

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

	cin.ignore();


	if (supplierID != "X")
	{

		cout << Menu.formatInput("Please follow the instruction to fill in, 0 to not change\n");

		// Enter Supplier Name
		bool inputError = true;
		while (inputError == true)
		{
			cout << promptLine[1];
			getline(cin, supplierName);

			error.data = supplierName;

			if (error.checkEmpty())
			{
				continue;
			}

			if (supplierName != "0")
			{
				if (db.values_list == "")
					db.values_list += "supplierName = '" + supplierName + "' ";
			}

			inputError = false;
		}

		// Enter Address
		inputError = true;
		while (inputError == true)
		{
			cout << promptLine[2];
			getline(cin, address);

			if (address != "0")
			{
				error.data = address;

				if (error.checkEmpty())
				{
					continue;
				}

				if (db.values_list == "")
					db.values_list += "Address = '" + address + "' ";
				else
					db.values_list += ", Address = '" + address + "' ";


				inputError = false;
			}

			else
			{
				inputError = false;
			}
		}

		// Enter Telephone Number
		inputError = true;
		while (inputError == true)
		{
			cout << promptLine[3];
			cin >> telephoneNumber;

			if (telephoneNumber != "0")
			{
				error.data = telephoneNumber;

				if (error.checkSpace() || error.checkDigit())
				{
					continue;
				}

				if ((telephoneNumber.length() < 9) || (telephoneNumber.length() > 13))
				{
					cout << Menu.formatInput("Supplier Telephone number should be 9 to 13 digits only, enter again ");
					continue;
				}

				if (db.values_list == "")
					db.values_list += "TelephoneNumber = '" + telephoneNumber + "' ";
				else
					db.values_list += ", TelephoneNumber = '" + telephoneNumber + "' ";

				inputError = false;
			}

			else
				inputError = false;
		}

		// Enter adminID
		exist = false;
		while (!exist)
		{
			cout << promptLine[4];
			cin >> adminID;
			if (adminID != "0")
			{
				for (char& c : adminID)
				{
					c = toupper(c);
				}

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
					cout << Menu.formatInput("The Admin ID can be updated\n");

					if (db.values_list == "")
						db.values_list += "adminID = '" + adminID + "' ";
					else
						db.values_list += ", adminID = '" + adminID + "' ";

					exist = true;
				}

				else
				{
					cout << Menu.formatInput("Admin ID " + adminID + " does not exists in the database.\n");
					cout << Menu.formatInput("The Admin ID cannot be updated. Please enter again. \n");

					exist = false;
				}
			}
			else if (adminID == "0")
			{
				exist = true;
			}
		}


		bool temp;
		string confirmation;

		//Update supplier information
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
				db.table_name = "supplier";
				db.condition = "supplierID = '" + supplierID + "'";

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


		//Repeat again or not
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

bool Supplier::deleteSupplier() {
	viewSupplier();


	bool exist = false;

	// Enter supplierID
	while (!exist)
	{
		cout << Menu.formatInput("Supplier ID (Press X to exit) : ");
		cin >> supplierID;

		for (char& c : supplierID)
		{
			c = toupper(c);
		}


		if (supplierID != "X")
		{

			error.data = supplierID;

			if (error.checkSpace())
			{
				continue;
			}


			db.column_list = "supplierID";
			db.table_name = "supplier";
			db.condition = "supplierID = '" + supplierID + "'";

			if (db.select() == true)
			{

				{
					cout << Menu.formatInput("Supplier ID " + supplierID + " exists in the database.\n");
					exist = true;
				}
			}

			else
			{
				cout << Menu.formatInput("Supplier ID " + supplierID + " does not exists in the database. Please enter again.\n");

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



	if (supplierID != "X")
	{
		bool temp;
		string confirmation;


		//Confirmation to delete
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
			db.table_name = "supplier";
			db.condition = "supplierID = '" + supplierID + "'";

			if (!db.Delete())
			{
				cout << Menu.formatInput("The supplier with supplier ID " + supplierID + " is successfuly deleted\n");


				//Delete the supplier ID also from the supplier_backup if it is not found from the record_restock table
				db.column_list = "supplierID";
				db.table_name = "record_restock";
				db.condition = "supplierID = '" + supplierID + "'";

				if (db.select() == false)
				{
					db.table_name = "supplier_backup";
					db.condition = "supplierID = '" + supplierID + "'";
					db.Delete();
				}
			}

			else
			{
				cout << Menu.formatInput("The supplier with supplier ID " + supplierID + " is unsuccessfully delete\n");
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


Supplier::~Supplier()
{

}