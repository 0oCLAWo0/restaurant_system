

/* CODE By : SUMIT AGGARWAL */

#include <iostream> // Input-Output
#include <iomanip>  // for setw
#include <ctype.h>  //isdigit
#include <sstream>  //stringstream
#include <cmath>    //round
#include <string>   //stof,stod
#include <limits>   //numeric_limits
#include <ctime>    // DATE AND TIME
#include <vector>
#include<unordered_map>
#include <cstdio>
#include <iomanip>
#include <chrono>
#include <thread>
#include <windows.h>
#include <mysql.h>
#include <mysqld_error.h>
#include <sstream>
#include<psapi.h>
#include <wininet.h>
#pragma comment(lib, "Wininet.lib")

using namespace std;


const char *HOST = "";     // YOUR OWN HOST : LOCAL OR ANY OTHER  
const char *USER = "";    // AND YOUR USER NAME
const char *DB = "";   // YOU HAVE TO USE YOUR OWN DB 
const char *PW = "";  // AND ITS PASSWORD
MYSQL *conn;

//---------------------------Forward Declarations--------------------------------------------------------------

class restaurant;
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
} use;


//-----------------CLASS RESTAURANT-------------------

class restaurant
{
private:
    // ------ void editmenu(void) will call these 3 functions  -----------
    void menumaker(void);        // helps to make a menu 
    void changeprice(void);    // change price or dish name in the menu
    void RemoveItem ( void ) ;              // remove items from menu
    
     // ------ void takeorder(void) will call these 2 functions  -----------
    void PrintBill(string order_id);            // to print bill/info for 3 cases, 0_customer, 1_admin, -1_customer(free order)
    vector<float> CalculateBill(int orderNo, vector<pair<string, int>> &i_q); // to calculate bill
   
   
    public : 
    void editmenu(void); // to view all the options available for menu - setting
    void placeorder(void);            // take order 
    void DisplayData(void);          // display custmor's data with the help of order number
    void menudisplay(void);     // display the menu you set          
   
}order;


// MYSQL FUNCTIONS ---------->

// ********************   ASK PASSWORD FROM USER TO ACCESS DATABASE **********************

bool checkInternetConnection()
{
    cout << "\t\t\t\tconnecting  ...  please wait  ";
    char url[256];
    strcat(url, "http://www.phpmyadmin.co");
    bool isconnect = InternetCheckConnectionA(url, FLAG_ICC_FORCE_CONNECTION,0);
    if(isconnect)
    {
        //cout<<"\nInternet Connected";
        return 1;
    }
    else
    {
    	system("cls");
        cout<<"\nDISCONNECTED : PLEASE CHECK YOUR INTERNET CONNECTION!!";
        return 0;
    }
    system("cls");
} 
 
// check databse connection .. server down etc 
bool checkDatabaseConnection()
{
	cout << "\t\t\t\tconnecting  ...  please wait  ";
	if (!mysql_real_connect(conn, HOST, USER, PW, DB, 3306, NULL, 0))
    {
        cout << "Error: " << mysql_error(conn) << endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(4000));
        return 0;
    }
    else
    {
        cout << "\t\t\tLogged in!" << endl;
    }
    system("cls");
    return 1;
} 

// check if person is a customer or adim to provide differernt functionalities
bool checkAccessId()
{
  cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');	
  cout <<"\n\n\t\t\tENTER PASSWORD TO CONTINUE : ";
  string pwd;
  getline(cin, pwd);
  
  if(pwd == PW) // same password as of databse
  return 1;
  
  else
  {
  	cout <<"\n\t\t\tACCESS DENIED !!\n";
  	std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    return 0;
  }
  
}  

