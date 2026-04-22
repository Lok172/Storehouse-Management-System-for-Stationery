#ifndef ERRORDETECTION_H
#define ERRORDETECTION_H

#include <iostream>
#include <string>
#include "Menu.h"

using namespace std;

 
class ErrorDetection
{
public:
	string data;
	Menu ErrorMenu;

	ErrorDetection();


	bool checkDigit();
	bool checkDouble();
	bool checkSpace();
	bool checkOption(string option1, string option2);
	bool checkEmpty();

	~ErrorDetection();

};


#endif // !ERRORDETECTION_H


