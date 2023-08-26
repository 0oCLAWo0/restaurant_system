

/* CODE By : SUMIT AGGARWAL */

#include <iostream> // Input-Output
#include <iomanip>  // for setw
#include <ctype.h>  //isdigit
#include <sstream>  //stringstream
#include <cmath>    //round
#include <string>   //stof,stod
#include <limits>   //numeric_limits
#include <ctime>    // DATE AND TIME
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

class Co_Details

{

private:
    friend class resturant;

    forcereturn use; //differnt class with object

    string Co_name;
    unsigned short int itemcount; // to store total number of item ordered for each order
    long long int Co_phonenumber;  // to store customer's phone number
    unsigned short int Co_visit;   // To store the custmore frequrnt visit
    unsigned short int Co_OrderNo; // To store every order no.
    float discount, tax_charge, total = 0.00 /* total amount which custmor have to pay */, paid /* amount paid by custmor */, change /*amount return by worker */;

    
    string storetime; // to store a particular time

    void SaveOrderNo(unsigned short int i);  // function to assign order no to co_orderno
    void SaveName(void);                     // assign name to Co_name member
    void SavePhoneNo_(void);                // assign phonenumber to co_phonenumber
    void Save_O_DateTime(void);             // display the current time and also store each diplay  "time" in different strings for every case 
    void Display_O_DateTime(void);          //  display the stored time (not current time)
    void CalculateBill(unsigned short int accept); // it will calulate the bill 
    void PrintBill(unsigned short int accept, bool dis); // it will print the bill
    inline void Count_Co_Visit(void) { Co_visit++; }; //functioning of function Count_Co_Visit

public:
    Co_Details() : Co_visit(0), itemcount(0) {}  // Constructor
};

//------------x---------------x-------------x-------------------x--------------x----------------x----------------x

//----------------Save-Name Function-----------------

inline void Co_Details ::SaveName(void)
{
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    getline(cin, Co_name);
}

//------------x---------------x-------------x-------------------x--------------x----------------x----------------x

//----------------Save-Phone Number Function-------------------

inline void Co_Details ::SavePhoneNo_(void)
{
    Co_phonenumber = use.StoUILLconv();
    if (Co_phonenumber < 1000000000 || Co_phonenumber > 9999999999)
        Co_phonenumber = 232323232323;
}

//------------x---------------x-------------x-------------------x--------------x----------------x----------------x

//-----------------CLASS RESTURANT-------------------

class resturant
{
private:
    friend class Co_Details;
    // friend void Co_Details ::SaveOrderNo(unsigned short int i);
    // friend void Co_Details ::CalculateBill(unsigned short int accept);

    forcereturn use;          // differnt class and its object
    Co_Details Details[1000]; // differnt class and its object

    static float price[50];       // store price for each item in the menu 
    static string dishes[50];     // store item name for each item in the menu
    static unsigned short int OrderNo;  // the OrderNO count till now (only store one value and rewrite that value when order no changes)  
    static unsigned int Customer_Count; // total number of custmors till now (repeated customers will be counted as one)
    static unsigned short int counter;  // total number of items in the menu

    unsigned short int quantity;        // how many quantity custmor wants for a particular item
    unsigned short int order_id[1000][50]; // store DISH'ID for every order with each item in that order :::: order_id[x][y] === x---> is order no. and y----> is item number in that particular order
    unsigned short int storequantity[1000][50]; // store quantity for every order with its each items 

    void menumaker(void);        // helps to make a menu 
    void menudisplay(void);     // display the menu you set
    void changeprice(void);    // change price or dish name in the menu
 //   void privateaccess(void);  // 

public:
    unsigned int dish_id = 0;
    void editmenu(void);             // to view all the options available for menu - setting
    void takeorder(void);            // take order 
    void DisplayData(void);          // display custmor's data with the help of order number 
   // void DisplayStatus(void);         
    void SaveOrder_id(unsigned short int save);   // assign dish'id to  order'id[x][y](above) 
    void RemoveItem ( void ) ;              // remove items from menu

} order;

//------------x---------------x-------------x-------------------x--------------x----------------x----------------x

//----------------Save Order NUmber Function-----------------------

inline void Co_Details ::SaveOrderNo(unsigned short int i)
{
    resturant ob;
    Co_OrderNo = ob.OrderNo;
}

