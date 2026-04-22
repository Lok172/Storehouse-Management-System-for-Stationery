#ifndef PRODUCT_H
#define PRODUCT_H

#include <iostream>
#include <string>

#include "Menu.h"
#include "DbConnection.h"
#include "ErrorDetection.h"

using namespace std;

class Product
{
public:
	ErrorDetection error;
	database db;
	Menu Menu;

	string restockID, dispatchID, productID, productName, productCategory, productBrand, productPrice,
		productStock, productRemark, supplierID,staffID, quantitySupplied, quantityDispatch, salesDispatch;

	Product();
	void viewProduct();
	bool addProduct();
	bool updateProduct();
	bool deleteProduct();


	void viewProductRemarks();
	bool addProductRemarks();
	bool updateProductRemarks();
	bool deleteProductRemarks();


	string staffProductPrompt();
	void productSupplier();
	bool restock();
	bool dispatch();

	void receipt(vector <string> receiptColumnName, vector <string> receiptContent, string Title);


	~Product();
};


#endif