//// ---------------------GET orderdone Time----------------------------
string getCurrTime()
{
    std::time_t currentTime = std::time(nullptr);
    std::tm timeInfo = *std::localtime(&currentTime);

    std::ostringstream formattedTime;
    formattedTime << std::setfill('0')
                  << std::setw(2) << timeInfo.tm_hour << ":"
                  << std::setw(2) << timeInfo.tm_min << ":"
                  << std::setw(2) << timeInfo.tm_sec;

    return formattedTime.str();
}


//// ---------------------get orderdone Date ----------------------------

string getDate() {
    auto now = std::chrono::system_clock::now();
    auto time_t_now = std::chrono::system_clock::to_time_t(now);

    std::tm* tm_now = std::localtime(&time_t_now);

    std::ostringstream oss;
    oss << std::put_time(tm_now, "%Y-%m-%d");

    return oss.str();
}

////  ***********************   TOTAL NUMBER OF ITEMS IN MENU   *********************

int getTotalItems(){
    string show = "SELECT dish_id FROM menu"; // ALL DISTINCT DISH ID FROM MENU
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
            int totalItems = mysql_num_rows(res);  // COUNT TOTAL ROWS OF DISTINCT DISH IDS == TOTAL ITEMS
            mysql_free_result(res); 
            return totalItems;
        }
    }
    return 0;
}

//  ***********************   TOTAL NUMBER OF ORDERS ON DATE YYYY-MM-DD   *********************
int getOrderNo()
{
    string date = getDate();  // today's date
    string show = "SELECT order_id FROM customers WHERE date = ' " +date+ " '";  // all orders taken today

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
            int orderNo = mysql_num_rows(res);
            mysql_free_result(res);  
            return orderNo;  // orderNo = all orders taken today + 1 as orderNo starts from 0 daily
        }
    }
    return 0;
}

//  ***********************   GET DISH_ID FROM MENU FROM DISH NAME  *********************
string getDishId(string name)
{
    string show = "SELECT dish_id FROM menu WHERE dish = '" + name + "'";
    
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
            {
            	mysql_free_result(res);  
            	return row[0];   // return dish id of dish(name);
	    }
        }
    }
    return "";
}

//  ***********************   SEARCH if DISH is present in MENU   *********************
bool searchDishInMenu(string dish_name)
{
    string show = "SELECT dish FROM menu WHERE dish = '" +dish_name+ "'";

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
            mysql_free_result(res); 
            
	   if(row && row[0])    // row[0] exist means there is a dish name at row[0] means dish is present   
	   return true;
        }
    }
    return false;
}

//  ***********************   SEARCH order_id is present in order   *********************
bool searchOrderIdInOrder(string order_id)
{
    string show = "SELECT order_id FROM orders WHERE order_id = '" +order_id+ "'";

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
            mysql_free_result(res); 
            
	   if(row && row[0])       // row[0] exist means there is a order id  at row[0] means order id is present   
	   return true;
        }
    }
    return false;
}


//  ***********************  PRINT ORDER ITEM FOR BILL  *********************
void printOrderItem(string order_id)
{
  string show = "SELECT dish, quantity, price FROM orders WHERE order_id = '"+ order_id +"'";

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
       printf("\n\n   %-10s   %-30s   %-10s\n\n\n", "ITEM", "QUANTITY", "PRICE");
       MYSQL_ROW row;
       
       while (row = mysql_fetch_row(res))
       { 
       printf("   %-10s   %-30s   %-10s\n", row[0], row[1], row[2]);
       }  
      
        mysql_free_result(res);
     }
   }
}

//   *********************  GET PRICE FOR DISHES FROM DATABSE - TABLE - menu  ********************
string getPrice(string dish_name)
{
    string show = "SELECT price FROM menu WHERE dish = '" + dish_name + "'";

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
            mysql_free_result(res);
            
	    if(row && row[0])  // return price for dish from menu
            return row[0];
        }
    }
    
    return "0";
}

