//Header file
#include "Menu.h"
#include "Register.h"
#include "DbConnection.h"
#include "Login.h"
#include "ErrorDetection.h"
#include "StaffManagement.h"
#include "AdminManagement.h"
#include "ErrorDetection.h"
#include "Profile.h"
#include "Product.h"
#include "Supplier.h"
#include "History.h"
#include "Report.h"



#include <iostream>
#include <string>
#include <conio.h>
#include <iomanip>

using namespace std;



//Function 
void registration();
void login();
void adminPage();
void staffPage();
void userProfile();
void staffManagement();
void adminManagement();
void productManagement();
void supplierManagement();
void staffProduct();
void productRemark();
void historyFlow();
void generateReport();

string userID ="", usertype = "";



int main()
{

	system("title  WISH Stationery System");
	
	while(1)
	{
		// Using the Menu Format
		Menu mainpage;
		mainpage.header = "    Welcome to WISH Stationery Storehouse Management System";
		mainpage.address1 = "Address: DT1, Jalan Pusat Perniagaan Durian Tunggal 2,";
		mainpage.address2 = "Durian Tunggal, 76100 Alor Gajah, Melaka";
		mainpage.CompanyPhoneNumber = "Company phone number: 06-989 1234";
		mainpage.functitle = "OPTION";
		mainpage.addOption("REGISTER");
		mainpage.addOption("LOGIN");
		mainpage.addOption("EXIT");
		string option = mainpage.prompt();
		if (option == "1")
		{
			cin.clear();
			cin.ignore(1000, '\n');
			registration();
		}
		else if (option == "2")
		{
			cin.clear();
			cin.ignore(1000, '\n');
			login();
		}
		else if (option == "3")
		{
			cin.clear();
			cin.ignore(1000, '\n');
			cout << mainpage.formatInput("Thank you for using.\n\n");
			exit(0);
		}
	}
	
}

void registration()
{
	Menu RegistrationMenu;
	Register RegistrationPage;
	bool loop = true;


	if (RegistrationMenu.accessMenu("register")  == false)
		return ; //return to last page

	else
	{
		while (loop == true)
		{
			system("cls");
			RegistrationMenu.header = "Registration Menu";
			RegistrationMenu.functitle = "Please follow the instruction to fill in";
			RegistrationMenu.prompt();

			loop = RegistrationPage.prompt();
			if (loop == true)
			{
				cin.clear();
				cin.ignore(1000, '\n');
				continue;
			}
		}
	}
}

void login()
{
	Menu LoginMenu;
	Login LoginPage;
	
	bool loop = true;

	if (LoginMenu.accessMenu("login")  == false)
	{
		return; //return to last page
		cin.clear();
		cin.ignore(1000, '\n');
	}
	else 
	{
		while (loop == true)
		{
			system("cls");
			LoginMenu.header = "Login Menu";
			LoginMenu.functitle = "Please follow the instruction to fill in";
			LoginMenu.prompt();

			loop = LoginPage.prompt();
			if (loop == true)
			{
				cin.clear();
				cin.ignore(1000, '\n');
				continue;
			}
		}

		usertype = LoginPage.usertype;
		userID = LoginPage.userID;

		if (LoginPage.loginstatus == true)
		{
			if (usertype == "admin")
			{
				adminPage();
			}
			else if (usertype == "staff")
			{
				staffPage();
			}
		}
	}

}

void adminPage()
{
	while (1)
	{
		system("cls");
		Menu Menu;
		Menu.header = "Admin Menu";

		Menu.functitle = "OPTION";
		Menu.addOption("Profile");
		Menu.addOption("Staff");
		Menu.addOption("Admin");
		Menu.addOption("Product");
		Menu.addOption("Supplier");
		Menu.addOption("History Flow of Product");
		Menu.addOption("Generate Report");
		Menu.addOption("Exit");

		string option = Menu.prompt();
		cin.clear();
		cin.ignore(1000, '\n');
		if (option == "1")
		{
			userProfile();
		}
		else if (option == "2")
		{
			staffManagement();
		}
		else if (option == "3")
		{
			adminManagement();
		}
		else if (option == "4")
		{
			productManagement();
		}
		else if (option == "5")
		{
			supplierManagement();
		}
		else if (option == "6")
		{
			historyFlow();
		}
		else if (option == "7")
		{
			generateReport();
		}
		else if (option == "8")
		{
			Login login;
			if (login.logout())
			{
				cout << Menu.formatInput("Logout successful. Press any key to exit this page...");
				_getch();
				return;
			}

			else
			{
				cout << Menu.formatInput("Logout unsuccessful. Press any key to reload this page...");
				_getch();
			}
			
		}
	}
}