//------------x---------------x-------------x-------------------x--------------x----------------x----------------x

// ---------------------Save orderdone Date and Time----------------------------

inline void Co_Details ::Save_O_DateTime(void)
{
    time_t ttime = time(0);
    char* datetime = ctime(&ttime) ;
    cout << "\n  Date and Time(day mm dd hours:min:sec yy) is: " << datetime << endl;
    storetime = datetime;
    
}

//------------x---------------x-------------x-------------------x--------------x----------------x----------------x

//--------------------Display order done Date and Time-----------------------------

inline void Co_Details ::Display_O_DateTime(void)
{
    cout << "\n  Date and Time(day mm dd hours:min:sec yy) was: " << storetime << endl;
}

//------------x---------------x-------------x-------------------x--------------x----------------x----------------x

//-----------------Calculate Bill Functon---------------------------

void Co_Details ::CalculateBill(unsigned short int accept)
{

    cout << "\n  Enter Discount like the below example: \n  ----  Enter 20 for '20%' discount  ----  \n";
    cout << "\n  Enter Discount :   ";  discount = use.StoFconv();
  
    while (discount >= 100.00f)
    {
        cout << "\n\n  *****   invalid input   ***** \n  Enter valid discount ( 0 <= Discount < 100 )\n";

        cout << "\n  Enter Discount :   ";  discount = use.StoFconv(); // how much discount u want to provide like 20 = 20percent not 20rs
    }

    tax_charge = 18.00; // 18 percent tax charges

    total = 0;

    for (int loop = 0; loop < itemcount; loop++)
    {
        total = total + ((order.price[order.order_id[accept - 1][loop] - 1001]) * (order.storequantity[accept - 1][loop])); // total = 0(total) + price*quantity
    }

    total = total - (((total) * (discount)) / 100.00f);  // total = total - discount_charges
    
    total = total + (((total) * (tax_charge)) / 100.0f); // total = total + tax_charges
    
    cout << "\n\n  Total Price:  " << float ( total  ) << "rs. " << endl;
    
    cout << "\n Enter the amount paid by custmor:   ";   paid = use.StoFconv();
    

    while ((paid) != 0.00f && (paid) < ( float (total) ) )
    {
        cout << "\n\n  !!!   Sorry   !!!  The amount is not enough for your order. \n";
        cout << "\n  -----   Enter enough amount to contiune   -----  OR  -----   Enter amount '0' to view more options   -----  \n ";

        cout << "\n  ENTER :  ";    paid = use.StoFconv(); //price paid by custmor
        
        cin.ignore(numeric_limits<streamsize>::max() );
    }

    if ((paid) >= ( float ( total ) ))

    {
        change = (paid) - ( float (total) );
        cout << "\n\n  Change ( the amount worker has to return )  :   " << change << "rs. " << endl;
    }
}

//------------x---------------x-------------x-------------------x--------------x----------------x----------------x

//-----------------------Print Bill Function------------------------

void Co_Details ::PrintBill(unsigned short int accept, bool dis) // accept = order no. && bool dis will be for deciding whether to print or not this detail

{

    cout << "\n\n\n      !!!!!!!!   SUMIT AGGARWAL aka CODE OWNER.RESTURANT.Details   !!!!!!!!\n";
    cout << "\n                    !!!!!!   OrderNo :: [" << accept << "]   !!!!!!! \n";

    if (dis == true) // only print these details for resturant (store data)  ,,,, don't print them on bill

    {
        cout << "\n        !!!!!!  Total custmor till now =  -- " << order.Customer_Count << " --   !!!!!!! \n\n";
        ;

        cout << "\n  ---------   CUSTOMER'S DETAILS   ----------\n";
        cout << "\n  Customer's frequent visit = " << Co_visit << endl;
        cout << "\n  Phone number : " << Co_phonenumber << endl;
    }

    cout << "\n  Name : " << Co_name << endl;

    cout << "\n  -------------   ORDER   -------------\n";
    cout << "\n  DISH " << setw(13) << ": "
         << "  Q(n)      :   Price*Q(n) rs.\n\n";

    for (int loop = 0; loop < itemcount; loop++)

    {

        if (dis == true || order.storequantity[accept - 1][loop])

        {

            cout << "\n  " << order.dishes[order.order_id[accept - 1][loop] - 1001] << setw(20 - (order.dishes[order.order_id[accept - 1][loop] - 1001].length()));
            cout << ": Q(" << order.storequantity[accept - 1][loop] << ")      :   ";
            cout << ((order.price[order.order_id[accept - 1][loop] - 1001]) * order.storequantity[accept - 1][loop]) << " rs.\n";
        }
    }

    cout << "  ---------------------------------------------------------\n";
    cout << "\n  Total         :  " << total   << " rs.\n";
    cout << "\n  Paid          :  " << paid << " rs.\n";
    cout << "\n  Change        :  " << change << " rs.\n";
    cout << "\n  Tax Charges   :  " << tax_charge << " %\n";
    cout << "\n  Discount      :  " << discount << " %\n";
    cout << "---------------------------------------------------------\n";

    if (dis == true)
        Display_O_DateTime();

    else

    {

        cout << "\n     ######   WE HOPE YOU ENJOY YOUR MEAL   ######\n";

        Save_O_DateTime();
    
    }
}

