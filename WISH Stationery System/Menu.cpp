#include "Menu.h"
#include <iostream>
#include <conio.h>
#include <algorithm> 
#include <cctype> 
#include <string>

using namespace std;

Menu::Menu() {
	menuWidth = 90;
	outside_left_padding = 30;
	inside_left_padding = 20;
	inside_right_padding = 20;
	border = string(outside_left_padding, ' ') + "|" + string(menuWidth, '-') + "|";
	header = "";
	address1 = "";
	address2 = "";
	CompanyPhoneNumber = "";
	title = "";
	functitle = "";
	footer = "Please select the option according to the menu: ";
	bracket = ") ";
	maxLength = 10;

}

string Menu::prompt()	 {
	string choice = "1";

	system("cls");

	cout << border << endl;

	header = formatMenu(header);
	cout << header << endl;

	if (address1 != "" && CompanyPhoneNumber != "" && address2 != "")
	{
		address1 = formatMenu(address1);
		cout << emptyRow() << address1 << endl;

		address2 = formatMenu(address2);
		cout << emptyRow() << address2 << endl;

		CompanyPhoneNumber = formatMenu(CompanyPhoneNumber);
		cout << emptyRow() << CompanyPhoneNumber << endl;
	}

	cout << border << endl;

	if (title != "")
	{
		title = formatMenu(title);
		cout << emptyRow() << title << endl;
	}

	if (OptionsList.empty())
	{
		functitle = formatMenu(functitle);
		cout << emptyRow() << functitle << endl;
		cout << border;
	}



	if (OptionsList.size() != 0)
	{
		//loop through each option 
		functitle = formatMenu(functitle);
		cout << functitle << endl << emptyRow();


		for (int i = 0; i < OptionsList.size(); i++)
		{
			OptionsList[i] = to_string(i + 1) + bracket + OptionsList[i];
		}

		for (const string& temp : OptionsList) {
			maxLength = max(maxLength, (int)temp.length());
		}

		//Format the option
		for (int i = 0; i < OptionsList.size(); i++)
		{
			if (OptionsList[i].length() == maxLength)
			{
				inside_left_padding = (menuWidth - maxLength) / 2;
				inside_right_padding = menuWidth - maxLength - inside_left_padding;
			}
			else if (OptionsList[i].length() < maxLength)
			{
				inside_left_padding = (menuWidth - maxLength) / 2;
				inside_right_padding = menuWidth - OptionsList[i].length() - inside_left_padding;
			}

			cout << string(outside_left_padding, ' ') + "|" + string(inside_left_padding, ' ') + OptionsList[i] + string(inside_right_padding, ' ') + "|";
			cout << endl;
		}



		cout << border << endl;

		footer = formatInput(footer);
		cout << footer;
		cin >> choice;
		while (!(all_of(choice.begin(), choice.end(), isdigit)) || stoi(choice) < 1 || stoi(choice) > OptionsList.size() || cin.peek() == ' ')
		{
			cout << formatInput("Follow the instruction!!!");
			cout << formatInput("Re-enter your choice : ");
			cin >> choice;
		}
	}

	return choice;
}

string Menu::formatMenu(const string& text)
{
	if (menuWidth < text.size())
	{
		inside_left_padding = 0;
		inside_right_padding = 0;
		return "the menuWidth is too short, please adjust it";
	}
	else
	{
		inside_left_padding = (menuWidth - text.length()) / 2 ;
		inside_right_padding = menuWidth - text.length() - inside_left_padding;
	}
	return string(outside_left_padding , ' ') + "|" + string(inside_left_padding , ' ') + text + string(inside_right_padding , ' ') + "|";
}

string Menu::formatInput(const string& text)

{
	if (menuWidth < text.length())
	{
		inside_left_padding = 0;
		return "the menuWidth is too short, please adjust it";
	}
	else
	{
		inside_left_padding = (menuWidth - text.length()) / 2;
	}
	return "\n" + string(outside_left_padding, ' ') + string(inside_left_padding + 1, ' ') + text;
}

void Menu::formatVectorInput(vector<string>& text)
{
	maxLength = 10;
	for (const string& temp : text) {
		maxLength = max(maxLength, (int)temp.length());
	}

	for (int i = 0; i < text.size(); i++)
	{
		inside_left_padding = (menuWidth - maxLength) / 2;

		text[i]= "\n" + string(outside_left_padding, ' ') + string(inside_left_padding, ' ') + text[i];

	}
}

string Menu::emptyRow()
{
	return string(outside_left_padding, ' ') + "|" + string(menuWidth, ' ') + "|\n" ;
}

string Menu::FormattedRow()
{
	return string(outside_left_padding, ' ') + "|" + string(menuWidth, '-') + "|\n";
}