void staffPage()
{
	while (1)
	{
		system("cls");
		Menu Menu;
		Menu.header = "Staff Menu";

		Menu.functitle = "OPTION";
		Menu.addOption("Profile");
		Menu.addOption("Product");
		Menu.addOption("Exit");

		string option = Menu.prompt();
		cin.clear();
		cin.ignore(1000, '\n');
		if (option == "1")
		{
			userProfile();
		}
		else if (option == "2")
		{
			staffProduct();
		}

		else if (option == "3")
		{
			Login login;
			if (login.logout())
			{
				
				cout << Menu.formatInput("Logout successful. Press any key to exit this page...");
				_getch();
				return;
			}

			else
			{
				cout << Menu.formatInput("Logout unsuccessful. Press any key to reload this page...");
				_getch();
			}

		}
	}
}

void userProfile() 
{
	while (1)
	{
		system("cls");
		Profile profile;

		profile.usertype = usertype;
		profile.userID = userID;

		Menu Menu;
		Menu.header = "Profile Menu";

		Menu.functitle = "OPTION";
		Menu.addOption("View Profile Information");
		Menu.addOption("Add Profile Information");
		Menu.addOption("Update Profile Information");
		Menu.addOption("Delete Profile Information");
		Menu.addOption("Exit");

		string option = Menu.prompt();
		cin.clear();
		cin.ignore(1000, '\n');
		if (option == "1")
		{
			system("cls");
			profile.viewProfile();
			cout << Menu.formatInput("Press any key to exit this page");
			cout << Menu.formatInput("or press P to view the password\n");
			cout << Menu.formatInput(" ");

			int temp = _getch();

			//If the user press p or P, show password
			if (temp == 80 || temp == 112)
			{
				cout << Menu.formatInput("P\n");
				profile.viewPassword();
			}
		}

		else if (option == "2")
		{
			if (Menu.accessMenu("add profile information") == false)
				continue; //Reload the page
			else
			{
				bool loop = true;
				while (loop == true)
				{
					system("cls");
					loop = profile.addInformation();
					if (loop == true)
					{
						cin.clear();
						cin.ignore(1000, '\n');
						continue;
					}
				}
			}
		}
		else if (option == "3")
		{
			if (Menu.accessMenu("update profile information") == false)
				continue; //Reload the page
			else
			{
				bool loop = true;
				while (loop == true)
				{
					system("cls");
					loop = profile.updateInformation();
					if (loop == true)
					{
						cin.clear();
						cin.ignore(1000, '\n');
						continue;
					}
				}
			}
		}
		else if (option == "4")
		{
			if (Menu.accessMenu("delete profile information") == false)
				continue; //Reload the page
			else
			{
				bool loop = true;
				while (loop == true)
				{
					system("cls");
					loop = profile.deleteInformation();
					if (loop == true)
					{
						cin.clear();
						cin.ignore(1000, '\n');
						continue;
					}
				}
			}
		}
		else if (option == "5")
		{
			return;
		}
	}

}

void staffManagement()
{
	while (1)
	{
		system("cls");
		Menu Menu;
		Staff EmployedStaff;
		Menu.header = "Employed Staff Menu";

		Menu.functitle = "OPTION";
		Menu.addOption("View Staff");
		Menu.addOption("Update Staff");
		Menu.addOption("Delete Staff");
		Menu.addOption("Exit");
		string option = Menu.prompt();
		cin.clear();
		cin.ignore(1000, '\n');
		
		
		if (option == "1")
		{
			system("cls");
			EmployedStaff.viewStaff();
			cout << Menu.formatInput("Press any key to exit this page...");
			_getch();
		}

		else if (option == "2")
		{
			
			
			bool loop = true;
			while (loop == true)
			{
				system("cls");
				loop = EmployedStaff.updateStaff();
				if (loop == true)
				{
					cin.clear();
					cin.ignore(1000, '\n');
					continue;
				}
			}
			
		}
		else if (option == "3")
		{
			
			
			bool loop = true;
			while (loop == true)
			{
				system("cls");
				loop = EmployedStaff.deleteStaff();
				if (loop == true)
				{
					cin.clear();
					cin.ignore(1000, '\n');
					continue;
				}
			}
			
		}
		else if (option == "4")
		{
			return;
		}
	}
}