//   *********************  GET TOTAL PRICE OF ORDER  ********************
float getTotalOrderAmount(vector<pair<string, int>> &i_q)
{
    float sum = 0.00;
    for(auto order : i_q)  // I_Q = {ITEM_NAME,  QUANTITY} == LENGTH == ALL DISH ORDERED
    {
        int quantity = order.second;
        string dish_name = order.first;

        sum += stof(getPrice(dish_name)) * quantity;  // GET PRICE FOR A DISH AND THEN MULTIPLY BY QUANTITY DO THIS FOR ALL DISH ORDERED
    }
    return sum;
}

//   *********************  UPDATE ORDER DISHES TO DATABSE - TABLE - orders  ********************
void updateMenu(string id, string newName, float newPrice)
{
    string upd = "UPDATE menu SET dish = '" + newName + "', price = '" + to_string(newPrice) + "' WHERE dish_id = '" + id + "'";  
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
}


//   *********************  INSERT ORDER ITEM TO DATABSE - TABLE - orders  ********************
void insertItemsToOrder(string order_id, vector<pair<string, int>> &i_q)
{
	
    // Begin transaction
    if (mysql_query(conn, "START TRANSACTION"))  // TRANSACTION : INSERT EITHER ALL ORDER ITEMS OR NONE(IF ONE OR MORE ITEM PRODUCE ERROR)
    {
        cout << "\t\tError: " << mysql_error(conn) << endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(7000));
        return ;
    }

    bool success = true;

    // Insert data using a loop
    for (auto order : i_q)  // I_Q = {ITEM_NAME,  QUANTITY}
    {   
	if(order.second == 0)  // IF QUANTITY OF ANY DISH ORDERED IS 0 THEN DON'T ADD IT INTO ORDER AND DATABASE
	continue;
		  
        string name =  order.first;
        string quantity = to_string(order.second);
        string dish_id = getDishId(name);
        string price = to_string(stof(getPrice(name))*order.second);
        
        string query = "INSERT INTO orders (order_id, dish_id, dish, quantity, price) VALUES ('" + order_id + "', '" + dish_id + "', '" + name + "', '" + quantity + "', '" + price + "')";

        if (mysql_query(conn, query.c_str()))
        {
        cerr << "Insert failed: " << mysql_error(conn) << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(3000));
        
        success = false;  // IF ONE OR MORE DISHES GIVES ERROR ON ENTERING DATABASE THEN DON'T ADD ANY OF THE ORDER ITEM AT ALL INTO THE DATABASE
        break; // Exit loop on first error
        }
    }

    // Commit or rollback transaction based on success
    if (success)
    {
        if (mysql_query(conn, "COMMIT"))
        {
        std::cerr << "Commit failed" << std::endl;
        }
        else
        {
        std::cout << "\t\t\tORDER TAKEN" << std::endl; // ORDER INSERTED SUCCESSFULLY
        }
    }
    else // ROLLBACK CURRENT INSERT ITEMS FROM DATABASE IF SUCCESS IS FALSE
    {
        if (mysql_query(conn, "ROLLBACK"))
        {
        std::cerr << "Rollback failed" << std::endl;
        }
        else
        {
        std::cerr << "invalid Item, rolling back" << std::endl;
        }
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    system("cls");
}

// insert item into menu
            
void insertItemToMenu(string name, float price)
{
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
}            

//   *********************  INSERT DETAILS ITEM TO DATABSE - TABLE - customer  ********************
void insertCustomerDetails(string order_id, string contact, string name, string time)
{
    string date = order_id.substr(0, 10);
    string ins = "INSERT INTO customers (order_id, contact, name, date, time) VALUES ('" + order_id + "', '" + contact + "', '" + name + "', '" + date + "', '" + time +"')";

    if (mysql_query(conn, ins.c_str()))
    {
        cout << "\t\tError: " << mysql_error(conn) << endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(7000));
    }
    else
    {
        cout << "\t\tPROCESSING: " << endl;   // processing of next step insert bill details into databse
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
        
    }
    system("cls");
}

