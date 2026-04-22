#include "Product.h"

#include <iostream>
#include <conio.h>
#include <cstdlib>
#include <iomanip>
#include <string>
#include <vector>
#include <algorithm>
#include <sstream>
#include <iomanip>
using namespace std;


Product::Product()
{
	restockID = "";
	dispatchID = "";
	productID = "";
	productName = "";
	productCategory = "";
	productBrand = "";
	productPrice = "";
	productStock = "";
	productRemark = "";
	supplierID = ""; 
	staffID = "";
	quantitySupplied = "";
	quantityDispatch = "";
	salesDispatch = "";


	Menu.menuWidth = 123;
	Menu.outside_left_padding = 20;
	error.ErrorMenu.menuWidth = 123;
	error.ErrorMenu.outside_left_padding = 20;
}

void Product::viewProduct() {

	cout << Menu.FormattedRow();
	cout << Menu.formatMenu("Product Menu");

	cout << endl << Menu.FormattedRow() << Menu.FormattedRow();

	//Format and display the columns
	vector <string> displayColumns = { "No", "Product ID", "Name", "Category", "Brand", "Price (RM)", "Stock" };
	vector <int> displayColumnsSize = { 3, 12, 30, 17, 30, 15, 10 };
	

	vector<vector<string>> result = {};
	db.column_list = "ProductID, ProductName, ProductCategory, ProductBrand, ProductPrice, ProductStock";
	db.table_name = "product";
	db.condition = "ProductID LIKE '%' ";
	db.select();
	db.result(result);


	//Make all the text capital letters
	Menu.capitalizedVectorVectorText(result);

	//Display Information
	Menu.displayVectorWrapText(displayColumns, displayColumnsSize, result);

}