string Menu::hideInput()
{
	int i = 0;
	char a;
	char temp[100] = {};
	bool show = false;
	
	//Hide input
	for (i = 0;;) {
		a = _getch();

		//Delete if backspace is pressed
		if (a == '\b' && i > 0) {
			cout << "\b \b";
			--i;
			temp[i] = '\0';
			continue;
		}
		
		//To avoid the error when the backspace is pressed even nothing left in temp
		else if (a == '\b' && i == 0)
		{
			cout << "";
		}

		//End if enter is pressed
		else if (a == '\r') {
			temp[i] = '\0';
			break;
		}

		//Switch to show or hide if tab is pressed
		else if (a == '\t')
		{
			//Set show true
			show = !show;

			//Set cursor position to initial position
			for (int j = 0; j < i; j++)
			{
				cout << "\b \b";
			}

			//Show input
			if (show == true)
			{
				cout << temp;
			}

			//Re-hide input
			else if (show == false)
			{
				cout << string(i, '*');
			}
		}

		//Hide input
		else
		{
			temp[i] = a;
			++i;

			if (show == true)
			{
				cout << a;
			}

			else if (show == false)
			{
				cout << "*";
				
			}
		}
	}

	text = temp;
	return text;
}




bool Menu::accessMenu(string pageName) {
	bool inputError = true;
	string input;
	string confirmation = "Confirm redirect to " + pageName + " page? (Y / N) : ";
	confirmation = formatInput(confirmation);
	cout << endl << confirmation;

	while (inputError)
	{
		cin >> input;
		transform(input.begin(), input.end(), input.begin(), ::toupper);
		if (input.length() != 1) {
			cout << formatInput("Enter only one character. Please enter again (Y / N)  : ");
			continue;
		}

		if (input[0] != 'Y' && input[0] != 'N') {
			cout << formatInput("Enter according instuation. Please enter again (Y / N) : ");
			continue;
		}
		inputError = false;
	}

	if (input[0] == 'Y')
	{
		cin.clear();
		cin.ignore(1000, '\n');
		return true;
	}

	else if (input[0] == 'N')
	{
		cin.clear();
		cin.ignore(1000, '\n');
		return false;
	}
}

void Menu::addOption(string option) {
	OptionsList.push_back(option);
}

string Menu::centerText(const string& text, int width)
{
	int left_padding, right_padding;
	if (width < text.size())
	{
		left_padding = 1;
		right_padding = 1;
	}
	else
	{
		left_padding = (width - text.size()) / 2;
		right_padding = width - text.size() - left_padding;
	}
	return "|" + string(left_padding, ' ') + text + string(right_padding, ' ')  ;
}

void Menu::displayVectorWrapText(vector <string> displayColumns, vector <int> displayColumnsSize, vector<vector<string>> result )
{
	string temp = string(outside_left_padding, ' ');

	for (int i = 0; i < displayColumns.size(); i++)
	{
		temp += centerText(displayColumns[i], displayColumnsSize[i]);
	}

	temp = temp + '|';
	cout << temp << endl;
	cout << FormattedRow();

	for (int i = 0; i < result.size(); i++)
	{

		vector <string> text;

		temp = {};
		temp = string(outside_left_padding, ' ') + string(1, '|');

		//Display the number of row
		if (i + 1 < 10)
		{
			temp += to_string(i + 1) + string(2, ' ');
		}
		else if (i + 1 >= 10 && i + 1 < 100)
		{
			temp += to_string(i + 1) + string(1, ' ');
		}
		else if (i + 1 >= 100 && i + 1 < 1000)
		{
			temp += to_string(i + 1);
		}


		bool exceed = true;

		int round = 1;

		while (exceed == true)
		{
			int testExceed = 0;
			if (round > 1)
			{
				temp = {};
				temp = string(outside_left_padding, ' ') + string(1, '|') + string(3, ' ');
			}

			//Display the informations
			for (int j = 0; j < result[i].size(); j++)
			{
				text.push_back(result[i][j]);
				
				//If the text is not empty
				if (!text[j].empty())
				{
	
					//Detect whether the text exceed the column size or not
					if (text[j].length() > displayColumnsSize[j + 1])
					{
						testExceed++;
						size_t pos = displayColumnsSize[j + 1] - 2;


						// Find the nearest comma or space backward within column size
						while (pos > 0 && text[j][pos] != ',' && text[j][pos] != ' ') {
							pos--;


						}


						// If no comma or space is found, break the line with respective column size
						if (pos == 0) {
							pos = displayColumnsSize[j + 1];
						}


						// Print the part of the text up to the found position
						temp += centerText(text[j].substr(0, pos), displayColumnsSize[j + 1]);


						// Remove the printed part 
						text[j] = text[j].substr(pos + 1);

						// Trim leading spaces and commas from the remaining text
						int start = text[j].find_first_not_of(" ,");

						if (start != string::npos)
							text[j] = text[j].substr(start);
					}

					else
					{
						
						text[j].erase(text[j].find_last_not_of(" \t\n\r") + 1);
						temp += centerText(text[j], displayColumnsSize[j + 1]);
						text[j] = "";
					}

				}

				//If the text is empty (not exceed text in round 2 or above)
				else if (text[j].empty())
				{
					temp += centerText(" ", displayColumnsSize[j + 1]);;
				}
				
				


				cout << temp;
				temp = {};
			}


			cout << '|';
			cout << endl;

			if (testExceed != 0)
			{
				exceed = true;
			}
			else if (testExceed == 0)
			{
				exceed = false;
			}
			round++;
		}
		cout << FormattedRow();

	}
}

void Menu::capitalizedVectorVectorText(vector<vector<string>>& text)
{
	for (int i = 0; i < text.size(); i++)
	{
		for (int j = 0; j < text[i].size(); j++)
		{
			for (char& c : text[i][j])
			{
				c = toupper(c);
			}
		}
	}
}

Menu::~Menu() {
	OptionsList.clear();
}