//   *********************  GET CUSTOMER DETAILS FROM DATABSE  ********************
vector<string> getCustomerDetails(string order_id)
{
    string show = "SELECT name, contact, time FROM customers WHERE order_id = '" + order_id + "'";

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
        mysql_free_result(res);

        vector<string> details = {row[0], row[1], row[2]};
        return details;
        }
    }

    return {};
}

//   *********************  INSERT BILL DETAILS ITEM TO DATABSE - TABLE - bill  ********************
void insertBillDetails(string order_id, vector<string> &bill)
{
    //VECTOR BILL [5] = DISCOUNT, TAX, TOTAL, PAID, CHANGE; 	
    string ins = "INSERT INTO bill (order_id, total, discount, paid, change_amount, tax) VALUES ('" + order_id + "', '" + bill[2] + "', '" + bill[0] + "', '" + bill[3] + "', '" + bill[4] + "', '" + bill[1] + "')";

    if (mysql_query(conn, ins.c_str()))
    {
        cout << "\t\tError: " << mysql_error(conn) << endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(7000));
    }
    else
    {
        cout << "\t\tPROCESSING: " << endl;  // processing for next step --- insert items of order into database
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
    }
    system("cls");
}

//   *********************  GET bill DETAILS FROM DATABSE  ********************
vector<string> fetchBill(string order_id) // TO GET BILL DATA OF THE CUSOMER
{
    string show = "SELECT total, paid, change_amount, tax, discount FROM bill WHERE order_id = '" + order_id + "'";

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
        mysql_free_result(res);

        vector<string> bill = {row[0], row[1], row[2], row[3], row[4]};
        return bill;
        }
    }

    return {};
}

//   *********************  DELETE Item From MENU ********************
void deleteItemFromMenu(string id)
{
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
}


//-----------------Calculate Bill Functon---------------------------

vector<float> restaurant :: CalculateBill(int orderNo, vector<pair<string, int>> &i_q)
{  
    float discount = 0;      // I had some other plans at first but its okay      
    float tax_charge = 18.00; // 18 percent tax charges
    float total = 0;          // total bill 
    float paid;               // amount paid by customer
    float change;            // change amount return to customer

    // -----------------x---------------x------------------x---------------x-----------  calculation of total price
    total = getTotalOrderAmount(i_q);      // sum of all (price*quantity);
    if(total < 0)
    return {};
    
    //getDiscount(); not implemented
	//total = total - (((total) * (discount)) / 100.00f);  // total = total - discount_charges
    total = total + (((total) * (tax_charge)) / 100.0f); // total = total + tax_charges
    cout << "\n\n\t\tTotal Price:  " << total << "rs. " << endl;
    
    // -----------------x---------------x------------------x---------------x-----------   amount paid by cusomer is acceptable or not
    
    cout << "\n\t\tEnter the amount paid by custmor:   ";   
    paid = use.StoFconv();
    system("cls");
    
    while (paid < total)  // if paid amount is less than payable amount
    {
        cout << "\n\n\t\t!!!   Sorry   !!!    The amount is not enough for your order. \n";
        cout << "\n\t\tEnter enough amount to contiune OR\t\tEnter amount '0' to cacel order   -----  \n ";
        cout << "\n\n\t\tTotal Price:  " << total << "rs.\t\tENTER:  ";    
	
	paid = use.StoFconv(); //price paid by custmor
	system("cls");	
	if(paid < total && paid == 0)
	return {};	
    }
    
    // -----------------x---------------x------------------x---------------x-----------  

    change = (paid) - total;
    cout << "\n\n\t\tChange ( the amount working employee has to return )  :   " << change << "rs. " << endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    
    
    system("cls");
    vector<float> bill = {discount, tax_charge, total, paid, change};
    return bill;
}

////------------x---------------x-------------x-------------------x--------------x----------------x----------------x


//-----------------------Print Bill Function------------------------

