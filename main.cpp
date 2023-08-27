

/* CODE By : SUMIT AGGARWAL */

#include <iostream> // Input-Output
#include <iomanip>  // for setw
#include <ctype.h>  //isdigit
#include <sstream>  //stringstream
#include <cmath>    //round
#include <string>   //stof,stod
#include <limits>   //numeric_limits
#include <ctime>    // DATE AND TIME
#include <cstdio>
#include <chrono>
#include <thread>
#include <windows.h>
#include <mysql.h>
#include <mysqld_error.h>
#include <sstream>
using namespace std;

const char *HOST = "localhost";
const char *USER = "root";
const char *DB = "Restaurant";
MYSQL *conn;

//---------------------------Forward Declarations--------------------------------------------------------------

class resturant;
class Co_Details;
void deleteFromOrders(string order_id, string dish_id = "null", bool choice = true);

//------------x---------------x-------------x-------------------x--------------x----------------x----------------x


//--------------------CLASS FORCED RETURN----------------------

//(it returns the value of desired data type by forcing user to input correct information)------------

class forcereturn
{

private:
    string String_Reader(int num);       // it's a user-build functoin works as replacement of  <<  cin  >> functon and take string as input than convert that string into other datatypes  
 
public:
    unsigned int StoIconv(void);         // string to integer converter 
    unsigned long long int StoUILLconv(void);  // string to unsigned unsigned long long integer conv
    char StoCconv(void);                       // string to character converter
    double StoDconv(void);                // string to double converter
    float StoFconv(void);               // string to float converter
};

//------------x---------------x-------------x-------------------x--------------x----------------x----------------x

//-----------------CLASS Custmor Details-----------------------

//class Co_Details
//
//{
//
//private:
//    friend class resturant;
//
//    forcereturn use; //differnt class with object
//
//    string Co_name;
//    unsigned short int itemcount; // to store total number of item ordered for each order
//    long long int Co_phonenumber;  // to store customer's phone number
//    unsigned short int Co_visit;   // To store the custmore frequrnt visit
//    unsigned short int Co_OrderNo; // To store every order no.
//    float discount, tax_charge, total = 0.00 /* total amount which custmor have to pay */, paid /* amount paid by custmor */, change /*amount return by worker */;
//
//    
//    string storetime; // to store a particular time
//
//    void SaveOrderNo(unsigned short int i);  // function to assign order no to co_orderno
//    void SaveName(void);                     // assign name to Co_name member
//    void SavePhoneNo_(void);                // assign phonenumber to co_phonenumber
//    void Save_O_DateTime(void);             // display the current time and also store each diplay  "time" in different strings for every case 
//    void Display_O_DateTime(void);          //  display the stored time (not current time)
//    void CalculateBill(unsigned short int accept); // it will calulate the bill 
//    void PrintBill(unsigned short int accept, bool dis); // it will print the bill
//    inline void Count_Co_Visit(void) { Co_visit++; }; //functioning of function Count_Co_Visit
//
//public:
//    Co_Details() : Co_visit(0), itemcount(0) {}  // Constructor
//};
//
////------------x---------------x-------------x-------------------x--------------x----------------x----------------x
//
////----------------Save-Name Function-----------------
//
//inline void Co_Details ::SaveName(void)
//{
//    cin.ignore(numeric_limits<streamsize>::max(), '\n');
//    getline(cin, Co_name);
//}
//
////------------x---------------x-------------x-------------------x--------------x----------------x----------------x
//
////----------------Save-Phone Number Function-------------------
//
//inline void Co_Details ::SavePhoneNo_(void)
//{
//    Co_phonenumber = use.StoUILLconv();
//    if (Co_phonenumber < 1000000000 || Co_phonenumber > 9999999999)
//        Co_phonenumber = 232323232323;
//}

//------------x---------------x-------------x-------------------x--------------x----------------x----------------x

//-----------------CLASS RESTURANT-------------------

class resturant
{
private:
    //*friend class Co_Details;
    // friend void Co_Details ::SaveOrderNo(unsigned short int i);
    // friend void Co_Details ::CalculateBill(unsigned short int accept);

    forcereturn use;          // differnt class and its object
    //*Co_Details Details[1000]; // differnt class and its object

    // static float price[50];       // store price for each item in the menu 
    // static string dishes[50];     // store item name for each item in the menu
    //*static unsigned short int OrderNo;  // the OrderNO count till now (only store one value and rewrite that value when order no changes)  
    //*static unsigned int Customer_Count; // total number of custmors till now (repeated customers will be counted as one)
    // static unsigned short int counter;  // total number of items in the menu

    //*unsigned short int quantity;        // how many quantity custmor wants for a particular item
    //*unsigned short int order_id[1000][50]; // store DISH'ID for every order with each item in that order :::: order_id[x][y] === x---> is order no. and y----> is item number in that particular order
    //*unsigned short int storequantity[1000][50]; // store quantity for every order with its each items 

    
    // ------ void editmenu(void) will call these 4 functions  -----------