//------------x---------------x-------------x-------------------x--------------x----------------x----------------x

//------------------------TAKE ORDER FUNCTION----------------------------

void resturant ::takeorder(void)

{

    if (counter == 0)
    {
        cout << "\n  !!!!!!   SORRY   !!!!!\n\n  **********   First You have to set a MENU to Take Orders   **********\n\n  ------   To set MENU follow these options   ------\n";
    }
    else
    {
        Details[OrderNo - 1].itemcount = 0;

        int cancel_0 = 10; // default case for switch

        cout << "\n  !!!!!!   Order No [" << OrderNo << "]    !!!!!!! \n";

        do
        {

            char loop = '1'; // condition for --for loop

            for (; loop == '1';)

            {

                cout << "\n  Enter id num of the dish  :  ";     dish_id = use.StoIconv();
                
                bool match = true; // to edit a order (dishes or quantities) --false

                if (Details[OrderNo - 1].itemcount > 0) // automatic editing of order (replacing the new quantity and in case of removal of a dish quantity '0' )

                {

                    for (unsigned short int brute = 0; brute < (Details[OrderNo - 1].itemcount); brute++)

                    {

                        if (dish_id == order_id[OrderNo - 1][brute])

                        {

                            cout << "\n\n  Enter Quantity\n";

                            quantity = use.StoIconv();

                            storequantity[OrderNo - 1][brute] = quantity;

                            match = false;

                            break;
                        }
                    }

                } // if - ends her (editing done)

                if (dish_id >= 1001 && dish_id < counter + 1001 && match == true) // if the last item was for editing of order than don't run this
                {

                    cout << "\n\n  Enter Quantity  :  ";     quantity = use.StoIconv();
                    

                    if (quantity != 0)

                    {

                        order_id[OrderNo - 1][Details[OrderNo - 1].itemcount] = dish_id; // order_id[x][y] = dish_id store to collect data

                        storequantity[OrderNo - 1][Details[OrderNo - 1].itemcount] = quantity;

                        Details[OrderNo - 1].itemcount++; //  added 1 more item
                    }

                } // if -ends here

                if (dish_id >= 1001 && dish_id < counter + 1001)

                {

                    cout << "\n\n   Enter '1' to add more items                                                          Enter '0' if done  \n";
                    cout <<     "                                            Enter 'c' to Cancel Order  \n";
                    
                    cout << "\n  ENTER  :  ";          loop = use.StoCconv();
                   
                    while (loop != '1' && loop != '0' && loop != 'c') // in case of invalid input-- repeat
                   
                    {
                   
                        cout << "\n\n  *****   INVALID INPUT   *****\n";
                        cout << "\n\n   Enter '1' to add more items                                                          Enter '0' if done  \n";
                        cout <<     "                                            Enter 'c' to Cancel Order  \n";
                        cout << "\n  ENTER  :  ";          loop = use.StoCconv();
                   
                    }

                } // use for both editied items and additional items as long as inputs(dish_id) is  valid .

                else
                    cout << "\n\n  *****   INVALID INPUT   *****\n"; // in case of invalid input(dish_id) ;

            } // for loop ends here---------

            bool match_2 = false; // false : all the items in order are of '0' quantity

            for (unsigned short int brute = 0; brute <= Details[OrderNo - 1].itemcount; brute++)

            {

                if (storequantity[OrderNo - 1][brute] != 0)

                {

                    match_2 = true; // true when atleast one of the quantity is non-zero
                    break;
                }
            }

            if (loop == '0' && match_2 == true)

            {
                bool same = false;

                if (cancel_0 != 2) //----------Don't ask these detils more than one time -- in case of order editing
                {

                    cout << "\n\n  Enter Custmor's name  :  ";    Details[OrderNo - 1].SaveName();                 // --------------------------
                    
                    cout << "\n  Enter Customer's Phone Number  :  ";   Details[OrderNo - 1].SavePhoneNo_();    //customer's details saving
                                                                                                                 //-----------------------------
             
                }

               
                Details[OrderNo - 1].CalculateBill(OrderNo); // payment calculations

                
                if (Details[OrderNo - 1].paid == 0) //either the order is free or custmor want to cancel order or custmor want to edit the order
                {
                    cin.ignore(numeric_limits<streamsize>::max() ); // to clean buffer

                    cout << "\n\n  Enter '0' to cancel order                                                        Enter '2' to edit order\n";
                    cout <<     "                                          Enter '1' if order is free                        \n";

                    cout << "\n  ENTER  :  ";            cancel_0 = use.StoIconv();

                    while (cancel_0 != 0 && cancel_0 != 1 && cancel_0 != 2) // in case of invalid input-- repeat

                    {

                    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // to clean buffer

                    cout << "\n\n  Enter '0' to cancel order                                                        Enter '2' to edit order\n";
                    cout <<     "                                          Enter '1' if order is free                        \n";

                    cout << "\n  ENTER  :  ";            cancel_0 = use.StoIconv();

                    } // while -ends here

                } // if -ends here

               
               
                else
                    cancel_0 = 10; // if paid != 0 then set the default value of cancel_0

                switch (cancel_0)

                { // switch starts

                case 2: // edit order

                    cout << "\n\n       *************    You can CHANGE, DELETE OR ADD ITEM BY SIMPLY ENTERING THE DISH'ID AND QUANTITY   ***********\n ";
                         
                    break;

                case 0: // case to cancel order

                    break;

                default: // print bill on any default case

                    if (cancel_0 != 1)                                  //---- sinlge line if staement
                        Details[OrderNo - 1].PrintBill(OrderNo, false); //don't print bill if order is free(cancel_0==1)
                   
                    unsigned short int copyloop;
                   
                    for (unsigned short int lup = 1; lup < OrderNo; lup++)
                    {
                        if ((Details[OrderNo - 1].Co_phonenumber) == (Details[lup - 1].Co_phonenumber))
                        {
                            Details[lup - 1].Count_Co_Visit();
                            same = true;
                            copyloop = lup - 1;
                        }
                    }
                    if (same == true)
                    {
                        for (unsigned short int lup = 1; lup < Details[copyloop].Co_visit; lup++)
                        {
                            Details[OrderNo - 1].Count_Co_Visit();
                        }
                    }
                    if (same == false)
                        Customer_Count++;

                    Details[OrderNo - 1].Count_Co_Visit();
                    Details[OrderNo - 1].SaveOrderNo(OrderNo);
                    OrderNo++;
                }
            }

            if (match_2 == 0)

            {

                cout << "\n\n        ***********   COME ON --- PAY ATTENTION TO THE WORK ---  HOW CAN YOU MAKE A BILL  FOR '0' QUANTITY   *************        \n";
            }

        } while (cancel_0 == 2);
    }
}