bool Product::addProduct() {

	viewProduct();
	vector <string> promptLine = { 
		"Product Name : " ,
		"Category (1-4) : ",
		"Brand : ", 
		"Price (RM) : ",
		string(50, ' ')
	};

	vector <string> Category = {
		"Writing Instruments" ,
		"Paper Products",
		"Organizational Supplies",
		"Other",
		string(50, ' ')
	};


	for (int i = 0; i < Category.size(); i++)
	{
		Category[i] = to_string(i + 1) + ") " + Category[i];
	}

	Menu.formatVectorInput(promptLine);
	Menu.formatVectorInput(Category);



	cout << Menu.formatInput("Please follow the instruction to fill in. \n");

	// Enter Product Name
	bool inputError = true;
	while (inputError == true)
	{
		cout << promptLine[0];
		getline(cin, productName);

		error.data = productName;

		if (error.checkEmpty())
		{
			continue;
		}
		inputError = false;
	}

	cout << endl;
	for (int i = 0; i < Category.size() - 1; i++)
	{
		cout << Category[i] << endl;
	}

	string index = "0";
	// Enter Category
	inputError = true;
	while (inputError == true)
	{
		cout << promptLine[1];
		cin >> index;

		error.data = index;

		if (error.checkSpace() || error.checkDigit())
		{
			continue;
		}

		if (stoi(index) < 1 || stoi(index) > Category.size() -1)
		{
			cout << Menu.formatInput("Follow the instruction! Enter again.\n");
			continue;
		}
		inputError = false;
	}


	int realIndex = stoi(index) - 1; 
	if (realIndex >= 0 && realIndex < Category.size())
	{ 
		// Adjust the starting position to skip the index prefix (including space) 
		size_t pos = Category[realIndex].find(')') + 2;
		productCategory = Category[realIndex].substr(pos);
	}


	cin.ignore();

	// Enter Brand
	inputError = true;
	while (inputError == true)
	{
		cout << promptLine[2];
		getline(cin, productBrand);

		for (char& c : productBrand)
		{
			c = toupper(c);
		}

		error.data = productBrand;

		if (error.checkEmpty())
		{
			continue;
		}

		inputError = false;
	}

	// Enter Price
	inputError = true;
	while (inputError == true)
	{
		cout << promptLine[3];
		cin >> productPrice;


		error.data = productPrice;

		if (error.checkSpace() || error.checkDouble())
		{
			continue;
		}

		 // Check for up to 2 decimal places
		 size_t dotPos = productPrice.find('.');
		 if (dotPos != string::npos) {
			 size_t decimalPlaces = productPrice.length() - dotPos - 1;
			 if (decimalPlaces > 2) {
				 cout << Menu.formatInput("The decimal places cannot be more than 2.\n");
				 continue; // More than 2 decimal places
			 }
		 }


		 // Convert modified price back to string
		 ostringstream oss;
		 oss << fixed << setprecision(2) << error.data;
		 productPrice = oss.str();

		 inputError = false;
	}

	//The stock by default is 0 when the product is just added


	//Ask Confirmation
	bool temp = true;
	string confirmation = {};
	while (temp == true)
	{
		cout << Menu.formatInput("Confirm to add product? (Y/N) : ");

		cin >> confirmation;
		for (char& c : confirmation)
		{
			c = toupper(c);
		}
		error.data = confirmation;

		temp = (error.checkOption("Y", "N") || error.checkSpace());
	}

	//Add product to database
	if (confirmation == "Y")
	{
		db.table_name = "product_backup";
		db.column_list = "productID";
		db.condition = "productID = (SELECT MAX(productID) FROM product_backup)";
		db.select();
		db.row = mysql_fetch_row(db.res);

		//Set supplier ID
		if (db.row == nullptr)
			productID = "P001";
		else
		{
			productID = db.row[0];
			if (!productID.empty() && productID[0] == 'P') {
				productID.erase(0, 1); // Remove character P at index 0 
			}

			int num = stoi(productID);
			num += 1;

			productID = to_string(num);
			while (productID.length() < 3)
			{
				productID = "0" + productID;
			}
			productID = "P" + productID;
		}

		db.table_name = "product";
		db.column_list = "";
		db.values_list = "'" + productID + "' , '" + productName + "' , '" + productCategory + "' , '" + productBrand + "' , '"
			+ productPrice + "' , DEFAULT , ''" ;
		
	

		if (!db.add())
		{
			vector <string> productPrompt = {
						"The addition is successful. ",
						"The new product will have product ID " + productID + ". \n"
			};
			Menu.formatVectorInput(productPrompt);

			cout << endl;
			for (int i = 0; i < productPrompt.size(); i++)
			{
				cout << productPrompt[i];
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

bool Product::updateProduct() {

	viewProduct();

	db.values_list = "";

	vector <string> promptLine = {
		"Product ID (Press X to exit) : " ,
		"Product Name : " ,
		"Category (1-4) : ",
		"Brand : ",
		"Price (RM) : ",
		"Stock (X to not change) : ",
		string(50,' ')
	};

	vector <string> Category = {
		"Writing Instruments" ,
		"Paper Products",
		"Organizational Supplies",
		"Other",
		string(50, ' ')
	};


	for (int i = 0; i < Category.size(); i++)
	{
		Category[i] = to_string(i + 1) + ") " + Category[i];
	}

	Menu.formatVectorInput(promptLine);
	Menu.formatVectorInput(Category);


	// Enter productID
	bool exist = false;
	while (!exist)
	{
		cout << promptLine[0];
		cin >> productID;

		for (char& c : productID)
		{
			c = toupper(c);
		}

		if (productID != "X")
		{

			error.data = productID;

			if (error.checkSpace())
			{
				continue;
			}


			db.column_list = "productID";
			db.table_name = "product";
			db.condition = "productID = '" + productID + "'";

			if (db.select() == true)
			{
				cout << Menu.formatInput("Product ID " + productID + " exists in the database.\n");

				exist = true;
			}

			else
			{
				cout << Menu.formatInput("Product ID " + productID + " does not exists in the database. \n");

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


	//If the user does not want to exit
	if (productID != "X")
	{

		cout << Menu.formatInput("Please follow the instruction to fill in, 0 to not change\n ");
		

		// Enter Product Name
		bool inputError = true;
		while (inputError == true)
		{
			cout << promptLine[1];
			getline(cin, productName);

			for (char& c : productName)
			{
				c = toupper(c);
			}


			error.data = productName;

			if (error.checkEmpty())
			{
				continue;
			}

			if (productName != "0")
			{
				if (db.values_list == "")
					db.values_list += "productName = '" + productName + "' ";
			}

			inputError = false;
		}



		// Enter Category
		cout << endl;
		for (int i = 0; i < Category.size() - 1; i++)
		{
			cout << Category[i] << endl;
		}

		string index = "1";
		inputError = true;
		while (inputError == true)
		{

			cout << promptLine[2];
			cin >> index;

			for (char& c : index)
			{
				c = toupper(c);
			}

			if (index != "0")
			{

				error.data = index;

				if (error.checkSpace() || error.checkDigit())
				{
					continue;
				}

				if (stoi(index) < 1 || stoi(index) > Category.size() - 1)
				{
					cout << Menu.formatInput("Follow the instruction! Enter again.\n");
					continue;
				}

				int realIndex = stoi(index) - 1;
				if (realIndex >= 0 && realIndex < Category.size())
				{
					// Adjust the starting position to skip the index prefix (including space) 
					size_t pos = Category[realIndex].find(')') + 2;
					productCategory = Category[realIndex].substr(pos);
				}

				if (db.values_list == "")
					db.values_list += "productCategory = '" + productCategory + "' ";
				else
					db.values_list += ", productCategory = '" + productCategory + "' ";


				inputError = false;
			}

			else
			{
				inputError = false;
			}
		}
		cin.ignore();

		// Enter Brand
		inputError = true;
		while (inputError == true)
		{
			cout << promptLine[3];
			getline(cin, productBrand);

			for (char& c : productBrand)
			{
				c = toupper(c);
			}


			error.data = productBrand;

			if (error.checkEmpty())
			{
				continue;
			}

			if (productBrand != "0")
			{
				if (db.values_list == "")
					db.values_list += "productBrand = '" + productBrand + "' ";
				else
					db.values_list += ", productBrand = '" + productBrand + "' ";
			}

			inputError = false;
		}

		// Enter Price
		inputError = true;
		while (inputError == true)
		{
			cout << promptLine[4];
			cin >> productPrice;

			for (char& c : productPrice)
			{
				c = toupper(c);
			}

			if (productPrice != "0")
			{
				error.data = productPrice;

				if (error.checkSpace() || error.checkDouble())
				{
					continue;
				}

				if (stod(productPrice) <= 0)
				{
					cout << Menu.formatInput("Please do not enter negative number or zero. Please enter again.\n");
					continue;
				}


				// Check for up to 2 decimal places
				size_t dotPos = productPrice.find('.');
				if (dotPos != string::npos) {
					size_t decimalPlaces = productPrice.length() - dotPos - 1;
					if (decimalPlaces > 2) {
						cout << Menu.formatInput("The decimal places cannot be more than 2.\n");
						continue; // More than 2 decimal places
					}
				}


				// Convert modified price back to string
				ostringstream oss;
				oss << fixed << setprecision(2) << error.data;
				productPrice = oss.str();

			


				if (db.values_list == "")
					db.values_list += "productPrice = '" + productPrice + "' ";
				else
					db.values_list += ", productPrice = '" + productPrice + "' ";


				inputError = false;
			}
			else
			{
				inputError = false;
			}

			inputError = false;
		}


		// Enter Stock
		inputError = true;
		while (inputError == true)
		{
			cout << promptLine[5];
			cin >> productStock;

			for (char& c : productStock)
			{
				c = toupper(c);
			}

			if (productStock != "X")
			{
				error.data = productStock;


				if (stod(productStock) < 0)
				{
					cout << Menu.formatInput("Please do not enter negative number. Please enter again.\n");
					continue;
				}

				if (error.checkSpace() || error.checkDigit())
				{
					continue;
				}


				if (db.values_list == "")
					db.values_list += "productStock = '" + productStock + "' ";
				else
					db.values_list += ", productStock = '" + productStock + "' ";


				inputError = false;
			}
			else
			{
				inputError = false;
			}

		}


		bool temp;
		string confirmation;

		//Update product information
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
				db.table_name = "product";
				db.condition = "productID = '" + productID + "'";

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

bool Product::deleteProduct() {
	viewProduct();


	bool exist = false;

	// Enter productID
	while (!exist)
	{
		cout << Menu.formatInput("Product ID (Press X to exit) : ");
		cin >> productID;

		for (char& c : productID)
		{
			c = toupper(c);
		}


		if (productID != "X")
		{

			error.data = productID;

			if (error.checkSpace())
			{
				continue;
			}


			db.column_list = "productID";
			db.table_name = "product";
			db.condition = "productID = '" + productID + "'";

			if (db.select() == true)
			{

				{
					cout << Menu.formatInput("Product ID " + productID + " exists in the database.\n");
					exist = true;
				}
			}

			else
			{
				cout << Menu.formatInput("Product ID " + productID + " does not exists in the database. Please enter again.\n");

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

	if (productID != "X")
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
			db.table_name = "product";
			db.condition = "productID = '" + productID + "'";

			if (!db.Delete())
			{
				cout << Menu.formatInput("The product with product ID " + productID + " is successfuly deleted\n");


				//Delete the supplier ID also from the product_backup if it is not found from the record_restock and record_dispatch table
				db.column_list = "productID";
				db.table_name = "record_restock";
				db.condition = "productID = '" + productID + "'";


				if (db.select() == false)
				{
					db.column_list = "productID";
					db.table_name = "record_dispatch";
					db.condition = "productID = '" + productID + "'";


					if (db.select() == false)
					{
						db.table_name = "product_backup";
						db.condition = "productID = '" + productID + "'";
						db.Delete();
					}
				}


			}
			else
			{
				cout << Menu.formatInput("The product with product ID " + productID + " is unsuccessfully delete\n");
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



void Product::viewProductRemarks() {

	cout << Menu.FormattedRow();
	cout << Menu.formatMenu("Product Remark Menu");

	cout << endl << Menu.FormattedRow() << Menu.FormattedRow();

	//Format and display the columns
	vector <string> displayColumns = { "No", "Product ID", "Name", "Stock" , "Remarks" };
	vector <int> displayColumnsSize = { 3, 12, 30, 10, 64};
	


	vector<vector<string>> result = {};
	db.column_list = "ProductID, ProductName, ProductStock, ProductRemark";
	db.table_name = "product";
	db.condition = "ProductID LIKE '%' ";
	db.select();
	db.result(result);


	//Make all the text capital letters
	Menu.capitalizedVectorVectorText(result);

	//Display Information
	Menu.displayVectorWrapText(displayColumns, displayColumnsSize, result);
}

bool Product::addProductRemarks() {

	viewProductRemarks();
	bool temp = true;

	vector <string> promptLine = {
		"Product ID (Press X to exit) : ",
		"Product Remarks : ",
		string(110,' ')
	};



	Menu.formatVectorInput(promptLine);
	
	// Enter productID
	bool exist = false;
	while (!exist)
	{
		cout << promptLine[0];
		cin >> productID;

		for (char& c : productID)
		{
			c = toupper(c);
		}

		if (productID != "X")
		{

			error.data = productID;

			if (error.checkSpace())
			{
				continue;
			}


			db.column_list = "productID";
			db.table_name = "product";
			db.condition = "productID = '" + productID + "'";

			if (db.select() == true)
			{
				cout << Menu.formatInput("Product ID " + productID + " exists in the database.\n");

				exist = true;
			}

			else
			{
				cout << Menu.formatInput("Product ID " + productID + " does not exists in the database.\n");

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
					cout << Menu.formatInput("The addition of product remark has been canceled.");
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



	if (productID != "X")
	{

		vector<vector<string>> result = {};
		db.column_list = "productRemark";
		db.table_name = "product";
		db.condition = "productID = '" + productID + "' ";
		db.select();
		db.result(result);

		// Enter Product Remarks if no information available 
		if (result[0][0] != "")
		{
			cout << Menu.formatInput("The Product Remark contained text, please go to Update Product Remark page if you wish to change it\n");
		}
		else
		{
			bool inputError = true;
			while (inputError == true)
			{
				cout << Menu.formatInput("Please write product remarks.\n");
				cout << promptLine[1];
				getline(cin, productRemark);


				error.data = productRemark;

				if (error.checkEmpty())
				{
					continue;
				}
				inputError = false;
			}
			db.values_list = "ProductRemark = '" + productRemark + "' ";

			//Ask Confirmation
			string confirmation = {};
			while (temp == true)
			{
				cout << Menu.formatInput("Confirm to add product remark? (Y/N) : ");

				cin >> confirmation;
				for (char& c : confirmation)
				{
					c = toupper(c);
				}
				error.data = confirmation;

				temp = (error.checkOption("Y", "N") || error.checkSpace());
			}

			//Add product remark to database
			if (confirmation == "Y")
			{
				db.table_name = "product";
				db.condition = "productID = '" + productID + "'";

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
				cout << Menu.formatInput("The addition of product remark is canceled.\n");
			}
		}



		//Ask repeat again or not
		temp = true;
		string again;
		while (temp == true)
		{
			cout << Menu.formatInput("Do you want to add for other? (Y/N) : ");

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
}

bool Product::updateProductRemarks() {

	viewProductRemarks();
	bool temp = true;

	vector <string> promptLine = {
		"Product ID (Press X to exit) : ",
		"Product Remarks : ",
		string(110,' ')
	};



	Menu.formatVectorInput(promptLine);


	// Enter productID
	bool exist = false;
	while (!exist)
	{
		cout << promptLine[0];
		cin >> productID;

		for (char& c : productID)
		{
			c = toupper(c);
		}

		if (productID != "X")
		{
			error.data = productID;

			if (error.checkSpace())
			{
				continue;
			}


			db.column_list = "productID";
			db.table_name = "product";
			db.condition = "productID = '" + productID + "'";

			if (db.select() == true)
			{
				cout << Menu.formatInput("Product ID " + productID + " exists in the database.\n");

				exist = true;
			}

			else
			{
				cout << Menu.formatInput("Product ID " + productID + " does not exists in the database.\n");

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
					cout << Menu.formatInput("The update of product remark has been canceled.");
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

	if (productID == "X")
	{
		return false;
	}


	
	vector<vector<string>> result = {};
	db.column_list = "productRemark";
	db.table_name = "product";
	db.condition = "productID = '" + productID + "' ";
	db.select();
	db.result(result);

	// Enter Product Remarks if no information available 
	if (result[0][0] == "")
	{
		cout << Menu.formatInput("The Product Remark do not contained text.");
		cout << Menu.formatInput("Please go to Add Product Remark page if you wish to change it\n");
	}

	else
	{
		bool inputError = true;
		while (inputError == true)
		{
			cout << promptLine[1];
			getline(cin, productRemark);

			error.data = productRemark;

			if (error.checkEmpty())
			{
				continue;
			}
			inputError = false;
		}
		db.values_list = "ProductRemark = '" + productRemark + "' ";

		//Ask Confirmation
		string confirmation = {};
		while (temp == true)
		{
			cout << Menu.formatInput("Confirm to update product remark? (Y/N) : ");

			cin >> confirmation;
			for (char& c : confirmation)
			{
				c = toupper(c);
			}
			error.data = confirmation;

			temp = (error.checkOption("Y", "N") || error.checkSpace());
		}

		//Update product remark to database
		if (confirmation == "Y")
		{
			db.table_name = "product";
			db.condition = "productID = '" + productID + "'";

			if (!db.update())
			{
				cout << Menu.formatInput("The data updated\n");
			}

			else
			{
				cout << Menu.formatInput("The data is not updated\n");
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
			cout << Menu.formatInput("The update of product remark has been canceled.\n");
		}
	}



	//Ask repeat again or not
	temp = true;
	string again;
	while (temp == true)
	{
		cout << Menu.formatInput("Do you want to update for other? (Y/N) : ");

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

bool Product::deleteProductRemarks() {

	viewProductRemarks();
	bool temp = true;

	vector <string> promptLine = {
		"Product ID (Press X to exit) : ",
		"Product Remarks : ",
		string(110,' ')
	};



	Menu.formatVectorInput(promptLine);

	// Enter productID
	bool exist = false;
	while (!exist)
	{
		cout << promptLine[0];
		cin >> productID;

		for (char& c : productID)
		{
			c = toupper(c);
		}


		if (productID != "X")
		{

			error.data = productID;

			if (error.checkSpace())
			{
				continue;
			}


			db.column_list = "productID";
			db.table_name = "product";
			db.condition = "productID = '" + productID + "'";

			if (db.select() == true)
			{
				cout << Menu.formatInput("Product ID " + productID + " exists in the database.\n");

				exist = true;
			}

			else
			{
				cout << Menu.formatInput("Product ID " + productID + " does not exists in the database.\n");

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
					cout << Menu.formatInput("The addition of product remark has been canceled.");
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

	

	if (productID != "X")
	{
		vector<vector<string>> result = {};
		db.column_list = "productRemark";
		db.table_name = "product";
		db.condition = "productID = '" + productID + "' ";
		db.select();
		db.result(result);

		// Enter Product Remarks if information available 
		if (result[0][0] == "")
		{
			cout << Menu.formatInput("The Product Remark is empty.");
		}

		else
		{
			
			//Ask Confirmation
			string confirmation = {};
			while (temp == true)
			{
				cout << Menu.formatInput("Confirm to delete product remark? (Y/N) : ");

				cin >> confirmation;
				for (char& c : confirmation)
				{
					c = toupper(c);
				}
				error.data = confirmation;

				temp = (error.checkOption("Y", "N") || error.checkSpace());
			}

			//Delete product remark to database
			if (confirmation == "Y")
			{
				db.values_list = "ProductRemark = ''";
				db.table_name = "product";
				db.condition = "productID = '" + productID + "'";

				if (!db.update())
				{
					cout << Menu.formatInput("The data deleted\n");
				}

				else
				{
					cout << Menu.formatInput("The data is not deleted\n");
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
				cout << Menu.formatInput("The deletion has been canceled.\n");
			}
		}



		//Ask repeat again or not
		temp = true;
		string again;
		while (temp == true)
		{
			cout << Menu.formatInput("Do you want to delete for other? (Y/N) : ");

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
}


string Product::staffProductPrompt()
{
	viewProduct();
	cout << Menu.FormattedRow() << Menu.emptyRow() << Menu.formatMenu("OPTION") << endl;

	string choice = "1";
	vector <string> OptionsList = { "Restock",
		"Dispatch",
		"Product Remarks",
		"Exit"
	};
	int maxLength = 0,
		outside_left_padding = 20 , 
		inside_left_padding = 0,
		inside_right_padding = 0;

	for (int i = 0; i < OptionsList.size(); i++)
	{
		OptionsList[i] = to_string(i + 1) + ") " + OptionsList[i];
	}

	for (const string& temp : OptionsList) {
		maxLength = max(maxLength, (int)temp.length());
	}

	for (int i = 0; i < OptionsList.size(); i++)
	{
		if (OptionsList[i].length() == maxLength)
		{
			inside_left_padding = (Menu.menuWidth - maxLength) / 2;
			inside_right_padding = Menu.menuWidth - maxLength - inside_left_padding;
		}
		else if (OptionsList[i].length() < maxLength)
		{
			inside_left_padding = (Menu.menuWidth - maxLength) / 2;
			inside_right_padding = Menu.menuWidth - OptionsList[i].length() - inside_left_padding;
		}

		cout << string(outside_left_padding, ' ') + "|" + string(inside_left_padding, ' ') + OptionsList[i] + string(inside_right_padding, ' ') + "|";
		cout << endl;
	}

	cout << Menu.FormattedRow() << Menu.FormattedRow();
	



	Menu.footer = Menu.formatInput(Menu.footer);

	cout << Menu.footer;
	
	cin >> choice;
	while (!(all_of(choice.begin(), choice.end(), isdigit)) || stoi(choice) < 1 || stoi(choice) > OptionsList.size() || cin.peek() == ' ')
	{
		cout << Menu.formatInput("Follow the instruction!!!");
		cout << Menu.formatInput("Re-enter your choice : ");
		cin >> choice;
	}


	return choice;
}


void Product::productSupplier() {

	

	//Format and display the columns
	vector <string> displayColumns = { "No", "Product ID", "Product Name", "Stock"," ", "Supplier ID", "Supplier Name" };
	vector <int> displayColumnsSize = { 3, 12, 30, 10, 9, 13, 40 };
	

	vector <string> productScript = { "ProductID", "ProductName", "ProductStock" };
	vector <string> supplierScript = { "SupplierID", "SupplierName" };

	vector<vector<string>> productResult = {};
	db.column_list = "ProductID, ProductName, ProductStock";
	db.table_name = "product";
	db.condition = "ProductID LIKE '%' ";
	db.select();
	db.result(productResult);


	vector<vector<string>> supplierResult = {};
	db.column_list = "SupplierID, SupplierName";
	db.table_name = "Supplier";
	db.condition = "SupplierID LIKE '%' ";
	db.select();
	db.result(supplierResult);





	//Determine the rowNum and columnNum of the table display 
	int rowNum = 0, columnNum = 0 ;
	rowNum = max(productResult.size(), supplierResult.size());
	columnNum = productResult[0].size()+ supplierResult[0].size() + 1;

	vector<vector<string>> result(rowNum, vector<string>(columnNum, " "));




	int rowProduct = productResult.size(),
		columnProduct = productResult[0].size(),
		rowSupplier = supplierResult.size(),
		columnSupplier = supplierResult[0].size();


	//Insert the columns for the combination of the product and supplier
	for (int currentRow = 0; currentRow < rowNum; currentRow++)
	{
		//Insert the empty columns for the empty row of the product
		if (currentRow >= rowProduct)
		{
			for (int currentColumn = 0; currentColumn < columnNum; currentColumn++)
			{

				if (currentColumn > columnProduct)
					result[currentRow][currentColumn] = supplierResult[currentRow][currentColumn - 4];
				else
					result[currentRow][currentColumn] = " ";
				
			}
		}
		//Insert the empty columns for the empty row of the supplier
		else if (currentRow >= rowSupplier)
		{
			for (int currentColumn = 0; currentColumn < columnNum; currentColumn++)
			{
				if (currentColumn >= columnProduct)
					result[currentRow][currentColumn] = " ";
				else
					result[currentRow][currentColumn] = productResult[currentRow][currentColumn];

			}
		}

		//Insert the columns for the combination of the product and supplier
		else
		{
			for (int currentColumn = 0; currentColumn < columnNum; currentColumn++)
			{
				if (currentColumn > columnProduct)
				{
					
					result[currentRow][currentColumn] = supplierResult[currentRow][currentColumn - 4];
				}
				//Insert the empty column to separate the product and the supplier 
				else if (currentColumn == columnProduct)
				{
					result[currentRow][currentColumn] = " ";
				}
				else
				{
					result[currentRow][currentColumn] = productResult[currentRow][currentColumn];
				}

			}
		}

	}



	

	cout << Menu.FormattedRow();
	cout << Menu.formatMenu("Product Menu" + string(20, ' ') + '|' + string (displayColumnsSize[4],' ' ) + '|' + string(15, ' ') 
		+ "Supplier Menu");

	cout << endl << Menu.FormattedRow() << Menu.FormattedRow();



	//Make all the text capital letters
	Menu.capitalizedVectorVectorText(result);

	//Display Information
	Menu.displayVectorWrapText(displayColumns, displayColumnsSize, result);

	

}


bool Product::restock()
{
	productSupplier();


	bool temp = true;

	vector <string> promptLine = {
		"Product ID (Press X to exit) : ",
		"Supplier ID (Press X to exit) : ",
		"Quantity Supplied : "
	};



	Menu.formatVectorInput(promptLine);

	// Enter productID
	bool exist = false;
	while (!exist)
	{
		cout << promptLine[0];
		cin >> productID;

		for (char& c : productID)
		{
			c = toupper(c);
		}

		if (productID != "X")
		{

			error.data = productID;

			if (error.checkSpace())
			{
				continue;
			}


			db.column_list = "productID";
			db.table_name = "product";
			db.condition = "productID = '" + productID + "'";

			if (db.select() == true)
			{
				cout << Menu.formatInput("Product ID " + productID + " exists in the database.\n");

				exist = true;
			}

			else
			{
				cout << Menu.formatInput("Product ID " + productID + " does not exists in the database.\n");

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
					exist = false;
				}
				else if (again == "N")
				{
					cout << Menu.formatInput("The restock has been canceled.");
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

	if (productID == "X")
	{
		return false;
	}	

	// Enter supplierID
	exist = false;
	while (!exist)
	{
		cout << promptLine[1];
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
				cout << Menu.formatInput("Supplier ID " + supplierID + " does not exists in the database.\n");

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
					exist = false;
				}
				else if (again == "N")
				{
					cout << Menu.formatInput("The restock has been canceled.");
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

	if (supplierID == "X")
	{
		return false;
	}

	// Enter quantitySupplied
	bool inputError = true;
	while (inputError == true)
	{
		cout << promptLine[2];
		cin >> quantitySupplied;

		
		error.data = quantitySupplied;

		if (error.checkSpace() || error.checkDigit())
		{
			continue;
		}

		if (stoi(quantitySupplied) <= 0)
		{
			cout << Menu.formatInput("Please do not enter negative number or zero. Please enter again.\n");
			continue;
		}

		

		inputError = false;
	}


	//Ask Confirmation
	temp = true;
	string confirmation = {};
	while (temp == true)
	{
		cout << Menu.formatInput("Confirm to restock? (Y/N) : ");

		cin >> confirmation;
		for (char& c : confirmation)
		{
			c = toupper(c);
		}
		error.data = confirmation;

		temp = (error.checkOption("Y", "N") || error.checkSpace());
	}



	//Add restock record to database
	if (confirmation == "Y")
	{
		db.table_name = "record_restock";
		db.column_list = "restockID";
		db.condition = "restockID = (SELECT MAX(restockID) FROM record_restock)";
		db.select();
		db.row = mysql_fetch_row(db.res);

		//Set restockID
		if (db.row == nullptr)
			restockID = "B00001";
		else
		{
			restockID = db.row[0];
			if (!restockID.empty() && restockID[0] == 'B') {
				restockID.erase(0, 1); // Remove character B at index 0 
			}

			int num = stoi(restockID);
			num += 1;

			restockID = to_string(num);
			while (restockID.length() < 5)
			{
				restockID = "0" + restockID;
			}
			restockID = "B" + restockID;
		}


		//Check the product stock and product name
		db.table_name = "product";
		db.column_list = "productStock, productName";
		db.condition = "productID = '" + productID + "'";

		


		if (db.select() == true)
		{
			db.row = mysql_fetch_row(db.res);



			//Set the product name
			productName = db.row[1];



			int num = atoi(db.row[0]) + stoi(quantitySupplied);
			productStock = to_string(num);


			//Update the product stock 
			db.table_name = "product";
			db.values_list = "productStock = '" + productStock + "'";
			db.condition = "productID = '" + productID + "'";


			if (!db.update()) 
			{

				//Insert the record_restock
				db.table_name = "record_restock";
				db.column_list = "";
				db.values_list = "'" + restockID + "' , DEFAULT, '" + quantitySupplied + "', '" + productID + "', '"
					+ supplierID + "',  '" + staffID + "'";

				if (!db.add())
				{


					vector <string> receiptColumnName = { "Restock ID", "Product ID", "Product Name", "Supplier ID" };
					vector <string> receiptContent = { restockID, productID, productName, supplierID };
					string Title = "Restock Receipt ";

					receipt(receiptColumnName, receiptContent, Title);


					cout << Menu.formatInput("The restock with ID "+ restockID +" is successful.\n");

				}
				else
				{
					cout << Menu.formatInput("The restock is unsuccessful, please try again\n");
				}
			}
			else
			{
				cout << Menu.formatInput("The restock is unsuccessful, please try again\n");
			}
		}

		else
		{
			cout << Menu.formatInput("The restock is unsuccessful, please try again\n");
		}
		
	}

	else if (confirmation == "N")
	{
		cout << Menu.formatInput("The restock is canceled.\n");
	}

	

	cout << Menu.formatInput("Press any key to restock again or press X to exit. ");
	int again = _getch();
	
	//If the user press x or X, exit
	if (again == 88 || again == 120)
	{
		
		return false;
	}
	else
	{
		return true;
	}

}

bool Product::dispatch()
{
	viewProduct();


	bool temp = true;

	vector <string> promptLine = {
		"Product ID (Press X to exit) : ",
		"Quantity of dispatch : "
	};



	Menu.formatVectorInput(promptLine);

	// Enter productID
	bool exist = false;
	while (!exist)
	{
		cout << promptLine[0];
		cin >> productID;

		for (char& c : productID)
		{
			c = toupper(c);
		}

		if (productID != "X")
		{

			error.data = productID;

			if (error.checkSpace())
			{
				continue;
			}


			db.column_list = "productID";
			db.table_name = "product";
			db.condition = "productID = '" + productID + "'";

			if (db.select() == true)
			{
				cout << Menu.formatInput("Product ID " + productID + " exists in the database.\n");

				exist = true;
			}

			else
			{
				cout << Menu.formatInput("Product ID " + productID + " does not exists in the database.\n");

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
					cout << Menu.formatInput("The dispatch has been canceled.");
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

	if (productID == "X")
	{
		return false;
	}

	
	//Check the product stock 
	db.table_name = "product";
	db.column_list = "productStock, productPrice, productName";
	db.condition = "productID = '" + productID + "'";
	db.select();
	db.row = mysql_fetch_row(db.res);

	productStock = db.row[0];
	productName = db.row[2];




	// Enter quantityDispatch
	bool inputError = true;
	while (inputError == true)
	{
		cout << promptLine[1];
		cin >> quantityDispatch;

		

		error.data = quantityDispatch;

		if (error.checkSpace() || error.checkDigit())
		{
			continue;
		}


		int num = stoi(productStock) - stoi(quantityDispatch);
		if (num < 0)
		{
			cout << Menu.formatInput("Insufficient stock. Please enter again.\n");
			continue;
		}

		if (stoi(quantityDispatch) <= 0)
		{
			cout << Menu.formatInput("Please do not enter negative number or zero. Please enter again.\n");
			continue;
		}

		inputError = false;
	}

	//Update the product stock 
	int num = stoi(productStock) - stoi(quantityDispatch);
	productStock = to_string(num);

	//Calculate the sales of this dispatch 
	double tempo;
	tempo = atof(db.row[1]) * stod(quantityDispatch);

	//Format the sales to two decimal places
	ostringstream oss;
	oss << fixed << setprecision(2) << tempo;
	salesDispatch = oss.str();




	//Ask Confirmation
	temp = true;
	string confirmation = {};
	while (temp == true)
	{
		cout << Menu.formatInput("Confirm to dispatch? (Y/N) : ");

		cin >> confirmation;
		for (char& c : confirmation)
		{
			c = toupper(c);
		}
		error.data = confirmation;

		temp = (error.checkOption("Y", "N") || error.checkSpace());
	}



	//Add dispatch record to database
	if (confirmation == "Y")
	{
		
		//Set restockID
		db.table_name = "record_dispatch";
		db.column_list = "dispatchID";
		db.condition = "dispatchID = (SELECT MAX(dispatchID) FROM record_dispatch)";
		db.select();
		db.row = mysql_fetch_row(db.res);

		
		if (db.row == nullptr)
			dispatchID = "D00001";
		else
		{
			dispatchID = db.row[0];
			if (!dispatchID.empty() && dispatchID[0] == 'D') {
				dispatchID.erase(0, 1); // Remove character D at index 0 
			}

			int num = stoi(dispatchID);
			num += 1;

			dispatchID = to_string(num);
			while (dispatchID.length() < 5)
			{
				dispatchID = "0" + dispatchID;
			}
			dispatchID = "D" + dispatchID;
		}



		
		
		
		db.table_name = "product";
		db.values_list = "productStock = '" + productStock + "'";
		db.condition = "productID = '" + productID + "'";


		if (!db.update())
		{
			//Insert the record_outstock in the database
			db.table_name = "record_dispatch";
			db.column_list = "";
			db.values_list = "'" + dispatchID + "' , DEFAULT, '" + quantityDispatch + "', '" + salesDispatch + "', '" + staffID + "',  '" + productID + "'";






			if (!db.add())
			{

				vector <string> receiptColumnName = { "Dispatch ID", "Product ID", "Product Name", "Sales (RM)" };
				vector <string> receiptContent = { dispatchID, productID, productName, salesDispatch };
				string Title = "Dispatch Receipt";

				receipt(receiptColumnName, receiptContent, Title);

				cout << Menu.formatInput("The dispatch with ID " + dispatchID + " is successful.\n");

			}
			else
			{
				cout << Menu.formatInput("The dispatch is unsuccessful, please try again\n");
			}
		}
		else
		{
			cout << Menu.formatInput("The dispatch is unsuccessful, please try again\n");
		}
	}

	else if (confirmation == "N")
	{
		cout << Menu.formatInput("The dispatch is canceled.\n");
	}



	cout << Menu.formatInput("Press any key to dispatch again or press X to exit. ");
	int again = _getch();

	//If the user press x or X, exit
	if (again == 88 || again == 120)
	{

		return false;
	}
	else
	{
		return true;
	}

}

void Product::receipt(vector <string> receiptColumnName, vector <string> receiptContent, string Title)
{
	int width = 10,
		receiptMenuWidth = 50,
		outside_left_padding = Menu.outside_left_padding + Menu.menuWidth / 3.5 ,
		inside_left_padding = 0,
		inside_right_padding = 0,
		maxLength = 10;

	string emptyLine = string(outside_left_padding, ' ') + '|' + string(receiptMenuWidth / 2, '-') + '|' + string(receiptMenuWidth / 2, '-') + '|';





	cout << endl << emptyLine << endl;
	cout << string(outside_left_padding, ' ') + '|' + string((receiptMenuWidth - Title.size()) / 2 , ' ')
		+ Title + string((receiptMenuWidth - Title.size()) / 2 + 1, ' ') + '|' << endl;

	cout << emptyLine << endl;

	for (const string& temp : receiptColumnName) {
		maxLength = max(maxLength, (int)temp.length());
	}


	for (int i = 0; i < receiptColumnName.size(); i++)
	{
		if (receiptColumnName[i].length() == maxLength)
		{
			inside_left_padding = (receiptMenuWidth / 2 - maxLength) / 2;
			inside_right_padding = receiptMenuWidth / 2 - maxLength - inside_left_padding;
		}
		else if (receiptColumnName[i].length() < maxLength)
		{
			inside_left_padding = (receiptMenuWidth / 2 - maxLength) / 2;
			inside_right_padding = receiptMenuWidth / 2 - receiptColumnName[i].length() - inside_left_padding;
		}

		cout << string(outside_left_padding, ' ') + Menu.centerText(receiptColumnName[i], receiptMenuWidth / 2) + Menu.centerText(receiptContent[i], receiptMenuWidth / 2) + "|";
		cout << endl << emptyLine << endl;
	}

}


Product::~Product()
{

}