    void menumaker(void);        // helps to make a menu 
    void menudisplay(void);     // display the menu you set
    void changeprice(void);    // change price or dish name in the menu
    void RemoveItem ( void ) ;              // remove items from menu
 

public:
    void editmenu(void);             // to view all the options available for menu - setting
    void takeorder(void);            // take order 
    void DisplayData(void);          // display custmor's data with the help of order number          
    void SaveOrder_id(unsigned short int save);   // assign dish'id to  order'id[x][y](above) 
    

} order;


// MYSQL FUNCTIONS ---------->

// ********************   ASK PASSWORD FROM USER TO ACCESS DATABASE **********************  

string getPassword(){                
	cout << "\t\tENTER PASSWORD TO CONNECT WITH DATABASE:  ";
	string pwd;
	getline(cin, pwd);
	system( "cls" );
	
	return pwd;
}

//// ---------------------Save orderdone Date and Time----------------------------

string getDate() {
    auto now = std::chrono::system_clock::now();
    auto time_t_now = std::chrono::system_clock::to_time_t(now);

    std::tm* tm_now = std::localtime(&time_t_now);

    std::ostringstream oss;
    oss << std::put_time(tm_now, "%Y-%m-%d");

    return oss.str();
}

//  ***********************   TOTAL NUMBER OF ITEMS IN MENU   *********************

int getTotalItems(){
    string show = "SELECT dish_id FROM menu";
    if (mysql_query(conn, show.c_str()))
    {
        cout << "Error: " << mysql_error(conn) << endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(4000));
    }
    else
    {
        MYSQL_RES *res;
        res = mysql_store_result(conn);
        if (res)
        {
            return mysql_num_rows(res);
        }
    }
    return 0;
}

//  ***********************   TOTAL NUMBER OF ORDERS ON DATE YYYY-MM-DD   *********************
int getOrderNo()
{
    string date = getDate();
    string show = "SELECT order_id FROM customers WHERE date = ' " +date+ " '";

    if(mysql_query(conn, show.c_str()))
    {
        cout << "Error: " << mysql_error(conn) << endl;
        sleep(4000);
    }
    else
    {
        MYSQL_RES *res;
        res = mysql_store_result(conn);
        if (res)
        {
            return mysql_num_rows(res);
        }
    }
    return 0;
}

//  ***********************   SEARCH DISH IN ORDER   *********************
bool searchDishInOrder(string order_id, string dish_id)
{
    
    string show = "SELECT dish_id FROM orders WHERE dish_id = ' " +dish_id+ " ' AND order_id = '"+order_id+"'";

    if(mysql_query(conn, show.c_str()))
    {
        cout << "Error: " << mysql_error(conn) << endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(4000));
    }
    else
    {
        MYSQL_RES *res;
        res = mysql_store_result(conn);
        if (res)
        {
            MYSQL_ROW row = mysql_fetch_row(res);
            if(row && row[0])
            return true;
        }
    }
    return false;
}

//   *********************  GET PRICE FOR DISHES FROM DATABSE - TABLE - menu  ********************
string getPrice(string dish_id)
{
    string show = "SELECT price FROM menu WHERE dish_id = ' " + dish_id + " '";

    if (mysql_query(conn, show.c_str()))
    {
        cout << "Error: " << mysql_error(conn) << endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(4000));
    }
    else
    {
        MYSQL_RES *res;
        res = mysql_store_result(conn);
        if (res)
        {
            MYSQL_ROW row;
            row = mysql_fetch_row(res);
            
			if(row && row[0])
            return row[0];
        }
    }
    return "null";
}

//   *********************  UPDATE ORDER DISHES TO DATABSE - TABLE - orders  ********************
void updateOrder(string order_id, string dish_id, string quantity, string price)
{
	price = to_string(stof(quantity)* stof(price));
    string upd = "UPDATE orders SET quantity = '"+quantity+"', price = '"+price+"' WHERE dish_id = ' " + dish_id + " ' AND order_id = '"+order_id+"'";

    if (mysql_query(conn, upd.c_str()))
    {
        cout << "Error: " << mysql_error(conn) << endl;
       std::this_thread::sleep_for(std::chrono::milliseconds(4000));
    }
    else
    {
        cout << "\n\t\tORDER EDITED"<<endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
    }
}

//   *********************  INSERT ORDER ITEM TO DATABSE - TABLE - orders  ********************
void insertItemsToOrder(string order_id, string dish_id, string quantity, string price)
{
	if(searchDishInOrder(order_id, dish_id))
	{
		if(quantity == "0")
		deleteFromOrders(order_id, dish_id, false);
		
		else
		updateOrder(order_id, dish_id, quantity, price);
		
		return;
	}
	
	
	price = to_string(stof(quantity)* stof(price));
    string ins = "INSERT INTO orders (order_id, dish_id, quantity, price) VALUES ('" + order_id + "', '" + dish_id + "', '" + quantity + "', '" + price + "')";

    if (mysql_query(conn, ins.c_str()))
    {
        cout << "\t\tError: " << mysql_error(conn) << endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(3000));
    }
    else
    {
        cout << "\t\t  Item Added" << endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
    }
    system("cls");
}