//------------x---------------x-------------x-------------------x--------------x----------------x----------------x



//-----------------DISPLAY DATA FUNCTION---------------------

void resturant ::DisplayData(void)
{
    for (char decide = '1'; decide == '1';)
    {
        cout << "\n  Enter Order Number  :  ";           unsigned short int accept = use.StoIconv();
        
        if (Details[accept - 1].Co_OrderNo == accept && accept != 0 && accept <= OrderNo)
        {
            if (Details[accept - 1].itemcount == 0)
                cout << "\n\n  !!!!!!   Sorry   !!!!!    **********   Don't have a order to display   --------   To place a order follow these options   ********** \n";
            else
            {
                Details[accept - 1].PrintBill(accept, true);
            }
        }
        else
        {
            cout << "\n\n  *******   No matches Found for Order Number : " << accept << "   ***********\n";
        }

        cout << "\n  Enter '1' to view more Orders                              Enter '0' to go back\n";
        cout <<   "                                         ENTER  :  ";         decide = use.StoCconv();
        while (decide != '1' && decide != '0')
        {
        cout << "\n                               *****   INVALID INPUT   *****\n";
        cout << "\n  Enter '1' to view more Orders                              Enter '0' to go back\n";
        cout <<   "                                         ENTER  :  ";         decide = use.StoCconv();      }
  
    }

}

