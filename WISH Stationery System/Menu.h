#pragma once
#ifndef MENU_H
#define MENU_H
using namespace std;
#include <string>
#include <vector>


class Menu
{
private:
	vector <string> OptionsList;
	
public:
	string border, header, title, functitle, footer, bracket, address1, address2,CompanyPhoneNumber, text;
	int maxLength,menuWidth, outside_left_padding, inside_left_padding, inside_right_padding;
	Menu();
	~Menu();

	string prompt();

	string formatMenu(const string& text);
	string formatInput(const string& text);
	void formatVectorInput(vector<string>& text);
	string emptyRow();
	string FormattedRow();
	string hideInput();



	bool accessMenu(string pageName);
	void addOption(string option);
	string centerText(const string& text, int width);
	void displayVectorWrapText(vector <string> displayColumns, vector <int> displayColumnsSize, vector<vector<string>> result);
	void capitalizedVectorVectorText(vector<vector<string>>& text);
};


#endif