//   *********************  INSERT DETAILS ITEM TO DATABSE - TABLE - customer  ********************
void insertCustomerDetails(string order_id, string contact, string name)
{
	string date = order_id.substr(0, 10);
    string ins = "INSERT INTO customers (order_id, contact, name, date) VALUES ('" + order_id + "', '" + contact + "', '" + name + "', '" + date + "')";

    if (mysql_query(conn, ins.c_str()))
    {
        cout << "\t\tError: " << mysql_error(conn) << endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(7000));
    }
    else
    {
        cout << "\t\tDeatails Filled" << endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
        
    }
    system("cls");
}

//   *********************  DELETE ORDER FROM DATABSE - TABLE - orders  ********************
void deleteFromOrders(string order_id, string dish_id, bool choice)
{
	string del;
	if(choice)
    del = "DELETE FROM orders WHERE order_id = '"+ order_id +"' ";
    
    else
    del = "DELETE FROM orders WHERE order_id = '"+ order_id +"' AND dish_id = '"+dish_id+"'";

    if(mysql_query(conn, del.c_str()))
    {
        cout << "\n\t\tError: " << mysql_error(conn) << endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(3000));
    }

    else
    {
        cout << "\n\t\tOrder Cancelled" << endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(400));
    }
    system("cls");
}

////------------x---------------x-------------x-------------------x--------------x----------------x----------------x
//
////----------------Save Order NUmber Function-----------------------
//
//inline void Co_Details ::SaveOrderNo(unsigned short int i)
//{
//    resturant ob;
//    Co_OrderNo = ob.OrderNo;
//}
//
////------------x---------------x-------------x-------------------x--------------x----------------x----------------x
//


////------------x---------------x-------------x-------------------x--------------x----------------x----------------x
//
////--------------------Display order done Date and Time-----------------------------
//
//inline void Co_Details ::Display_O_DateTime(void)
//{
//    cout << "\n  Date and Time(day mm dd hours:min:sec yy) was: " << storetime << endl;
//}
//
////------------x---------------x-------------x-------------------x--------------x----------------x----------------x
//
////-----------------Calculate Bill Functon---------------------------
//
//void Co_Details ::CalculateBill(unsigned short int accept)
//{
//
//    cout << "\n  Enter Discount like the below example: \n  ----  Enter 20 for '20%' discount  ----  \n";
//    cout << "\n  Enter Discount :   ";  discount = use.StoFconv();
//  
//    while (discount >= 100.00f)
//    {
//        cout << "\n\n  *****   invalid input   ***** \n  Enter valid discount ( 0 <= Discount < 100 )\n";
//
//        cout << "\n  Enter Discount :   ";  discount = use.StoFconv(); // how much discount u want to provide like 20 = 20percent not 20rs
//    }
//
//    tax_charge = 18.00; // 18 percent tax charges
//
//    total = 0;
//
//    for (int loop = 0; loop < itemcount; loop++)
//    {
//        total = total + ((order.price[order.order_id[accept - 1][loop] - 1001]) * (order.storequantity[accept - 1][loop])); // total = 0(total) + price*quantity
//    }
//
//    total = total - (((total) * (discount)) / 100.00f);  // total = total - discount_charges
//    
//    total = total + (((total) * (tax_charge)) / 100.0f); // total = total + tax_charges
//    
//    cout << "\n\n  Total Price:  " << float ( total  ) << "rs. " << endl;
//    
//    cout << "\n Enter the amount paid by custmor:   ";   paid = use.StoFconv();
//    
//
//    while ((paid) != 0.00f && (paid) < ( float (total) ) )
//    {
//        cout << "\n\n  !!!   Sorry   !!!  The amount is not enough for your order. \n";
//        cout << "\n  -----   Enter enough amount to contiune   -----  OR  -----   Enter amount '0' to view more options   -----  \n ";
//
//        cout << "\n  ENTER :  ";    paid = use.StoFconv(); //price paid by custmor
//        
//        cin.ignore(numeric_limits<streamsize>::max() );
//    }
//
//    if ((paid) >= ( float ( total ) ))
//
//    {
//        change = (paid) - ( float (total) );
//        cout << "\n\n  Change ( the amount worker has to return )  :   " << change << "rs. " << endl;
//    }
//}
//
////------------x---------------x-------------x-------------------x--------------x----------------x----------------x
//
////-----------------------Print Bill Function------------------------
//
//void Co_Details ::PrintBill(unsigned short int accept, bool dis) // accept = order no. && bool dis will be for deciding whether to print or not this detail
//
//{
//    cout << "\n\n\n\t\t!!!!!!!!   SUMIT AGGARWAL aka CODE OWNER.RESTURANT.Details   !!!!!!!!\n";
//    cout << "\n\t\t\t!!!!!!   OrderNo :: [" << accept << "]   !!!!!!! \n";
//
//    if (dis == true) // only print these details for resturant (store data)  ,,,, don't print them on bill
//
//    {
//        cout << "\n        !!!!!!  Total custmor till now =  -- " << order.Customer_Count << " --   !!!!!!! \n\n";
//        ;
//
//        cout << "\n  ---------   CUSTOMER'S DETAILS   ----------\n";
//        cout << "\n  Customer's frequent visit = " << Co_visit << endl;
//        cout << "\n  Phone number : " << Co_phonenumber << endl;
//    }
//
//    cout << "\n  Name : " << Co_name << endl;
//
//    cout << "\n  -------------   ORDER   -------------\n";
//    cout << "\n  DISH " << setw(13) << ": "
//         << "  Q(n)      :   Price*Q(n) rs.\n\n";
//
//    for (int loop = 0; loop < itemcount; loop++)
//
//    {
//
//        if (dis == true || order.storequantity[accept - 1][loop])
//
//        {
//
//            cout << "\n  " << order.dishes[order.order_id[accept - 1][loop] - 1001] << setw(20 - (order.dishes[order.order_id[accept - 1][loop] - 1001].length()));
//            cout << ": Q(" << order.storequantity[accept - 1][loop] << ")      :   ";
//            cout << ((order.price[order.order_id[accept - 1][loop] - 1001]) * order.storequantity[accept - 1][loop]) << " rs.\n";
//        }
//    }
//
//    cout << "  ---------------------------------------------------------\n";
//    cout << "\n  Total         :  " << total   << " rs.\n";
//    cout << "\n  Paid          :  " << paid << " rs.\n";
//    cout << "\n  Change        :  " << change << " rs.\n";
//    cout << "\n  Tax Charges   :  " << tax_charge << " %\n";
//    cout << "\n  Discount      :  " << discount << " %\n";
//    cout << "---------------------------------------------------------\n";
//
//    if (dis == true)
//        Display_O_DateTime();
//
//    else
//
//    {
//
//        cout << "\n     ######   WE HOPE YOU ENJOY YOUR MEAL   ######\n";
//
//        Save_O_DateTime();
//    
//    }
//}
//
////------------x---------------x-------------x-------------------x--------------x----------------x----------------x
//
//------------------------TAKE ORDER FUNCTION----------------------------

