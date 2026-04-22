#ifndef HISTORY_H
#define HISTORY_H

#include <iostream>
#include <string>

#include "Menu.h"
#include "DbConnection.h"
#include "ErrorDetection.h"

using namespace std;

class History
{
public:
	database db;
	Menu Menu;

	string restockID, dispatchID, productID, productName, supplierID, staffID, quantitySupplied, 
		dateSupplied, quantityDispatch, salesDispatch;


	History();
	void viewRestock();
	void viewDispatch();
	~History();
};


#endif