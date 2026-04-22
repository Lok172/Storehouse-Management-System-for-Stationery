# WISH Stationery Storehouse Management System

## Overview
Storehouse Management System for Stationery is designed to help both administrative and operational staff to increase storehouse management efficiency with modules for admin and staff to assist in modification of details of products and suppliers, product history tracking, and financial reports.

## Tech Stack
- C++
- MySQL
- Visual Studio

## Features

**Admins**
- CRUD on products database (product details, remarks, history tracking, etc.)
- CRUD on supplier database
- CRUD on staff database
- CRUD on admin database
- Financial reports

**Staff**
- CRUD on products database (remarks, stock, etc.)

## Setup in Visual Studio

1. Get MySQL Connector: https://dev.mysql.com/downloads/connector/cpp/

2. In project properties:
   - C/C++ → General → Additional Include Directories → add the `include` folder
   - Linker → General → Additional Library Directories → add the `lib64` folder
   - Linker → Input → Additional Dependencies → type vs14/mysqlcppconn.lib

3. Get `libmysql-cpp` from NuGet Package Manager

4. Test Accounts:
   - Admin: ID: A001, PW: 123
   - Staff: ID: S001, PW: 123