void resturant ::takeorder(void)
{
    int orderNo = getOrderNo() + 1;  // orderNo
    string order_id = getDate() + " " + to_string(orderNo); // order_id (unqiue for everyorder) = date + order_no;
    bool details = false;   // customer details are stored -- not yet
    string name;           // custmor name
    string contact;        // contact;
    int cancel_0;          // to track loop

    if (!getTotalItems())
    {
        cout << "\n\t\t  !!!!!!   SORRY   !!!!!\n\t\t  **********   NO ITEM TO ORDER   **********\n\n";
    }
    else
    {
       cout << "\n\t\t  !!!!!!   Order No [" << orderNo << "]    !!!!!!! \n";
       do // @@@@@@@ EDIT ORDER @@@@@@
       {
          char loop = '1'; // condition for --for loop
          for (; loop == '1';)
           {
           	   cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');    
               // dish_id for the dish to be ordered
               cout << "\n\t\t  Enter id num of the dish  :  ";     
               string dish_id;
               getline(cin, dish_id);

               // quantity of the dish
               cout << "\n\t\t  Enter Quantity  :  ";
               string quantity;
               getline(cin, quantity);
               
               // price of the dish
               string price = getPrice(dish_id);
               
               // insert into orders table into database
               insertItemsToOrder(order_id, dish_id, quantity, price);

               // ADD MORE ITEMS / CANCEL ORDER / ORDER COMPLETED
               cout << "\n\n\t\tEnter '1' to add more items\t\tEnter '0' if done  \n";
               cout << "\n\t\t\tEnter 'c' to Cancel Order  \n";
               cout << "\n  ENTER  :  ";
               loop = use.StoCconv();
               system("cls");

               while (loop != '1' && loop != '0' && loop != 'c') // in case of invalid input-- repeat
               {
                   cout << "\n\n  *****   INVALID INPUT   *****\n";
                   cout << "\n\n\t\tEnter '1' to add more items\t\t\tEnter '0' if done  \n";
                   cout << "\t\t\t\tEnter 'c' to Cancel Order  \n";
                   cout << "\n\n\t\tENTER  :  ";
                   loop = use.StoCconv();
                   system("cls");
                }

           }// for loop = 0, c;

           if(loop == 'c')
           {
                deleteFromOrders(order_id);
                return;
           }

           if (loop == '0' && !details) // if details are already filled don't ask again(in case of order edit)
           {
           	  cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');    
              cout << "\n\n\t\t  Enter Custmor's name  :  ";
              getline(cin, name);

              cout << "\n  Enter Customer's Phone Number  :  ";
              getline(cin, contact);

              system("cls");

              //CalculateBill(OrderNo); // payment calculations

              cout << "\n\t\t ENTER '0' TO EDIT ORDER\t\t\tENTER '1' TO CONTINUE";
              cout<<"\n\n\t\t\t ENTER:  ";
              cancel_0 = use.StoIconv();
              system("cls");

              if (cancel_0 == 0) // either the order is free or custmor want to cancel order or custmor want to edit the order
              {
                   cin.ignore(numeric_limits<streamsize>::max() ); // to clean buffer

                   cout << "\n\n\t\tEnter '0' to cancel order\t\t\tEnter '2' to edit order\n";
                   cout << "\n\t\t\tEnter '1' if order is free\n";
                   cout << "\n  ENTER  :  ";            
                   cancel_0 = use.StoIconv();

                   while (cancel_0 != 0 && cancel_0 != 1 && cancel_0 != 2) // in case of invalid input-- repeat
                   {
                   cin.ignore(numeric_limits<streamsize>::max(), '\n'); // to clean buffer
                   cout << "\n\n\t\tEnter '0' to cancel order\t\tEnter '1' to edit order\n";
                   cout << "\n\t\t\tEnter '2' if order is free\n";
                   cout << "\n  ENTER  :  ";            
                   cancel_0 = use.StoIconv();
                   } // while -ends here
              } // if -ends here

               switch (cancel_0)
               {
               case 0: // case to cancel order
               deleteFromOrders(order_id);
               break;

               case 1: //order free
               insertCustomerDetails(order_id, contact, name); // contact, name into database
               return;

               case 2: // edit order
               cout << "\n\n       *************    You can CHANGE, DELETE OR ADD ITEM BY SIMPLY ENTERING THE DISH'ID AND QUANTITY   ***********\n ";
               break;

               default: // print bill on any default case
               insertCustomerDetails(order_id, contact, name);
               //PrintBill(OrderNo, false);        
               }
            }
       } while (cancel_0 == 2); // to edit order
   }
}