//------------x---------------x-------------x-------------------x--------------x----------------x----------------x

//----------------STATIC VARIABLE SCOPING-------------------
//initilaize with '0'.

unsigned short int resturant ::OrderNo = 1;
unsigned int resturant ::Customer_Count = 0;
unsigned short int resturant ::counter = 0;
float resturant ::price[50];
string resturant ::dishes[50];

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

          
            string name;
            float cost;

            cin.ignore(numeric_limits<streamsize>::max() , '\n' );
            cout << "\n  Enter the dish(" << counter + 1 << ")'s name :  ";
            getline(cin, name);
            dishes[counter] = name;

            cout << "\n\n  Enter the price for dish(" << dishes[counter] << ")  :  ";
            cost = use.StoFconv();
            price[counter] = cost;
            counter++; // initilized with '0' and updates +1 after addititon of every item

            
            // insert item into database
            string ID = to_string(counter + 1000);
            string PRICE = to_string(cost);
            
            string ins = "INSERT INTO menu (dish_id, dish, price) VALUES ('" + ID + "', '" + name + "', '" + PRICE + "')";
            if (mysql_query(conn, ins.c_str()))
            {
               cout << "Error: " << mysql_error(conn) << endl;
            }
            else
            {
              cout << "Item Added Successfuly." << endl;
            }
            Sleep(2000);
            
            
			// more items to add ?
            
            cout << "\n  Enter '0' if done                        Enter '1' to add next dish\n";
                cout <<   "                               ENTER  :  ";    loop = use.StoCconv();
            
            while (loop != '1' && loop != '0' )
            {
                cout << "\n\n  *****   INVALID INPUT   *****\n";
                cout << "\n  Enter '0' if done                        Enter '1' to add next dish\n";
                cout <<   "                               ENTER  :  ";    loop = use.StoCconv();
                
            } 

    }
}

//------------x---------------x-------------x-------------------x--------------x----------------x----------------x

//------------------ To remove item from menu function -------------------------------------------------------------------



void resturant :: RemoveItem ( void )

{  

    char input = '2' ;
    
  do
  {
      
    cout << "\n\n  Enter Dish'Id of item you want to Remove  :   ";              dish_id = use.StoIconv();

    
     if (dish_id >= 1001 && dish_id < counter + 1001)

    {

        for (int lup = dish_id - 1001; lup <= counter ; lup++)

        { // if lup = 0 ;  ( say )
          

            dishes[lup] = dishes[lup + 1]; // arr[0] = arr[ 1 ] , arr[1] = arr[2] ....

            price[lup] = price[lup + 1]; // ...  arr [ counter ] = arr [ counter + 1]
      
        }

        counter--; // one item is removed so counter updates to counter - 1 ;

        cout << "\n\n                               Enter '1' TO VIEW MENU\n ";
        cout <<     "  Enter '0' TO GO BACK                                           Enter '2' TO REMOVE MORE ITEMS   ------  \n";
        cout <<     "                                         ENTER  :  ";        input = use.StoCconv () ;

        
        if (input == '1' && counter != 0 )
  
             { 
                               
                menudisplay();
                
                input = '0' ;

             }   


        if ( input == '1' && counter == 0 )

             {

                 cout << "\n\n  !!!!!   SORRY  !!!!!!  THERE IS NO MENU TO DISPLAY   \n";

                 input = '0' ;

             }

        while ( input != '1' && input != '2'  && input != '0' )
         
         { 

            cout << "\n\n   **********   INVAILD INPUT   **********\n " ;

            cout << "\n  ENTER AGAIN  :  ";  input = use.StoCconv();

            if (input == '1' && counter != 0)
  
             {
                
                menudisplay();
                
                input = '0' ;

              
             }

             if ( input == '1' && counter == 0 )

             {

                 cout << "\n\n  !!!!!   SORRY  !!!!!! THERE IS NO MENU TO DISPLAY   \n";

                 input = '0' ;

             }
 

         }

    
    } // if -ends here

    else

    {

        cout << "\n\n                      **********   INVAILD INPUT   **********\n";
        
        cout <<     "  Enter '2' TO CONTIUNE                                     Enter '0' TO GO BACK \n ";    

        cout <<     "                               Enter '1' TO VIEW DISH'ID   \n";

        cout <<     "\n  ENTER  :  ";           input = use.StoCconv();


        while (input != '1' && input != '2' && input != '0')

        {

        cout << "\n\n                      **********   INVAILD INPUT   **********\n";
        
        cout <<     "  Enter '2' TO CONTIUNE                                     Enter '0' TO GO BACK \n ";    

        cout <<     "                               Enter '1' TO VIEW DISH'ID   \n";

        cout <<     "\n  ENTER  :  ";           input = use.StoCconv();
          
          
          
          if (input == '1')

            { 

                menudisplay();
                
                input == '2' ;
            }
      
        } 
    

    }  // --else ends here

  } while ( input == '2' ) ;


}