void restaurant :: PrintBill(string order_id) 
{ 
	auto details = getCustomerDetails(order_id); // name, contact, frequency_visit;
    
    if(details.empty()) 
    return;
     
    int orderNo = stoi(order_id.substr(11));
    cout << "\n\n\n\t\t!!!!!!!!   SUMIT AGGARWAL aka CODE OWNER.RESTURANT.Details   !!!!!!!!\n";
    cout << "\n\t\t\t!!!!!!   OrderNo :: [" << orderNo << "]   !!!!!!! \n";

    
    cout << "\n------------   CUSTOMER'S DETAILS   ----------\n";
    cout << "\n    Phone number : " << details[1] << endl;
    cout << "\n    Name : " << details[0] << endl;
    cout << "\n-------------       ORDER      ----------------\n";


    printOrderItem(order_id);
    auto bill = fetchBill(order_id);

    if(bill.empty())
    return;

    cout << "  ---------------------------------------------------------\n";
    cout << "\n       Total         :  " << bill[0]   << " rs.\n";
    cout << "\n       Paid          :  " << bill[1] << " rs.\n";
    cout << "\n       Change        :  " << bill[2] << " rs.\n";
    cout << "\n       Tax Charges   :  " << bill[3] << " %\n";
    cout << "\n       Discount      :  " << bill[4] << " %\n";
    cout << "---------------------------------------------------------\n";
    
    cout << "\n     ######   WE HOPE YOU ENJOY YOUR MEAL   ######\n";
    string time = details[2];
    cout <<"\n\t\t"<<order_id.substr(0,10)<<" "<<time<<endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(5000));
}

//------------x---------------x-------------x-------------------x--------------x----------------x----------------x

//------------------------TAKE ORDER FUNCTION----------------------------

