#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include <mysql.h> 
#include <conio.h>
#include <fstream>
#include "DbConnection.h"
using namespace std;

 
database ::database()   
{
        conn = mysql_init(0);
        if (!conn)
        {
            cout << "Failed To Initialize the database!" << endl;
            _getch();
        }


        conn = mysql_real_connect(conn, "localhost", "root", "", "storehouse_management_system", 3306, NULL, 0);
        if (!conn)
        {
            cout << "Failed To Connect the database! " << mysql_error(conn) << endl;
            _getch();
        }

        res = nullptr;
        statement = {}, column_list = {}, table_name = {}, condition = {};
}

 


bool database::select()
{
    statement = "SELECT " + column_list + " FROM " + table_name + " WHERE " + condition;
    const char* q = statement.c_str();
    qstate = mysql_query(conn, q);
    res = mysql_store_result(conn);
    
    if (!qstate)
    {
        if (mysql_num_rows(res) != 0)
            return true;
        else
            return false;
    }

    else
    {
        cout << Menu.formatInput( "The query execute unsuccessful, Please try again");
        return false;
    }
}
 
void database ::result(vector<vector<string>>& result)
{
    while (row = mysql_fetch_row(res))
    {
        vector<string> currentRow;
        for (int j = 0; j < mysql_num_fields(res); j++) 
        {
            currentRow.push_back(row[j]);
        }
        result.push_back(currentRow);
    }
}

bool database::add()
{
    statement = "INSERT INTO " + table_name + " ( " + column_list + " ) " + " VALUES " + " ( " + values_list + " ) ";
    const char* q = statement.c_str();
    qstate = mysql_query(conn, q);
    return qstate;
}

bool database::update()
{
    statement = "UPDATE " + table_name + " SET " + values_list  + " WHERE " +  condition ;
    const char* q = statement.c_str();
    qstate = mysql_query(conn, q);
    return qstate;
}

bool database::Delete()
{
    statement = "DELETE FROM " + table_name  + " WHERE " + condition;
    const char* q = statement.c_str();
    qstate = mysql_query(conn, q);
    return qstate;
}

bool database::selectGroupBy()
{
    statement = "SELECT " + column_list + " FROM " + table_name + " WHERE " + condition + " GROUP BY " + group_by_column 
        + " ORDER BY " + order_column;

    const char* q = statement.c_str();
    qstate = mysql_query(conn, q);
    res = mysql_store_result(conn);

    if (!qstate)
    {
        if (mysql_num_rows(res) != 0)
            return true;
        else
            return false;
    }

    else
    {
        cout << Menu.formatInput("The query execute unsuccessful, Please try again");
        return false;
    }
}

database ::~database()
{
    mysql_close(conn);
}