////------------x---------------x-------------x-------------------x--------------x----------------x----------------x
//
//
//
////-----------------DISPLAY DATA FUNCTION---------------------
//
//void resturant :: DisplayData(void)
//{
//    for (char decide = '1'; decide == '1';)
//    {
//        cout << "\n  Enter Order Number  :  ";           unsigned short int accept = use.StoIconv();
//        
//        if (Details[accept - 1].Co_OrderNo == accept && accept != 0 && accept <= OrderNo)
//        {
//            if (Details[accept - 1].itemcount == 0)
//                cout << "\n\n  !!!!!!   Sorry   !!!!!    **********   Don't have a order to display   --------   To place a order follow these options   ********** \n";
//            else
//            {
//                Details[accept - 1].PrintBill(accept, true);
//            }
//        }
//        else
//        {
//            cout << "\n\n  *******   No matches Found for Order Number : " << accept << "   ***********\n";
//        }
//
//        cout << "\n  Enter '1' to view more Orders                              Enter '0' to go back\n";
//        cout <<   "                                         ENTER  :  ";         decide = use.StoCconv();
//        while (decide != '1' && decide != '0')
//        {
//        cout << "\n                               *****   INVALID INPUT   *****\n";
//        cout << "\n  Enter '1' to view more Orders                              Enter '0' to go back\n";
//        cout <<   "                                         ENTER  :  ";         decide = use.StoCconv();      }
//  
//    }
//
//}
//
////------------x---------------x-------------x-------------------x--------------x----------------x----------------x
//
////----------------STATIC VARIABLE SCOPING-------------------
////initilaize with '0'.
//
//unsigned int resturant ::Customer_Count = 0;
//// unsigned short int resturant ::counter = 0;
//// float resturant ::price[50];
//// string resturant ::dishes[50];

//------------x---------------x-------------x-------------------x--------------x----------------x----------------x

//----------------MENU-MAKER FUNCTION----------------------

void resturant ::menumaker(void)

{
    
    cout << "\n\n                         :::  NOTE  ::: \n\n"
         << "  THERE WILL BE A UNIQUE DISH'ID FOR EVERY ITEM YOU PUT ON THE MENU \n"
         << "  ORDERS CAN ONLY BE TAKEN BY ENTERING THE DISH'ID NOT BY DISHES NAMES. \n" 
         << "  TO GET DISH'ID GO TO DISPLAY MENU OPTION ONCE DONE WITH MAKING MENU.\n"
         << "  THERE YOU WILL FIND ALL THE DISHES'ID RIGHT NEXT TO DISHES NAMES.\n\n";
    
    for (char loop = '1'; loop == '1' ; )

    {      
            string name;   //dish name
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "\n  Enter the dish(" << getTotalItems() + 1 << ")'s name :  ";
            getline(cin, name);
            
            float price;
            cout << "\n\n  Enter the price for dish(" << name << ")  :  ";
            price = use.StoFconv();

            // insert item into database
            
            string ins = "INSERT INTO menu (dish, price) VALUES ('" + name + "', '" + to_string(price) + "')";
            if (mysql_query(conn, ins.c_str()))
            {
               cout << "Error: " << mysql_error(conn) << endl;
            }
            else
            {
              cout << "\n\tItem Added Successfuly." << endl;
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(3000));
            system("cls");
            
			// more items to add ?
            
            cout << "\n  Enter '0' if done                        Enter '1' to add next dish\n";
            cout <<   "\n\tENTER  :  ";    loop = use.StoCconv();
            
            while (loop != '1' && loop != '0' )
            {
                cout << "\n\n\t  *****   INVALID INPUT   *****\n";
                cout << "\n  Enter '0' if done                        Enter '1' to add next dish\n";
                cout <<   "\n\tENTER  :  ";    
                loop = use.StoCconv();
                system("cls");
                
            } 

    }
}