void adminManagement()
{
	while (1)
	{
		system("cls");
		Menu Menu;
		Admin EmployedAdmin;
		EmployedAdmin.userID = userID;
		Menu.header = "Employed Admin Menu";

		Menu.functitle = "OPTION";
		Menu.addOption("View Admin");
		Menu.addOption("Update Admin");
		Menu.addOption("Delete Admin");
		Menu.addOption("Exit");
		string option = Menu.prompt();
		cin.clear();
		cin.ignore(1000, '\n');


		if (option == "1")
		{
			system("cls");
			EmployedAdmin.viewAdmin();
			cout << Menu.formatInput("Press any key to exit this page...");
			_getch();
		}
		else if (option == "2")
		{
			
			bool loop = true;
			while (loop == true)
			{
				system("cls");
				loop = EmployedAdmin.updateAdmin();
				if (loop == true)
				{
					cin.clear();
					cin.ignore(1000, '\n');
					continue;
				}
			}
			
		}

		else if (option == "3")
		{
			
			bool loop = true;
			while (loop == true)
			{
				system("cls");
				loop = EmployedAdmin.deleteAdmin();
				if (loop == true)
				{
					cin.clear();
					cin.ignore(1000, '\n');
					continue;
				}
					
			}
			
		}
		else if (option == "4")
		{
			return;
		}
	}
}

void productManagement()
{
	while (1)
	{
		system("cls");
		Menu Menu;
		Product product;
		Menu.header = "Product Menu";

		Menu.functitle = "OPTION";

		Menu.addOption("View Product");
		Menu.addOption("Add Product");
		Menu.addOption("Update Product");
		Menu.addOption("Delete Product");
		Menu.addOption("Product Remarks");
		Menu.addOption("Exit");
		string option = Menu.prompt();
		cin.clear();
		cin.ignore(1000, '\n');


		if (option == "1")
		{
			system("cls");
			product.viewProduct();
			cout << Menu.formatInput("Press any key to exit this page...");
			_getch();
		}
		else if (option == "2")
		{
			if (Menu.accessMenu("add product") == false)
				continue; //Reload the page
			else
			{
				bool loop = true;
				while (loop == true)
				{
					system("cls");
					loop = product.addProduct();
					if (loop == true)
					{
						cin.clear();
						cin.ignore(1000, '\n');
						continue;
					}
				}
			}
		}
		else if (option == "3")
		{

			bool loop = true;
			while (loop == true)
			{
				system("cls");
				loop = product.updateProduct();
				if (loop == true)
				{
					cin.clear();
					cin.ignore(1000, '\n');
					continue;
				}
			}

		}
		else if (option == "4")
		{

			bool loop = true;
			while (loop == true)
			{
				system("cls");
				loop = product.deleteProduct();
				if (loop == true)
				{
					cin.clear();
					cin.ignore(1000, '\n');
					continue;
				}
			}

		}
		else if (option == "5")
		{
			productRemark();
		}
		else if (option == "6")
		{
			return;
		}
	}
}

void supplierManagement()
{
	while (1)
	{
		system("cls");
		Menu Menu;
		Supplier supplier;
		supplier.adminID = userID;
		Menu.header = "Supplier Menu";

		Menu.functitle = "OPTION";
		Menu.addOption("View Supplier");
		Menu.addOption("Add Supplier");
		Menu.addOption("Update Supplier");
		Menu.addOption("Delete Supplier");
		Menu.addOption("Exit");
		string option = Menu.prompt();
		cin.clear();
		cin.ignore(1000, '\n');


		if (option == "1")
		{
			system("cls");
			supplier.viewSupplier();
			cout << Menu.formatInput("Press any key to exit this page...");
			_getch();
		}
		else if (option == "2")
		{
			if (Menu.accessMenu("add supplier") == false)
				continue; //Reload the page
			else
			{
				bool loop = true;
				while (loop == true)
				{
					system("cls");
					loop = supplier.addSupplier();
					if (loop == true)
					{
						cin.clear();
						cin.ignore(1000, '\n');
						continue;
					}
				}
			}
		}
		else if (option == "3")
		{
			
			bool loop = true;
			while (loop == true)
			{
				system("cls");
				loop = supplier.updateSupplier();
				if (loop == true)
				{
					cin.clear();
					cin.ignore(1000, '\n');
					continue;
				}
			}
			
		}
		else if (option == "4")
		{
			
			bool loop = true;
			while (loop == true)
			{
				system("cls");
				loop = supplier.deleteSupplier();
				if (loop == true)
				{
					cin.clear();
					cin.ignore(1000, '\n');
					continue;
				}
			}
			
		}
		else if (option == "5")
		{
			return;
		}
	}
}

