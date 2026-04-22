#include "Report.h"

#include <iostream>
#include <string>
#include <vector>
#include <conio.h>
#include <sstream>
#include <iomanip>
#include <cmath>


using namespace std;

Report::Report()
{
	
	topProductSales = "0";
	overallSales = "0";
	option = "1";
	years = "2000";
	period = "Daily";
	totalPercentage = "0";



	result = {};
	topSalesProduct = {};


	Menu.menuWidth = 69;
	Menu.outside_left_padding = 40;

	error.ErrorMenu.menuWidth = 69;
	error.ErrorMenu.outside_left_padding = 40;
}

bool Report::viewReport() {

	result = {};


	cout << Menu.FormattedRow();


	//Display header based on the selection
	if (option == "1")
		period = "Daily";
	else if (option == "2")
		period = "Monthly";
	else if (option == "3")
		period = "Yearly";


	cout << Menu.formatMenu(period + " Report");
	cout << endl << Menu.FormattedRow();

	//Format and display the columns
	vector <string> displayColumns = { "No", "Product ID", "Product Name", "Product Sales (RM)" };
	vector <int> displayColumnsSize = {  3, 15, 26, 22 };


	bool inputError ;
	//Year to display
	if (option == "3")
	{
		inputError = true;
		while (inputError == true)
		{
			cout << Menu.formatInput("Years (Press X to exit) :");
			cin >> years;

			for (char& c : years)
			{
				c = toupper(c);
			}

			if (years != "X")
			{
				error.data = years;

				if (error.checkSpace() || error.checkDigit())
				{
					continue;
				}

				if ((years.length() != 4) )
				{
					cout << Menu.formatInput("Years should be exactly 4 digits only, enter again ");
					continue;
				}
			}

			else if (years == "X")
			{
				return false;
			}


			inputError = false;


		}

	}



	db.column_list = "productID, ProductName, SUM(TotalSales) as productSales";
	db.table_name = "record_dispatch join product_backup using (ProductID)";
	db.group_by_column = "ProductID";
	db.order_column = " productSales DESC ";

	if (option == "1")
		db.condition = "dateDispatch = CURRENT_DATE";
	else if (option == "2")
		db.condition = "MONTH(DateDispatch) = MONTH(CURRENT_DATE) AND YEAR(DateDispatch) = YEAR(CURRENT_DATE) ";
	else if (option == "3")
		db.condition = "YEAR(DateDispatch) = " + years ;


	db.selectGroupBy();
	db.result(result);

	


	if (result.empty() == false)
	{
		
		
		//Resize the top sales product
		if (result.size() < 3)
			topSalesProduct.resize(result.size(), vector<string>(result[0].size()));
		else 
			topSalesProduct.resize(3, vector<string>(result[0].size()));


		//Store the top sales product
		if(result.size() > 3)
		{
			for (int i = 0; i < 3; i++)
			{
				for (int j = 0; j < result[0].size(); j++)
				{
					topSalesProduct[i][j] = result[i][j];
				}
			}
		}

		else
		{
			for (int i = 0; i < result.size(); i++)
			{
				for (int j = 0; j < result[0].size(); j++)
				{
					topSalesProduct[i][j] = result[i][j];
				}
			}
		}

		
		

		if (option == "3")
		{
			//Refresht the page for yearly report
			system("cls");
			cout << Menu.FormattedRow();
			cout << Menu.formatMenu("Yearly Report");
			cout << endl << Menu.FormattedRow();
		}

		cout << Menu.formatMenu("Top Sales") << endl;
		cout << Menu.FormattedRow();

		//Make all the text capital letters
		Menu.capitalizedVectorVectorText(topSalesProduct);

		//Display Information
		Menu.displayVectorWrapText(displayColumns, displayColumnsSize, topSalesProduct);

		
	



		//Calculate Sales for top sales product
		double num = 0;
		for (int i = 0; i < topSalesProduct.size(); i++)
		{
			num += stod(topSalesProduct[i][2]);
		}

		ostringstream oss;
		oss << fixed << setprecision(2) << num;
		topProductSales = oss.str();



		//Calculate overall sales
		for (int i = 0; i < 3; i++)
		{
			cout << endl;
		}


		num = 0;
		for (int i = 0; i < result.size(); i++)
		{
			num += stod(result[i][2]);
		}

		oss.str("");
		oss.clear();
		oss << fixed << setprecision(2) << num;
		overallSales = oss.str();

		//Display chart
		chart();


		//Display overall sales (daily/ monthly/ yearly)
		cout << "\n\n" << Menu.formatInput("Total " + period + " Sales = RM " + overallSales);



		//Display sales for the top sales products
		cout << endl << Menu.formatInput("Total sales for the top sales products = RM " + topProductSales);
		cout << endl << Menu.formatInput("Total percentage contributed by top sales products =  " + totalPercentage + "%\n");
	}

	else
	{
		cout << Menu.formatInput("No sales were made during the specified time.\n");
	}

}


void Report::chart()
{
	vector <string> percentage;
	int percentagePerMarker = 5;

	//Calculate the percentage of product sales among the top sales
	for (int i = 0; i < topSalesProduct.size(); i++)
	{
		double num;
		num = stod(topSalesProduct[i][2]) / stod(overallSales) * 100;
		num = round(num*10) /10;


		ostringstream oss;
		oss << fixed << setprecision(1) << num;

		percentage.push_back(oss.str());

	}

	double num = 0;
	//Calculate total percentage contributed by top sales product
	for (int i = 0; i < percentage.size(); i++)
	{
		
		num += stod(percentage[i]);
		if (i == percentage.size() -1)
		{
			ostringstream oss;
			oss << fixed << setprecision(1) << num;
			totalPercentage = oss.str();
		}
		
	}



	//Store the chart Diagram
	vector <string> chartDiagram;

	for (int i = 0; i < topSalesProduct.size(); i++)
	{
		double temp = round(stod(percentage[i]) / percentagePerMarker);
		chartDiagram.push_back(topSalesProduct[i][0] + " : " + string(temp, '*') + " ( "+ percentage[i] + "% ) ");
	}






	//Display the chart diagram
	cout << Menu.formatInput("Chart Diagram\n");
	Menu.formatVectorInput(chartDiagram);
	for (int i = 0; i < chartDiagram.size(); i++)
	{
		cout << chartDiagram[i];
	}


}


Report::~Report()
{

}