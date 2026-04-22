#ifndef REPORT_H
#define REPORT_H

#include <iostream>
#include <string>

#include "Menu.h"
#include "DbConnection.h"
#include "ErrorDetection.h"

using namespace std;

class Report
{
public:
	ErrorDetection error;
	database db;
	Menu Menu;

	string  topProductSales, overallSales, option, years, period, totalPercentage;


	vector<vector<string>> result, topSalesProduct ;

	Report();


	bool viewReport();
	void chart();


	~Report();
};


#endif