void restaurant :: placeorder(void)
{
    int orderNo = getOrderNo() + 1;  // orderNo
    string order_id;                 // order_id (unqiue for everyorder) = date + order_no;
    bool details = false;   // customer details are stored -- not yet
    string name;           // custmor name
    string contact;        // contact;
    int cancel_0 = 1;          // to track loop
    string time;
    vector<pair<string, int>> item_quantity;
    unordered_map<string, int> track; // keeps tracks of dishes and there index number in vector item_quantity to edit/change
    int index = 0; //index to be put inot unorder_map

    if (!getTotalItems())
    {
      cout << "\n\t\t  !!!!!!   SORRY   !!!!!\n\t\t  **********   NO ITEM TO ORDER   **********\n\n";
	  return;    
    }
    
 while (cancel_0 == 1) 
 {  
    char loop = '1'; // Initialize loop condition for entering the loop
    for (; loop == '1';) 
    {
        cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        // Prompt user to enter the dish name
        cout << "\n\t\tEnter dish: ";
        string dish_name;
        getline(cin, dish_name);

        while (!searchDishInMenu(dish_name)) 
        {
            system("cls");
            cout << "\n\t\t ## THIS DISH IS NOT PRESENT IN MENU ##\n\n\n\t\tENTER AVAILABLE DISH NAME TO CONTINUE OR ENTER '0' TO CANCEL ORDER\n\n";
            cout << "\t\tENTER: ";
            getline(cin, dish_name);

            if (dish_name == "0")
            return; // Exit if user chooses to cancel the order
        }

        // Prompt user to enter the quantity of the dish
        cout << "\n\t\tEnter Quantity: ";
        int quantity = use.StoIconv();

        system("cls");

        if (track.count(dish_name) > 0) 
        {
            // Update quantity if the dish is already in the order
            item_quantity[track[dish_name]].second = quantity;
        } 
        else 
        {
            // Add the dish and quantity to the order
            item_quantity.push_back({ dish_name, quantity });
            track[dish_name] = index++;
        }

        // Prompt user for further action: add more items, cancel order, or complete order
        cout << "\n\n\t\tENTER '1' TO ADD MORE ITEMS\t\tENTER '0' IF DONE\n";
        cout << "\n\t\tENTER 'C' TO CANCEL ORDER\t\t  ENTER: "; loop = use.StoCconv();
        system("cls");

        while (loop != '1' && loop != '0' && loop != 'c' && loop != 'C') 
        {
            // In case of invalid input, prompt user until valid input is received
            cout << "\n\n\t\t\t*****   INVALID INPUT   *****\n\n";
            cout << "\n\n\t\tENTER '1' TO ADD MORE ITEMS\t\tENTER '0' IF DONE\n";
            cout << "\n\t\t\tENTER 'C' TO CANCEL ORDER\t\t  ENTER: "; loop = use.StoCconv();
            system("cls");
        }
    } // for loop

    if (loop == '0') 
    {
      if (!details) 
      {
        // Get customer's name
        bool isValidName = false;
        cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        while (!isValidName) 
	    {
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
            system("cls");

            cout << "\n\n\t\t  Enter Customer's Name :  ";
            getline(cin, name);

            if (name.size() > 30) 
	        {
                cout << "\n\t\tNAME TOO LONG: MAX LENGTH 30 CHARACTERS\n\n";
                continue;
            }
            if (name.size() == 0) 
	        {
                cout << "\n\t\tNAME TOO SHORT: MIN LENGTH 1 CHARACTER\n\n";
                continue;
            }
            isValidName = true;
        }

        // Get customer's contact
        bool isValidContact = false;
        while (!isValidContact) 
	    {
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
            system("cls");
            cout << "\n\n\t\t  Enter Customer's Contact :  ";
            getline(cin, contact);

            if (contact.size() > 14) 
	        {
                cout << "\n\t\tINVALID CONTACT\n\n";
                continue;
            }
            isValidContact = true;
        }

        system("cls");
	    details = true;
      }// IF --- DETAILS FILLED	      
        
     // Calculate the bill and handle order cancellation -------------------------------------------->
     auto temp = CalculateBill(orderNo, item_quantity);
     if (temp.empty()) 
     {
        cout << "\t\tORDER CANCELLED\n";
        return;
      }

      // Convert the bill to string for database storage
      vector<string> bill;
      for (auto i : temp)
      bill.push_back(to_string(i));

      // Prompt user for further action: cancel order, edit order, or continue
      cout << "\n\n\t\tEnter '0' to cancel order\t\t\tEnter '1' to edit order\n";
      cout << "\n\n\t\ENTER ANY OTHER 'NUMBER' TO CONTINUE\t\tENTER: ";
      cancel_0 = use.StoIconv();
      
      switch (cancel_0) 
      {
            case 0: // Case to cancel order
                return;

            case 1: // Case to edit order
                cout << "\n\n       *************    You can CHANGE, DELETE OR ADD ITEM BY SIMPLY ENTERING THE DISH NAME AND QUANTITY   ***********\n ";
                break;

            default: // Print bill on any default case
                order_id = getDate() + " " + to_string(orderNo);
                time = getCurrTime();
                insertCustomerDetails(order_id, contact, name, time);
                insertItemsToOrder(order_id, item_quantity);
                insertBillDetails(order_id, bill);
                PrintBill(order_id);
       }//SWITCH
	
    } // If loop condition
	 
 }//while customer wants to edit order keep going to step 1        
       
}//takeorder

//------------x---------------x-------------x-------------------x--------------x----------------x----------------x



//-----------------DISPLAY DATA FUNCTION---------------------

