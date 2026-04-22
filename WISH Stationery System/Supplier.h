#ifndef SUPPLIER_H
#define SUPPLIER_H

#include <iostream>
#include <string>


#include "Menu.h"
#include "DbConnection.h"
#include "ErrorDetection.h"

using namespace std;

class Supplier
{
public:
	ErrorDetection error;
	database db;
	Menu Menu;

	string supplierID, supplierName, address, telephoneNumber, adminID;


	Supplier();
	void viewSupplier();
	bool addSupplier();
	bool updateSupplier();
	bool deleteSupplier();
	~Supplier();
};


#endif