//------------x---------------x-------------x-------------------x--------------x----------------x----------------x

//------------------ To remove item from menu function -------------------------------------------------------------------



void resturant :: RemoveItem ( void )
{
    for (char loop = '1'; loop == '1';)
    {
            cout << "\n\n  Enter the id number of item you want to change  :  ";
            string id; // dish_id requrired to delete from database
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            getline(cin, id);
            // delete item from database
            string del = "DELETE FROM menu WHERE dish_id = '" + id + "' ";
                if (mysql_query(conn, del.c_str()))
            {
                cout << "\tError: " << mysql_error(conn) << endl;
            }
            
            else
            {
                cout << "\tItem Deleted Successfuly" << endl;
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
            system("cls");

            // repeat or exit
            cout << "\n  Enter '1' to Delete Item                                        Enter '0' to go back\n";
            cout << "\n\t\tENTER  :  ";
            loop = use.StoCconv();

            while (loop != '1' && loop != '0')
            {
                cout << "\n\n\t\t*****   INVALID INPUT   *****\n";
                cout << "  Enter '1' to Delete Item                                     Enter '0' to go back\n";
                cout << "\t\tENTER  :  ";
                loop = use.StoCconv();
                system("cls");
            }
    }
}


//------------x---------------x-------------x-------------------x--------------x----------------x----------------x            


//----------------------MENU DISPLAY-------------------------------

void resturant ::menudisplay(void)
{
  string show = "SELECT * FROM menu";

  if (mysql_query(conn, show.c_str()))
  {
    cout << "Error: " << mysql_error(conn) << endl;
    Sleep(3000);
  }

  else
  {
    MYSQL_RES *res;
    res = mysql_store_result(conn);
    if (res)
    {
        cout << "\n\n\t*****   Good Food, Good Life   *****\n";
        printf("\n\n   %-10s   %-30s   %-10s\n", "ITEM_ID", "ITEM", "PRICE");
        MYSQL_ROW row;

        while (row = mysql_fetch_row(res))
        { 
        printf("   %-10s   %-30s   %-10s\n", row[0], row[1], row[2]);
        }
        
        mysql_free_result(res);
    }

  }
  
}

//------------x---------------x-------------x-------------------x--------------x----------------x----------------x

//---------------------CHANGE PRICE FUNCTION---------------------------

void resturant ::changeprice(void)

{
    for (char loop = '1'; loop == '1';)
    {
        cout << "\n\n  Enter the id number of item you want to change  :  ";
        string id;    // dish_id requrired to update database 
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        getline(cin, id);
        
        string newName;    // new name for item          
        cout << "\n\n  To change dish name, enter new name of dish  :  ";
        getline(cin, newName);
	   
	    cout << "\n\n  To change price, enter new price  :  ";
        string newPrice;   // new price for item
        getline(cin, newPrice);
        
        system("cls");

        // update database
        string upd = "UPDATE menu SET dish = '" + newName + "', price = '" + newPrice + "' WHERE dish_id = '" + id + "'";
        if (mysql_query(conn, upd.c_str()))
        {
        cout << "\tError: " << mysql_error(conn) << endl;
        }
        else
        {
        cout << "\tMenu Updated Successfuly" << endl;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        system("cls");

        // repeat or exit  
        cout << "\n  Enter '1' to update menu                                        Enter '0' to go back\n";
        cout << "\n\t\tENTER  :  ";     
		loop = use.StoCconv();
            
        while (loop != '1' && loop != '0')
        {
        cout << "\n\n\t\t*****   INVALID INPUT   *****\n";
        cout << "  Enter '1' to Update items                                        Enter '0' to go back\n";
        cout << "\t\tENTER  :  ";     loop = use.StoCconv();
        }
    }
}


//------------x---------------x-------------x-------------------x--------------x----------------x----------------x

//---------------------EDIT MENU FUNCTION----------------------------------

void resturant ::editmenu(void)
{

    for (bool loop = true; loop;)
    {
        cout << "\n\n  Enter'0' to GO BACK                                                                  Enter '1' to ADD ITEM in MENU \n " ;
        
        cout <<     "                               Enter '3' to change dish name and / or Price \n";
        
        cout <<     "  Enter '2' to REMOVE ITEM                                                             Enter '4' to DISPLAY MENU \n ";
        
        cout <<     "                                          ENTER  :  ";          char ch = use.StoCconv();
       
 
        system ( " cls " );

        switch (ch)
        {
        case '0':
            loop = false;
            break;
        case '1':
            menumaker();
            break;
        case '2':
            if (getTotalItems())
                RemoveItem();
            else
                cout << "\n\n  ---------   There is no MENU to edit       -------       Follow case '1' to Make MENU  ------\n ";
            break;
        case '3':
            if (getTotalItems())
                changeprice();
            else
                cout << "\n\n  ---------   There is no MENU to edit       -------       Follow case '1' to Make MENU  ------\n ";
            break;
        case '4':
            if (getTotalItems())
                menudisplay();
            else
                cout << "\n\n  --------   There is no MENU to display     -------       Follow case '1' to Make MENU  ------\n  ";
            break;
        default:
            cout << "\n\n  *****   INVALID INPUT   *****\n";
        }
    }
}

//------------x---------------x-------------x-------------------x--------------x----------------x----------------x

//----------------------STRING READER FUNCTION-----------------------

string forcereturn ::String_Reader(int num)
{
    string copy;
    char check;
    bool result;
    switch (num)
    {
    case 1: //string type character
        do
        {
            cin >> copy;
            if (copy.length() != 1)
            {
                cout << "\n\n  *******   Invalid Input   *******\n  -----   Please Enter Again   -----\n";
                cout << "\n  ENTER  :  \n";
                result = false;
            }
            else
                result = true;

        } while (result == false);
        return copy;
        break;

    case 2: // string type integer
        do
        {
            cin >> copy;
            for (int i = 0; i < copy.length(); i++)
            {
                check = copy[i];
                if (isdigit(check) == 0)
                {
                    cout << "\n\n  *****   Invalid Input   *****\n  -----   Please Enter Again   -----\n";
                    cout <<"\n  ENTER  :  \n";
                    result = false;
                    break;
                }
                else
                    result = true;
            }
        } while (result == false);
        return copy;
        break;

    case 3: // string type double or float
        do
        {
            cin >> copy;
            for (int i = 0; i < copy.length(); i++)
            {
                check = copy[i];
                if (isdigit(check) == 0 && check != '.')
                {
                    cout << "\n\n  *******   Invalid Input   *******\n  -----   Please Enter Again   -----\n";
                    cout << "\n  ENTER  :  ";
                    result = false;
                    break;
                }
                else
                    result = true;
            }
            if ((copy.length() == 1) && (copy[0] == '.'))
            {
                copy = "0.00";
                result = true;
            }

        } while (result == false);
        return copy;
        break;

    default:
        return NULL;
    }
}

//------------x---------------x-------------x-------------------x--------------x----------------x----------------x

//---------------------STRING TO UNSIGNED INTEGR CONVERTER---------------------------

unsigned int forcereturn ::StoIconv(void)
{
    string getstring = String_Reader(2);
    stringstream convert(getstring);
    unsigned int paste = 0;
    convert >> paste;
    return paste;
}

//--------------------STRING TO UNSIGNED LONG LONG INTERGER CONVERTER-------------------------

unsigned long long int forcereturn ::StoUILLconv(void)
{
    string getstring = String_Reader(2);
    stringstream convert(getstring);
    unsigned long long int paste;
    convert >> paste;
    return paste;
}

//--------------------STRING TO CHARACTER CONVERTER-------------------------

char forcereturn ::StoCconv(void)
{
    char getCH = String_Reader(1)[0];
    return getCH; // getCH is just a variable with data type char.
}

//---------------------STRING TO DOUBLE CONVERTER------------------------

double forcereturn ::StoDconv(void)
{
    double getD = stod(String_Reader(3));
    getD = round(getD * 100.0) / 100.0;
    return getD;
}

//---------------------STRING TO FLOAT CONVERTER-------------------------

float forcereturn ::StoFconv(void)
{
    float getF = stof(String_Reader(3));
    getF = round(getF * 100.0f) / 100.0f;
    return getF;
}

//------------x---------------x-------------x-------------------x--------------x----------------x----------------x

//-----------------------------   GLOBAL INFORMATION FUNCTOIN   ------------------------------------

    void Information (void)
    {

       cout << " -----  NOTE :: INFORMATION HERE IS GOING TO BE ABOUT HOW TO USE THIS PROGRAM NOT ABOUT HOW TO MAKE CHANGES IN THIS PROGRAM\n" 
            << " \nAt the  very satrting when you will hit your run button you are going to see '5' options and you have to choose one of them (of course).\n" 
            << " \nBut you can't choose option no. '2' & '3' until you set a menu for your resturant becuase both options requires a 'MENU' to do their function\n "
            << "   \n------   A BRIEF INTRODUCTION OF THESE '5' OPTIONS  ----------   \n"
            << " \n     To select an Option you just have to enter the option No.    \n"
            << " \nOption '0' will help you to take exit from program just by choosing option '0' you will be able to exit this program succesfully\n"
            << " \nOption '1' will help you to set and edit your ' RESTURANT MENU '.         \n"
            << " \nOption '2' will help you to take orders but can only be activate by setting a 'MENU'(USING OPTION 1) \n"
            << " \nOption '3' will be for Displaying all the CUSTOMER'S DATA you collected till now but can only be activate by atleast taking one Order(USING OPTION '2')  \n"
            << " \nOption '4' is for the displaying all the information about this program as you are seeing right now.   \n"
            << "\n   -------      Detailed informatoin about Option '1' ( ***  EDIT MENU  *** )    --------  \n"
            << " \nAs soon as you will enter '1' your option '1' will get activate and your blank menu will be generated now you can fill that menu with more of its features which are embedded into more Options   \n"
            << " \n$$$  NOTE $$$ A blank menu can not be considered as a menu until you fill it with atleast one item \n" 
            << " \n--- ADD :: OPTION :: --- will add new items and their price in your menu  $$$$$ IMPORANT $$$$ all of your items are going to be identified by a unique ID named as DISH'ID.\n "
            << " \n--- DISH'ID :: It's a '4 Digit Code' created by system to uniquely identify  each item in your menu----- you can find it inside DISPLAY-MENU OPTION ( '4' ) inisde Set -Menu OPtion( '1' )\n "
            << " \n--- REMOVE :: OPTION '2' :: --- Will remove items from your Menu. To do so you just have to enter the DISH'ID(MENTIONED IN ABOVE POINT) OF THE ITEM you want to remove from menu\n  "
            << " \n--- Change Price :: OPTION '3' :: --- this option will be useful in case you want to change an item name or price and you can do so just by entering DISH'ID and new name or price\n"
            << " \n--- DISPLAY-MENU :: OPTION '4' :: --- this option is for presenting the menu you build so far.\n"     
            << " \n--- TO GO  BACK ::  OPTION '0' :: --- it will help to go one step back(where the last options were present) inside the program \n"
            << " \n$$$  NOTE $$$ It will not exit the program right away it will only help you to take one step back inside the program\n"
            << " \n\n\n  TAKE ORDER FUNCTON : : :  once you choose this option, a order number will be generated and will be equal to '1' after completion of 1st order and will futher increase by '1' for next order\n"
            << " \n  By providing order number you can access all the data of customer by using Dispaly order option.\n" 
            << "\n  ----------------- HOW TO TAKE ORDER --------------\n"
            << "\n to add new items : enter desired dish'id and the quantity \n"
            << "\n to REMOVE items : enter desired dish'id and the quantity = '0'  \n"
            << "\n to CHANGE QUANITY : enter desired dish'id and the quantity = NEW QUANTITY and it will overwrite the old.  \n"
            << " \n  once you done taking order you will be asked for entering customer's details consist of name and phone number.\n"
            << " \n Now, phone number here plays a crucial role let's see in depth : : \n"
            << " \n first of all if a customer don,t have a phone number or phone number entered turns to be invalid than a default phone number will be added instead of the wrong one.\n"
            << "\n VALID PH_NO. is a 10- digit number whose 1st digit is non zero or a 11- digit number whose 1st digit is zero any other number will be count as INAVLID PH_NO.\n"
            << "\n eg: 1122334455 - vaild or 01122334455 - vaild  but 001122334455 - INVALID     and     DEFAULT PH_NO IS : 2323232323\n"
            << "\n Now if a custmor comes with the agian and provide the same details than program will automatically detect and incease the visiting of that person with '1' every time where defalut visitng is '1'  \n"        
            << "\n After done with details a u be be asked for any discount and then total amount of the order will be shown on screen and worker will be asked for talking that amount from custmoe and enter below the amount taken from custmor named as paid amount\n"
            << "\n\n Now,  if custmor decides to quit u can enter amount '0' or u can enetr amount <= total in case of succesful payment but u can't enter 0 < paid < total \n"
            << "\n and if order is free than u can also enter the amount zero but this time data will be stored, without bill \n"
            << "\n also if customer wants to edit order so that his/her price will be in budget than u can also ener '0' this time it will take u back to the order and will asked for the dish'id you want to change\n"
            << "\n to add, remove or change item u just have to enter dish'id and quantity \n"
            << "\n to add new items : enter desired dish'id and the quantity \n"
            << "\n to REMOVE items : enter desired dish'id and the quantity = '0'  \n"
            << "\n to CHANGE QUANITY : enter desired dish'id and the quantity = NEW QUANTITY and it will overwrite the old.  \n"
            << "\n you have to repeat billing process and than you will be done with your order, now you can view data by using DisplayData function. \n";
     
    }




//------------x---------------x-------------x-------------------x--------------x----------------x----------------x

//-----------------------------MAIN BODY------------------------------------

int main()
{

	const char *PW = getPassword().c_str();	
    conn = mysql_init(NULL);

    if (!mysql_real_connect(conn, HOST, USER, PW, DB, 3306, NULL, 0))
    {
        cout << "Error: " << mysql_error(conn) << endl;
    }
    else
    {
        cout << "\tLogged in!" << endl;
    }
    Sleep(1000);
    
    forcereturn use; //forcereturn is a class ---and --- 'use' is its object;
    //Co_Details view;
    
    bool loop = true;
    while(loop)
    {
        cout << "\n\n  Enter '0' to exit                                                                    Enter '1' to set MENU\n ";

        cout <<     "               Enter '4' To get detailed INFORMATION about this program's terms and working \n";

        cout <<     "  Enter '2' to Take Order                                                              Enter '3' to access data\n";

        cout <<     "                                         ENTER  :  ";    char choice = use.StoCconv();

     system( "cls" );

       switch (choice)
        {
        case '0':
            loop = false;
            break;
        case '1':
            order.editmenu();
            break;
        case '2':
            order.takeorder();
            break;
        case '3':
            //order.DisplayData();
            break;
        case '4':
            Information();
            break;    
        default:
            cout <<"\n\n  *****   INVALID INPUT   *****\n";
        }
    }
    return 0;
}

//------------x---------------x-------------x-------------------x--------------x----------------x----------------x
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                 