//------------x---------------x-------------x-------------------x--------------x----------------x----------------x            


//----------------------MENU DISPLAY-------------------------------

void resturant ::menudisplay(void)
{
    cout << "\n\n  *****   Good Food, Good Life   *****\n";
    cout << "\n\n  ITEM(id) " << setw(11) << ": "
         << "  PRICE\n";
    for (dish_id = 0; dish_id < counter; dish_id++)
    {
        cout<<"\n  " << dishes[dish_id] << "(" << 1001 + dish_id << ") " << setw(15 - (dishes[dish_id].length())) << ": " << fixed << setprecision(2) << price[dish_id] << endl;
    }
}

//------------x---------------x-------------x-------------------x--------------x----------------x----------------x

//---------------------CHANGE PRICE FUNCTION---------------------------

void resturant ::changeprice(void)

{
    
    for (char loop = '1'; loop == '1';)
 
    {
 
        cout << "\n\n  Enter the id number of item you want to change  :  ";    dish_id = use.StoIconv();
        
        
        if (dish_id - 1001 < counter && dish_id - 1001 >= 0)
 
        {
            cin.ignore(numeric_limits<streamsize>::max(), '\n' );

            cout << "\n\n  To change dish name, enter new name of dish  :  ";
            
            getline( cin, dishes [ dish_id - 1001 ] ) ;
            
            cout << "\n\n  To change price, enter new price  :  ";      price[dish_id - 1001] = use.StoFconv();
            
            cout <<"\n\n  " << dishes[dish_id - 1001] << "(" << dish_id << ") " << setw(15 - (dishes[dish_id - 1001].length())) << ": " << fixed << setprecision(2) << price[dish_id - 1001] << endl;
            
            cout << "\n  Enter '1' to change more items                                        Enter '0' to go back\n";
            
            cout << "\n                                             ENTER  :  ";     loop = use.StoCconv();
            
            while (loop != '1' && loop != '0')
            {
            cout << "\n\n                                   *****   INVALID INPUT   *****\n";
                
                cout << "  Enter '1' to change more items                                        Enter '0' to go back\n";
            
                cout << "                                              ENTER  :  ";     loop = use.StoCconv();
            }
        }
        else
        {
            cout << "\n\n  *****   INVALID INPUT   *****\n";
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
            if (counter != 0 )
                RemoveItem();
            else
                cout << "\n\n  ---------   There is no MENU to edit       -------       Follow case '1' to Make MENU  ------\n ";
            break;
        case '3':
            if (counter != 0)
                changeprice();
            else
                cout << "\n\n  ---------   There is no MENU to edit       -------       Follow case '1' to Make MENU  ------\n ";
            break;
        case '4':
            if (counter != 0)
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



// ********************   ASK PASSWORD FROM USER TO ACCESS DATABASE **********************  

string getPassword(){                
	cout << "\t\tENTER PASSWORD TO CONNECT WITH DATABASE:  ";
	string pwd;
	cin >> pwd;
	system( "cls" );
	
	return pwd;
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
    Co_Details view;
    
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
            order.DisplayData();
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

/* input----------------------------------------------------------------------input file 
1
1
d1
10
0
0
2
1001
3
0
sumit
9306435220
3
1
0


*/








