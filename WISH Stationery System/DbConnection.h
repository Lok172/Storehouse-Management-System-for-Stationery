#pragma once
#ifndef DBCONNECTION_H
#define DBCONNECTION_H

#include "Menu.h"

#include <iostream>
#include <string>
#include <vector>
#include <mysql.h> 

using namespace std;


 
class database
{
public:
    Menu Menu;

    database();
    ~database();
    
    int qstate;
    MYSQL* conn;
    MYSQL_ROW row;
    MYSQL_RES* res; 
    string statement, column_list, table_name, condition, values_list, group_by_column, order_column;
    char* query;
  
    
    bool select();
    void result(vector<vector<string>>& result);
    bool add();
    bool update();
    bool Delete();
    bool selectGroupBy();


};
#endif 