void restaurant :: DisplayData(void)
{
        cout << "\n\t\tEnter Order Number  :  ";           
	string orderNo;
	getline(cin, orderNo);
		
	string date;
	cout << "\n\t\tEnter Order Date IN  FORMAT (YYYY-MM-DD)  :  ";
	getline(cin, date);
        
        string order_id = date + " " + orderNo;
       
	    if(searchOrderIdInOrder(order_id)) 
        PrintBill(order_id);
		      
        else
        {
          cout << "\n\t\t THERE IS NO ORDER NO:  "<<orderNo<<"  ON DATE:  "<<date<<endl;
          std::this_thread::sleep_for(std::chrono::milliseconds(1200));
          return;
	    }
        
        cout << "\n\n\nGOING BACK AFTER 12 SECONDS ... \n";
        std::this_thread::sleep_for(std::chrono::milliseconds(12000));
        
        system("cls");
}

////------------x---------------x-------------x-------------------x--------------x----------------x----------------x

//----------------MENU-MAKER FUNCTION----------------------

void restaurant ::menumaker(void)
{
  for (char loop = '1'; loop == '1' ; )
  {      
    string name;   //dish name
    system("cls");
    cout << "\n\n\t\t  ENTER DISH NAME  :  ";
    cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    getline(cin, name); 
    
    if(name.size() > 30)  // maximum capacity in databse 30 character
    {
      cout << "\n\t\tNAME TOO LONG : MAX LENGTH 30 CHARACTERS"<<endl;
      return;
    }
				
    if(name.empty())
    {
      cout << "\n\t\tNAME TOO SHORT : MIN LENGTH 1 CHARACTER"<<endl;
      return;
    }
    
    if(searchDishInMenu(name))
    {
        cout << "\n\t\tITEM ALREADY PRESENT IN MENU"<<endl;                                          
	return;		
    }  
				
    float price;
    cout << "\n\n  Enter the price for dish(" << name << ")  :  ";
    price = use.StoFconv();
    
    insertItemToMenu(name, price); // insert into databse
            
   // more items to add ?
   cout << "\n  Enter '0' if done                        Enter '1' to add next dish\n";
   cout <<   "\n\tENTER  :  ";    
   loop = use.StoCconv();
            
   while (loop != '1' && loop != '0' )
   {
     cout << "\n\n\t  *****   INVALID INPUT   *****\n";
     cout << "\n  Enter '0' if done                        Enter '1' to add next dish\n";
     cout <<   "\n\tENTER  :  ";    
     loop = use.StoCconv();
     system("cls");
    } 

  }//for
}// function  

//------------x---------------x-------------x-------------------x--------------x----------------x----------------x

//------------------ To remove item from menu function -------------------------------------------------------------------