void staffProduct()
{
	while (1)
	{
		system("cls");
		Menu Menu;
		Product product;
		product.staffID = userID;

		string option = product.staffProductPrompt();
		cin.clear();
		cin.ignore(1000, '\n');


		if (option == "1")
		{
			bool loop = true;
			while (loop == true)
			{
				system("cls");
				loop = product.restock();
				if (loop == true)
				{
					cin.clear();
					cin.ignore(1000, '\n');
					continue;
				}
			}
		}
		else if (option == "2")
		{
			bool loop = true;
			while (loop == true)
			{
				system("cls");
				loop = product.dispatch();
				if (loop == true)
				{
					cin.clear();
					cin.ignore(1000, '\n');
					continue;
				}
			}
		}
		else if (option == "3")
		{
			productRemark();
		}
		else if (option == "4")
		{
			return;
		}

	}
}

void productRemark()
{
	while (1)
	{

		system("cls");
		Menu Menu;
		Product product;
		Menu.header = "Product Remark Menu";

		Menu.functitle = "OPTION";


		Menu.addOption("View Product Remark");
		Menu.addOption("Add Product Remark");
		Menu.addOption("Update Product Remark");
		Menu.addOption("Delete Product Remark");
		Menu.addOption("Exit");
		string option = Menu.prompt();
		cin.clear();
		cin.ignore(1000, '\n');


		if (option == "1")
		{
			system("cls");
			product.viewProductRemarks();
			cout << Menu.formatInput("Press any key to exit this page...");
			_getch();
		}
		else if (option == "2")
		{
			
			
			bool loop = true;
			while (loop == true)
			{
				system("cls");
				loop = product.addProductRemarks();
				if (loop == true)
				{
					cin.clear();
					cin.ignore(1000, '\n');
					continue;
				}
			}
			
		}
		else if (option == "3")
		{
			
			bool loop = true;
			while (loop == true)
			{
				system("cls");
				loop = product.updateProductRemarks();
				if (loop == true)
				{
					cin.clear();
					cin.ignore(1000, '\n');
					continue;
				}
			}
			
		}
		else if (option == "4")
		{
			
			
			bool loop = true;
			while (loop == true)
			{
				system("cls");
				loop = product.deleteProductRemarks();
				if (loop == true)
				{
					cin.clear();
					cin.ignore(1000, '\n');
					continue;
				}
			}
			
		}
		else if (option == "5")
		{
			return;
		}

	}
}

void historyFlow()
{
	while (1)
	{
		system("cls");
		Menu Menu;
		History history;
		Menu.header = "History Menu";

		Menu.functitle = "OPTION";
		Menu.addOption("History Restock");
		Menu.addOption("History Dispatch");
		Menu.addOption("Exit");
		string option = Menu.prompt();
		cin.clear();
		cin.ignore(1000, '\n');


		if (option == "1")
		{
			system("cls");
			history.viewRestock();
			cout << Menu.formatInput("Press any key to exit this page...");
			_getch();
		}

		else if (option == "2")
		{
			system("cls");
			history.viewDispatch();
			cout << Menu.formatInput("Press any key to exit this page...");
			_getch();
		}
		
		else if (option == "3")
		{
			return;
		}
	}
}

void generateReport()
{
	while (1)
	{
		system("cls");
		Menu Menu;
		Report report;
		Menu.header = "Report Menu";

		Menu.functitle = "OPTION";
		Menu.addOption("Daily Sales");
		Menu.addOption("Monthly Sales");
		Menu.addOption("Yearly Sales");
		Menu.addOption("Exit");
		string option = Menu.prompt();
		cin.clear();
		cin.ignore(1000, '\n');

		report.option = option;

		if (option == "1")
		{
			system("cls");
			report.viewReport();
			cout << Menu.formatInput("Press any key to exit this page...");
			_getch();
		}

		else if (option == "2")
		{
			system("cls");
			report.viewReport();
			cout << Menu.formatInput("Press any key to exit this page...");
			_getch();
		}

		else if (option == "3")
		{
			
			bool loop = true;
			while (loop == true)
			{
				system("cls");
				loop = report.viewReport();

				if (loop == false)
				{
					cin.clear();
					cin.ignore(1000, '\n');
					continue;
				}


				cout << Menu.formatInput("Press any key to generate another or press X to exit. ");
				int again = _getch();

				//If the user press x or X, exit
				if (again == 88 || again == 120)
				{
					loop = false;
				}
				else
				{
					loop = true;
				}

			}


		}

		else if (option == "4")
		{
			return;
		}
	}
}
