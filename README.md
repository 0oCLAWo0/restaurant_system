# restaurant_system
-----  NOTE :: INFORMATION HERE IS GOING TO BE ABOUT HOW TO USE THIS PROGRAM NOT ABOUT HOW TO MAKE CHANGES IN THIS PROGRAM

At the  very satrting when you will hit your run button you are going to see '5' options and you have to choose one of them (of course).

But you can't choose option no. '2' & '3' until you set a menu for your resturant becuase both options requires a 'MENU' to do their function     

------   A BRIEF INTRODUCTION OF THESE '5' OPTIONS  ----------

     To select an Option you just have to enter the option No.

Option '0' will help you to take exit from program just by choosing option '0' you will be able to exit this program succesfully

Option '1' will help you to set and edit your ' RESTURANT MENU '.

Option '2' will help you to take orders but can only be activate by setting a 'MENU'(USING OPTION 1)

Option '3' will be for Displaying all the CUSTOMER'S DATA you collected till now but can only be activate by atleast taking one Order(USING OPTION '2')

Option '4' is for the displaying all the information about this program as you are seeing right now.

   -------      Detailed informatoin about Option '1' ( ***  EDIT MENU  *** )    --------

As soon as you will enter '1' your option '1' will get activate and your blank menu will be generated now you can fill that menu with more of its features which are embedded into more Options

$$$  NOTE $$$ A blank menu can not be considered as a menu until you fill it with atleast one item

--- ADD :: OPTION :: --- will add new items and their price in your menu  $$$$$ IMPORANT $$$$ all of your items are going to be identified by a unique ID named as DISH'ID.

--- DISH'ID :: It's a '4 Digit Code' created by system to uniquely identify  each item in your menu----- you can find it inside DISPLAY-MENU OPTION ( '4' ) inisde Set -Menu OPtion( '1' )

--- REMOVE :: OPTION '2' :: --- Will remove items from your Menu. To do so you just have to enter the DISH'ID(MENTIONED IN ABOVE POINT) OF THE ITEM you want to remove from menu

--- Change Price :: OPTION '3' :: --- this option will be useful in case you want to change an item name or price and you can do so just by entering DISH'ID and new name or price

--- DISPLAY-MENU :: OPTION '4' :: --- this option is for presenting the menu you build so far.

--- TO GO  BACK ::  OPTION '0' :: --- it will help to go one step back(where the last options were present) inside the program

$$$  NOTE $$$ It will not exit the program right away it will only help you to take one step back inside the program



  TAKE ORDER FUNCTON : : :  once you choose this option, a order number will be generated and will be equal to '1' after completion of 1st order and will futher increase by '1' for next order

  By providing order number you can access all the data of customer by using Dispaly order option.

  ----------------- HOW TO TAKE ORDER --------------

 to add new items : enter desired dish'id and the quantity

 to REMOVE items : enter desired dish'id and the quantity = '0'

 to CHANGE QUANITY : enter desired dish'id and the quantity = NEW QUANTITY and it will overwrite the old.

  once you done taking order you will be asked for entering customer's details consist of name and phone number.

 Now, phone number here plays a crucial role let's see in depth : :

 first of all if a customer don,t have a phone number or phone number entered turns to be invalid than a default phone number will be added instead of the wrong one.

 VALID PH_NO. is a 10- digit number whose 1st digit is non zero or a 11- digit number whose 1st digit is zero any other number will be count as INAVLID PH_NO.

 eg: 1122334455 - vaild or 01122334455 - vaild  but 001122334455 - INVALID     and     DEFAULT PH_NO IS : 2323232323

 Now if a custmor comes with the agian and provide the same details than program will automatically detect and incease the visiting of that person with '1' every time where defalut visitng is '1'

 After done with details a u be be asked for any discount and then total amount of the order will be shown on screen and worker will be asked for talking that amount from custmoe and enter below the amount taken from custmor named as paid amount


 Now,  if custmor decides to quit u can enter amount '0' or u can enetr amount <= total in case of succesful payment but u can't enter 0 < paid < total

 and if order is free than u can also enter the amount zero but this time data will be stored, without bill

 also if customer wants to edit order so that his/her price will be in budget than u can also ener '0' this time it will take u back to the order and will asked for the dish'id you want to change

 to add, remove or change item u just have to enter dish'id and quantity

 to add new items : enter desired dish'id and the quantity

 to REMOVE items : enter desired dish'id and the quantity = '0'

 to CHANGE QUANITY : enter desired dish'id and the quantity = NEW QUANTITY and it will overwrite the old.

 you have to repeat billing process and than you will be done with your order, now you can view data by using DisplayData function.  