void restaurant :: RemoveItem ( void )
{
    for (char loop = '1'; loop == '1';)
    {
            cout << "\n\n  Enter the item name you want to remove  :  ";
            string name; // dish_name requrired to delete from database
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            getline(cin, name);
            
            
            string id = getDishId(name);   // fetch dish_id using dish name
            if(id == "")
            {
            	cout <<"\n\t\tITEM NOT FOUND IN THE MENU";
            	return;
	    }
            
            deleteItemFromMenu(id);

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

void restaurant ::menudisplay(void)
{
  string show = "SELECT * FROM menu";

  if (mysql_query(conn, show.c_str()))
  {
    cout << "Error: " << mysql_error(conn) << endl;
    Sleep(3000);
    return;
  }
  
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
 
 cout << "\n\n\n\n\t\tGOING BACK AFTER 12 SECONDS ... \n"; 
 std::this_thread::sleep_for(std::chrono::milliseconds(12000));
  
}

//------------x---------------x-------------x-------------------x--------------x----------------x----------------x

//---------------------CHANGE PRICE FUNCTION---------------------------

void restaurant ::changeprice(void)

{
    for (char loop = '1'; loop == '1';)
    {
        cout << "\n\n  Enter the item you want to change  :  ";
        string oldName;    // dish_name requrired to update database 
        cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        getline(cin, oldName);
        
        string id = getDishId(oldName);  //fetch dish_id from database whose dish_name is oldName
        if(id == "")
        {
         cout << "\n\t\tNO ITEM FOUND FOR DISH NAME = "<<oldName<<endl;
         return;
	}
        system("cls");
	string newName;    // new name for item
	cout << "\n\n\t\t  ENTER NEW DISH NAME  :  ";
	getline(cin, newName);
			
	if(newName.size() > 30)
	{
	  cout << "\n\t\tNAME TOO LONG:  MAX LENGTH 30 CHARACTERS\n\n";	
	  return;
	}
	
	if(newName.size() == 0)
	{
	  cout << "\n\t\tNAME TOO SHORT:  MIN LENGTH 1 CHARACTERS\n\n";
	  return;	
	}
			
	if(newName != oldName && getDishId(newName) != "")
	{
	  cout << "\n\n\t\tOTHER ITEM HAVE SAME NAME CANNOT CONTINUE";
	  return;
	}
	cout << "\n\n\t\tENTER NEW PRICE:  ";
        float newPrice = use.StoFconv();   // new price for item
        system("cls");

        // update database
        updateMenu(id, newName, newPrice);

        // repeat or exit  
        cout << "\n\t\tEnter '1' to Update items\t\tEnter '0' to go back\n";
        cout << "\n\t\tENTER  :  ";     loop = use.StoCconv();     
            
        while (loop != '1' && loop != '0')
        {
        cout << "\n\n\t\t*****   INVALID INPUT   *****\n";
        cout << "\n\t\tEnter '1' to Update items\t\tEnter '0' to go back\n";
        cout << "\n\t\tENTER  :  ";     loop = use.StoCconv();
        }
     
    }
}


//------------x---------------x-------------x-------------------x--------------x----------------x----------------x

//---------------------EDIT MENU FUNCTION----------------------------------

void restaurant ::editmenu(void)
{

    for (bool loop = true; loop;)
    {
        cout << "\n\n  Enter '0' TO GO BACK             \t\t\tEnter '1' to ADD ITEM in MENU \n " ;
        cout << "\n\n  Enter '2' TO REMOVE ITEM         \t\t\tEnter '3' to DISPLAY MENU \n ";
        cout << "\n\n  ENTER '4' TO UPDATE EXISTING ITEM\t\t\tENTER: ";          char ch = use.StoCconv();
       
 
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
        case '4':
            if (getTotalItems())
                changeprice();
            else
                cout << "\n\n  ---------   There is no MENU to edit       -------       Follow case '1' to Make MENU  ------\n ";
            break;
        case '3':
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
                cout << "\n\t\t  ENTER  :  ";
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
                    cout <<"\n\t\t  ENTER  :  ";
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
                    cout << "\n\t\tENTER  :  ";
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

void viewOptions(char choice, bool &loop)
{
   switch (choice)
    {
        case '0':
            loop = false;
            break;
        case '1':
        	if(checkAccessId())
            order.editmenu();
            break;
        case '2':
            order.placeorder();
            break;
        case '3':
        	if(checkAccessId())
            order.DisplayData();
            break;
        case '4' :
		    order.menudisplay();
			break;    
        default:
            cout <<"\n\n  *****   INVALID INPUT   *****\n";
    }	
}



//-----------------------------MAIN BODY------------------------------------

int main()
{    
    while(!checkInternetConnection); // connect to internet than continue	 
    conn = mysql_init(NULL);       // establish connection
    while(!checkDatabaseConnection()); // connect to database
    
    bool loop = true;
    while(loop)
    {
    	system("cls");
        cout << "\n\n  Enter '0' to Exit         \t\t\tEnter '1' to EDIT MENU\n ";
        cout << "\n\n  Enter '2' to Take Order   \t\t\tEnter '3' to access data\n";
        cout << "\n\n  Enter '4' to Display Menu \t\t\tENTER  :  "; 
	char choice = use.StoCconv();
        system( "cls" );
        viewOptions(choice, loop);
    }
    
    mysql_close(conn);
    return 0;
}

//------------x---------------x-------------x-------------------x--------------x----------------x----------------x
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                 
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                      
