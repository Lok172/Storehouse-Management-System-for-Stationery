#include "History.h"

#include <iostream>
#include <string>
#include <vector>
using namespace std;


History::History()
{
	restockID = "";
	dispatchID = "";
	productID = "";
	productName = "";
	supplierID = "";
	staffID = "";
	quantitySupplied = "";
	quantityDispatch = "";
	salesDispatch = "";


	Menu.menuWidth = 120;
	Menu.outside_left_padding = 20;
}

void History::viewRestock() {

	cout << Menu.FormattedRow();
	cout << Menu.formatMenu("History Menu");

	cout << endl << Menu.FormattedRow() << Menu.FormattedRow();

	//Format and display the columns
	vector <string> displayColumns = { "No", "Restock ID", "Date Supplied", "Product ID", 
		"Product Name", "Quantity", "Supplier ID", "Staff ID" };


	vector <int> displayColumnsSize = { 3, 12, 15, 12, 38, 10, 13, 10 };
	

	

	vector<vector<string>> result = {};
	db.column_list = "RestockID, DateSupplied, ProductID , productName, QuantitySupplied, SupplierID, StaffID";
	db.table_name = "record_restock join product_backup using (ProductID)";
	db.condition = "ProductID LIKE '%' order by RestockID";
	db.select();
	db.result(result);


	//Make all the text capital letters
	Menu.capitalizedVectorVectorText(result);

	//Display Information
	Menu.displayVectorWrapText(displayColumns, displayColumnsSize, result);	

}

void History::viewDispatch() {

	cout << Menu.FormattedRow();
	cout << Menu.formatMenu("History Menu");

	cout << endl << Menu.FormattedRow() << Menu.FormattedRow();

	//Format and display the columns
	vector <string> displayColumns = { "No", "Dispatch ID", "Date Dispatch", "Product ID", "Name", "Price (RM)",
		"Quantity", "Sales (RM)", "Staff ID" };


	vector <int> displayColumnsSize = { 3, 14, 14, 12, 25, 12, 10, 12, 10 };




	vector<vector<string>> result = {};
	db.column_list = "DispatchID, DateDispatch, ProductID, productName,ProductPrice, QuantityDispatch, TotalSales, StaffID";
	db.table_name = "record_dispatch join product_backup using (ProductID)";
	db.condition = "ProductID LIKE '%' order by dispatchID";
	db.select();
	db.result(result);


	//Make all the text capital letters
	Menu.capitalizedVectorVectorText(result);

	//Display Information
	Menu.displayVectorWrapText(displayColumns, displayColumnsSize, result);

}

History::~History()
{
	
}