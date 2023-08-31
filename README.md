It is a C++ project connected with TIER-2 MySQL Database Architecture.
----------------------------------------------------------------
It is a online restaurant operation system where customers can place their orders from menu and then it will be proceed by billing and then to employees and ADMIN will have all the data access and can set menu or eidt menu for customers.

FUNCTIONALITIES : ACCESS
1) CREATE MENU : ADMIN
2) EDIT MENU : ADMIN
3) DISPLAY MENU : ALL
4) PLACE ORDER : ALL
5) BILLING : SYSTEM AUTOMATIC
6) PRINT BILL : SYSTEM AUTOMATIC
7) DISPLAY CUSTOMER'S DATA(NAME, CONTACT, ORDER HISTORY ETC) : ADMIN


System Requiements:
* WINDOWS
* INTERNET

DOWNLOAD AND USE: 
* Download Restaurant.exe and libmysql.dll in same from and you can use it by running Restaurant.exe on your system.
It will be connected to Owner's database // SUMIT AGGARWAL
You can only use it for Placing your Order rest of the functions are for admin. 

HOW TO MODIFY OR CONNECT IT TO YOUR OWN DATABASE:
* Downlaod all the files to your system in a same folder.
* MODIFY host, pw, username, server in restaurant.cpp file at top with your own databse details.

DATABASE STRUCTURE: MySQL: case sensitive
*Database Name : Restaurant
*Tables : customers, orders, menu, bill

* menu : 3 columns : dish_id(varchar(30), auto_increment), dish(varchar(30), not null) unique, price(float, not null)
* customers : 5 coloums : order_id(varchar(30), primary key,), name(varchar(30), not null),  contact(varchar(15), not null), date(date), time(time)
* orders : 5 coloums : order_id(same as customers), dish_id, dish, quantity(int not null), price(float)
* bill : 6 coloums : order_id(same), total(float), paid(float), tax(float), change_amount(float), discount(float);

