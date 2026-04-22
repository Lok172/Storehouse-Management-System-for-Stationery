#include <iostream>
#include <sstream>
#include <iomanip>
#include <conio.h>
#include <string>
#include <algorithm>
#include "ErrorDetection.h"

using namespace std;
 
ErrorDetection ::ErrorDetection() {
	data = {};
}

  
 bool ErrorDetection ::checkDigit()
{
	if (!(all_of(data.begin(), data.end(), isdigit)))
	{
		cin.clear();
		cin.ignore(1000, '\n');
		cout << ErrorMenu.formatInput("Enter number only.\n");
		return true;
	}
	else 
		return false;
}

 bool ErrorDetection::checkDouble() {
	 // Check for valid number format
	 double test;
	 size_t pos;

	 // Convert string to double and ensure the entire string was a valid number
	 try {
		 test = stod(data, &pos);
	 }
	 catch (...) {
		 return true; // If conversion failed
	 }

	 if (pos != data.length()) {
		 return true;
	 }


	

	 return false;
 }
  
bool ErrorDetection::checkSpace()
{
	if (cin.peek() == ' ')
	{
		cin.clear();
		cin.ignore(1000, '\n');
		cout << ErrorMenu.formatInput("Please do not include the space.\n");
		return true;
	}
	else
		return false;
}

bool ErrorDetection::checkEmpty()
{
	if (data.empty())
	{
		cin.clear();
		cout << ErrorMenu.formatInput("Please do not enter an empty input.\n");
		return true;
	}
	else
		return false;
}
 
bool ErrorDetection::checkOption(string option1, string option2)
{
	if (data != option1 && data != option2)
	{
		cin.clear();
		cin.ignore(1000, '\n');
		cout << ErrorMenu.formatInput("Please enter according to the option.\n");
		return true;
	}
	else
		return false;
}	

ErrorDetection ::~ErrorDetection() {

}