It is a C++ project connected with TIER-2 Database Architecture.
----------------------------------------------------------------

System Requiements:
* WINDOWS
* INTERNET

DOWNLOAD AND USE: 
* Download Restaurant.exe and libmysql.dll in same from and you can use it by running Restaurant.exe on your system.
It will be connected to Owner's database // SUMIT AGGARWAL

HOW TO MODIFY OR CONNECT IT TO YOUR OWN DATABASE:
* Downlaod all the files to your system in a same folder
* MODIFY host, pw, username, server in cpp file at top with your own databse dateails.

DATABASE STRUCTURE: case sensitive
*Database Name : Restaurant
*Tables : customers, orders, menu, bill

@ menu : 3 columns : dish_id(varchar(30), auto_increment), dish(varchar(30), not null) unique, price(float, not null)
@ customers : 5 coloums : order_id(varchar(30), primary key,), name(varchar(30), not null),  contact(varchar(15), not null), 
