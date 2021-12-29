// +----------------------------------+
// |          HEADER FILES            |
// +----------------------------------+

#include <iostream>
#include <windows.h>
#include <mysql.h>
#include <sstream>
#include <conio.h>
#define HOST "127.0.0.1"
#define USER "root"
#define PASS /*put the password of MySQL in here between ""*/
#define DATABASE "Management"
#define PORT 3306
#define PASSWORD /*Set a Numeric Password for Your Application*/

using namespace std;

// +--------------------------------+
// |		Global Variable     |
// +--------------------------------+

MYSQL * conn;
MYSQL_RES *res_set;
MYSQL_ROW row;
stringstream stmt;
const char * q;
string query;
// my_bool result;

typedef struct
{
	int date;
	int month;
	int year;
}date;


// +----------------------------------+
// |	         Classes Made 		  |
// +----------------------------------+

class books
{
	int id;			// Primary Key
	string name;
	string auth;
	int price;
	int qty;
public:
	void add();
	void update_price();
	void search();
	void update();
	void display();
};

class suppliers
{
	int id;				//Primary Key
	string name;
	long int phn;
	string addr_line1;
	string addr_line2;
	string addr_city;
	string addr_state;
public:
	void add_sup();
	void remove_supplier();
	void search_id();
};

class purchases
{
	int ord_id;			//Primary Key
	int book_id;		//FK ref (books)
	int sup_id;			//FK ref (suppliers)
	int qty;
	date dt_ordered;
	int eta;
	char received;		// Check(T or C or F) def (F)
	int inv;
public:
	void new_ord();
	void view();
	void mar_cancel();
	void mark_reciv();
};

class employees
{
	int id;				//Primary Key
	string name;
	string addr_line1;
	string addr_line2;
	string addr_city;
	string addr_state;
	long int phn;
	date date_of_joining;
	long int salary;
	string mgr_status;	//check(T or F) def f
public:
	void add_emp();
	void search_emp();
	void assign_mgr_stat();
	void display();
	void update_sal();
};

class members
{
    int id;				//Primary Key
	string name;
	string addr_line1;
	string addr_line2;
	string addr_city;
	string addr_state;
	long int phn;
	date beg_date;
	date end_date;
	string valid;
public:
	void add_mem();
	void refresh();
	void search_mem();
};

class sales
{
	int invoice_id;		//Primary key
 	int member_id;		//FK ref member(id)
	int book_id;		//FK ref books(id)
	int qty;
	int amount;
	date date_s;
public:
	void add();
	void find_total_sales();
};

// +----------------------------------+
// |		Member Functions	  	  |
// +----------------------------------+

// ------------
// class books
// ------------

void books::add()
{
	cout << "Enter the name of the book : " ;
	cin >> name;
	cout << "Enter the name of the author : ";
	cin >> auth;
	cout << "Enter the Price : ";
	cin >> price;
	cout << "Enter the Qty Recived : ";
	cin >> qty;
	stmt.str("");
	stmt << "Insert into books(name,auth,price,qty) values('" << name << "','" << auth << "'," << price << "," << qty << ");";
	query = stmt.str();
	q = query.c_str();
	mysql_query(conn,q);
	res_set = mysql_store_result(conn);
	if (!(res_set))
        cout << endl << endl << "Book Record Inserted Successfully" << endl << endl << endl;
    else
        cout << endl << endl << "Entry ERROR !" << endl << "Contact Technical Team " << endl << endl << endl;
}

void books::update_price()
{
    char choice;
    cout << "Enter the id of the book for update in price : ";
    cin >> id;
    stmt.str("");
    stmt << "Select name,price from books where id = " << id << ";";
    query = stmt.str();
    q = query.c_str();
    mysql_query(conn,q);
    res_set = mysql_store_result(conn);
    if((row = mysql_fetch_row(res_set)) != NULL)
    {
        cout << "The Name of the book is : " << row[0] << endl ;
        cout << "The current price of the book is : " << row[1] << endl ;
        cout << "Do you Want to Update the Price [y/n] : " ;
    	cin >> choice;
		if (choice == 121 || choice == 89)
		{
			cout << "Enter the new price : ";
			cin >> price;
			stmt.str("");
			stmt << "Update books set price = " << price << " where id = " << id << ";";
			res_set = mysql_store_result(conn);
			if (!(res_set))
        		cout << endl << endl << "Book Price Updated Successfully" << endl << endl << endl;
    		else
        		cout << endl << endl << "Entry ERROR !" << endl << "Contact Technical Team " << endl << endl << endl;
		}
		else
		{
			cout << "No changes Made!!";
		}
	}
	else
	{
		cout << "No Book found!!!";
	}
}

void books::search()
{
	cout << "Enter book id for details : ";
	cin >> id;
    stmt.str("");
    stmt << "Select * from books where id = " << id << ";";
    query = stmt.str();
    q = query.c_str();
    mysql_query(conn,q);
    res_set = mysql_store_result(conn);
    if((row = mysql_fetch_row(res_set)) != NULL)
    {
		cout << "The Details of Book Id " << row[0] << endl;
        cout << "The Name of the book is : " << row[1] << endl ;
		cout << "THE Author of " << row[1] << " is " << row[2] << endl;
        cout << "The Price of the book is : " << row[3] << endl ;
		cout << "The inventory count is " << row[4] << endl;
	}
	else
	{
		cout << "No record Found" << endl;
	}
}

void books::update()
{
	int b_id[100],qty[100],i = 0,max;
	stmt.str("");
	stmt << "Select book_id,qty from purchases where recieves = 'T' and inv IS NULL;";
	query = stmt.str();
	q = query.c_str();
	mysql_query(conn,q);
	res_set = mysql_store_result(conn);
	stmt.str("");
	stmt << "Update purchases set inv = 1 where recieves = 'T' and inv IS NULL;";
	query = stmt.str();
	q = query.c_str();
	mysql_query(conn,q);
	while((row = mysql_fetch_row(res_set)) != NULL)
	{
		b_id[i] = (int) row[0];
		qty[i] = (int) row[1];
		i++;
	}
	max = i;
	for(i = 0; i <= max; i++)
	{
		stmt.str("");
    	stmt << "update books set qty = " << qty[i] << " where id = " << b_id[i] << ";";
    	query = stmt.str();
    	q = query.c_str();
    	mysql_query(conn,q);
	}
	cout << "The orders recieved have been updated.";
}

void books::display()
{
	int i = 0;
	query = "Select * from books;";
	q = query.c_str();
	mysql_query(conn,q);
	res_set = mysql_store_result(conn);
	while((row = mysql_fetch_row(res_set)) != NULL)
	{
		cout << "Name for book " << ++i << " : " << row[1] << endl;
		cout << "Name of Author : " << row[2] << endl;
		cout << "Price : " << row[3] << endl;
		cout << "Quantity : " << row[4] << endl;
		cout << endl << endl << endl << endl ;
	}
}



// ---------------
// class suppliers
// ---------------

void suppliers::add_sup()
{
	cout << "Enter the Supplier Name : ";
	cin >> name;
	cout << "Enter Phone no. : ";
	cin >> phn;
	cout << "Enter the address (in 3 lines) : ";
	cin >> addr_line1;
	cin >> addr_line2;
	cin >> addr_city;
	cout << "Enter State : ";
	cin >> addr_state;
	stmt.str("");
	stmt << "Insert into suppliers(name,phone_no,addr1,addr2,addr_city,addr_stat) values('" << name << "'," << phn << ",'" << addr_line1 << "','" << addr_line2 << "','" << addr_city << "','" << addr_state << "');";
	query = stmt.str();
	q = query.c_str();
	mysql_query(conn,q);
	res_set = mysql_store_result(conn);
	if (!(res_set))
        cout << endl << endl << "Supplier Record Inserted Successfully" << endl << endl << endl;
    else
        cout << endl << endl << "Entry ERROR !" << endl << "Contact Technical Team " << endl << endl << endl;
}

void suppliers::remove_supplier()
{
	cout << "Enter the supplier id to remove the Supplier : ";
	cin >>  id;
	stmt.str("");
	stmt << "Delete from suppliers where id = " << id << ";";
	query = stmt.str();
	q = query.c_str();
	mysql_query(conn,q);
	cout << "Supplier Removed.";
}

void suppliers::search_id()
{
	cout << "Enter the supplier id to find the Supplier details : ";
	cin >>  id;
	stmt.str("");
	stmt << "Select * from suppliers where id = " << id << ";";
	query = stmt.str();
	q = query.c_str();
	mysql_query(conn,q);
	res_set = mysql_store_result(conn);
	if((row = mysql_fetch_row(res_set)) != NULL)
	{
		cout << "Details of Supplier Id : " << row[0] << endl;
		cout << "Name : " << row[1] << endl;
		cout << "Phone no. : " << row[2] << endl;
		cout << "Address Line 1 : " << row[3] << endl;
		cout << "Address Line 2 : " << row[4] << endl;
		cout << "City : " << row[5] << endl;
		cout << "State : " << row[6] << endl;
	}
	else
	{
		cout << "No Supplier Found!!";
	}
}



// ------------------
// class Purchases
// ------------------

void purchases::new_ord()
{
	cout << "Enter the book Id : ";
	cin >> book_id;
	cout << "Enter Supplier Id : ";
	cin >> sup_id;
	cout << "Enter the Quantity : ";
	cin >> qty;
	cout << "Estimated expected Delivery (in days) : ";
	cin >> eta;
	stmt.str("");
	stmt << "Insert into purchases (book_id,sup_id,qty,dt_ord,eta) values ( " << book_id << "," << sup_id << "," << qty << ",curdate(),Date_add(curdate(), INTERVAL " << eta << " DAY)" << ";";
	query = stmt.str();
	q = query.c_str();
	mysql_query(conn,q);
	cout << "New order Added!!";
}

void purchases::mark_reciv()
{
	cout << "Enter the order id for order recieved : ";
	cin >> ord_id;
	stmt.str("");
	stmt << "Update puchases set recieved = 'T' where ord_id = " << ord_id << ";";
	query = stmt.str();
	q = query.c_str();
	mysql_query(conn,q);
	cout << "Recieved Marked successfully";
}

void purchases::mar_cancel()
{
	cout << "Enter the order id for order cancelled : ";
	cin >> ord_id;
	stmt.str("");
	stmt << "Update puchases set recieved = 'C' where ord_id = " << ord_id << ";";
	query = stmt.str();
	q = query.c_str();
	mysql_query(conn,q);
	cout << "Cancelled Marked successfully";
}

void purchases::view()
{
	int c;
	cout << "Select an Option" << endl;
	cout << "1. View orders not Recieved" << endl;
	cout << "2. View orders Cancelled" << endl;
	cout << "3. View orders Recieved" << endl;
	cout << "Enter Your choice : ";
	cin >> c;
	if (c == 1)
		received = 'F';
	else if (c == 2)
		received = 'C';
	else if (c == 3)
		received = 'T';
	else
		return;
	stmt.str("");
	stmt << "Select * from purchases where recieved = '" << received << "';";
	query  = stmt.str();
	q = query.c_str();
	mysql_query(conn,q);
	res_set = mysql_store_result(conn);
	if (c == 1)
		cout << endl << "Orders not recieved are" << endl;
	else if (c == 2)
		cout << endl << "Orders Cancelled are" << endl;
	else if (c == 3)
		cout << endl << "Orders recieved are" << endl;
	while((row = mysql_fetch_row(res_set)) != NULL)
	{
		cout << endl;
		cout << "Order Id : " << row[0] << endl;
		cout << "Book Id : " << row[1] << endl;
		cout << "Supplier Id :" << row[2] << endl ;
		cout << "Quantity : " << row[3] << endl;
		cout << "Date Ordered : " << row[4] << endl;
		cout << "Estimated Delivery date : " << row[5] << endl;
		cout << endl << endl << endl;
	}
	cout <<endl << endl << endl <<endl << endl;
}


// ----------------
// class employees
// ----------------

void employees::add_emp()
{
	cout << "Enter Your Id for verification : ";
	cin >> id ;
	stmt.str("");
	stmt << "Select mgr_stat from employees where id  = " << id << ";";
	query = stmt.str();
	q = query.c_str();
	mysql_query(conn,q);
	res_set = mysql_store_result(conn);
	if ((row = mysql_fetch_row(res_set)) == NULL)
	{
		cout << "Employee Not Found!!" << endl << endl << endl;
		return;
	}
	else
	{
		mgr_status = (char*) row[0];
		if (mgr_status == "T")
		{
			cout << "You Do Not have Manager Rights!!!" << endl << endl ;
			return;
		}
	}
	cout << "Enter The details of new employees!!";
	cout << "Enter The name of the employee : ";
	cin >> name;
	cout << "Enter the Address (in 3 lines) : " << endl;
	cin >> addr_line1;
	cin >> addr_line2;
	cin >> addr_city;
	cout << "Enter State : ";
	cin >> addr_state;
	cout << "Enter phone no.";
	cin >> phn;
	cout << "Enter the salary : ";
	cin >> salary;
	stmt.str("");
	stmt << "Insert into employees (name,addr1,addr2,addr_city,addr_stat,phone_no,doj,salary) values ('" << name << "','" << addr_line1 << "','" << addr_line2 << "','" << addr_city << "','" << addr_state << "'," << phn << ",curdate()," << salary << ");";
	query = stmt.str();
	q = query.c_str();
	mysql_query(conn,q);
	cout << endl << endl << "Employee Added Succesfully!" << endl << endl << endl;
}


void employees::assign_mgr_stat()
{
	cout << "Enter Your Id for verification : ";
	cin >> id ;
	stmt.str("");
	stmt << "Select mgr_stat from employees where id  = " << id << ";";
	query = stmt.str();
	q = query.c_str();
	mysql_query(conn,q);
	res_set = mysql_store_result(conn);
	if ((row = mysql_fetch_row(res_set)) == NULL)
	{
		cout << "Employee Not Found!!" << endl << endl << endl;
		return;
	}
	else
	{
		mgr_status = (char*) row[0];
		if (mgr_status == "T")
		{
			cout << "You Do Not have Manager Rights!!!" << endl << endl ;
			return;
		}
	}
	cout << "Enter the employee id to grant Manager status : ";
	cin >> id;
	stmt.str("");
	stmt << "update employees set mgr_stat = 'T' where id = " << id << ";";
	query = stmt.str();
	q = query.c_str();
	mysql_query(conn,q);
	cout << endl << endl << endl;
	cout << "Manager Status granted";
	cout << endl << endl << endl;
}

void employees::search_emp()
{
	cout << "Enter the id for searching an employee : ";
	cin >> id ;
	stmt.str("");
	stmt << "Select * from employees where id  = " << id << ";";
	query = stmt.str();
	q = query.c_str();
	mysql_query(conn,q);
	res_set = mysql_store_result(conn);
	if ((row = mysql_fetch_row(res_set)) != NULL)
	{
		cout << "Employees Details" << endl ;
		cout << "Name : " << row[1] <<endl;
		cout << "Address : " << endl << row[2] << endl << row[3] << endl << row[4] << endl;
		cout << "State : " << row[5] << endl;
		cout << "Contact no. : " << row[6] << endl;
		cout << "Date of Joining" << row[7] << endl;
		cout << "Salary : " << row[8] << endl << endl << endl;
	}
	else
	{
		cout << "No Employee Found!!" << endl << endl << endl;
	}
}

void employees::display()
{
	int i = 0;
	query = "Select * from employees;";
	q = query.c_str();
	mysql_query(conn,q);
	res_set = mysql_store_result(conn);
	if((row = mysql_fetch_row(res_set)) != NULL)
    {
       do
       {
    	cout << "Employees Details of Emp no." << ++i << endl ;
        cout << "Name : " << row[1] <<endl;
		cout << "Address : " << endl << row[2] << endl << row[3] << endl << row[4] << endl;
		cout << "State : " << row[5] << endl;
		cout << "Contact no. : " << row[6] << endl;
		cout << "Date of Joining" << row[7] << endl;
		cout << "Salary : " << row[8] << endl;
		cout << endl << endl << endl << endl;
       }while((row = mysql_fetch_row(res_set)) != NULL);
    }
	else
	{
		cout << "Employees Not found!" << endl;
	}
}

void employees::update_sal()
{
	cout << "Enter the id to update the salary of an employee : ";
	cin >> id ;
	cout << "Enter the updated salary : ";
	cin >> salary;
	stmt.str("");
	stmt << "Update employees set salary = " << salary << " where id  = " << id << ";";
	query = stmt.str();
	q = query.c_str();
	mysql_query(conn,q);
	cout << endl << endl << endl << endl;
	cout << "Salary update Successfully";
	cout << endl << endl << endl << endl;
}

// ---------------------
// class members
// ---------------------

void members::add_mem()
{
	cout << "Enter the name of the member : ";
	cin >> name;
	cout << "Enter phone no. : ";
	cin >> phn;
	cout << "Enter address (in 3 lines)" << endl;
	cin >> addr_line1;
	cin >> addr_line2;
	cin >> addr_city;
	cout << "Enter the name of the state : " ;
	cin >> addr_state;
	stmt.str("");
	stmt << "insert into members(name,addr1,addr2,addr_city,addr_stat,phone_no,beg_date,end_date) values ('" << name << "','" << addr_line1 << "','" << addr_line2 << "','" << addr_city << "','" << addr_state << "'," << phn << ",curdate(),Date_add(curdate(), INTERVAL 1 YEAR));";
	query = stmt.str();
	q = query.c_str();
	mysql_query(conn,q);
	// fetching member id...
	stmt.str("");
	stmt << "Select id from members where phone_no = " << phn << ";";
	query = stmt.str();
	q = query.c_str();
	mysql_query(conn,q);
	res_set = mysql_store_result(conn);
	row = mysql_fetch_row(res_set);
	cout << endl << endl << endl << endl;
	cout << "Member Added successfully" << endl << endl <<"Member Id : " << row[0];
	cout << endl << endl << endl << endl;
}


void members::refresh()
{
	query = "Update members set valid = 'invalid' where end_date <= curdate();";
	q = query.c_str();
	mysql_query(conn,q);
}

void members::search_mem()
{
	cout << "Enter member id : ";
	cin >> id;
	stmt.str("");
	stmt << "Select * from members where id  = " << id << ";";
	query = stmt.str();
	q = query.c_str();
	mysql_query(conn,q);
	res_set = mysql_store_result(conn);
	if ((row = mysql_fetch_row(res_set)) != NULL)
	{
		cout << "Member Details" << endl ;
		cout << "Name : " << row[1] << endl;
		cout << "Address : " << endl << row[2] << endl << row[3] << endl << row[4] << endl;
		cout << "State : " << row[5] << endl;
		cout << "Contact no. : " << row[6] << endl;
		cout << "Membership begin date : " << row[7] << endl;
		cout << "Membership end date  : " << row[8] << endl;
		cout << "Membership Status : " << row[9] << endl << endl << endl;
	}
	else
	{
		cout << "No Member Found!!" << endl << endl << endl;
	}
}

// ------------------
// Class Sales
// ------------------

void sales::add()
{
	cout << "Enter Menber id :";
	cin >> member_id;
	cout << "Enter the book id : ";
	cin  >> book_id;
	cout << "Enter the quantity : ";
	cin  >> qty;
	stmt.str("");
	stmt << "select price*" << qty << " from books where id = " << book_id << ";";
	query = stmt.str();
	q = query.c_str();
	mysql_query(conn,q);
	res_set = mysql_store_result(conn);
	if ((row = mysql_fetch_row(res_set)) != NULL)
	{
		cout << "The bill amount : " << row[0] << endl << endl;
		amount = (int) row[0];
	}
	else
	{
		cout << "Book Id invalid!!" << endl;
		getch();
		return;
	}
	stmt.str("");
	stmt << "insert into sales(mem_id,book_id,qty,amt,sales_date) values (" << member_id << "," << book_id << "," << qty << "," << amount << ",curdate());";
	query = stmt.str();
	q = query.c_str();
	mysql_query(conn,q);
	// fetching invoice id...
	stmt.str("");
	stmt << "select inv_id from sales where mem_id = " << member_id << " AND book_id = " << book_id << " AND qty = " << qty << " AND sales_date = curdate();";
	query = stmt.str();
	q = query.c_str();
	mysql_query(conn,q);
	res_set = mysql_store_result(conn);
	if((row = mysql_fetch_row(res_set)) != NULL)
	{
		cout << "The Invoice id for the bill is " << row[0] << endl << endl << endl;
	}
	else
	{
		cout << "The entered details maybe wrong." << endl << "Please Recheck and Enter again" << endl << endl << endl;
	}
}


void sales::find_total_sales()
{
	query = "select sum(amt) from sales where year(sales_date) = year(curdate());";
	q = query.c_str();
	mysql_query(conn,q);
	res_set = mysql_store_result(conn);
	if((row = mysql_fetch_row(res_set)) != NULL)
	{
		cout << "Total sales this year : " << row[0] << endl << endl << endl << endl;
	}
}

// ----------------
// Functions
// ----------------

void book_menu();
void sup_menu();
void pur_menu();
void emp_menu();
void mem_menu();
void sal_menu();

// main menu

void main_menu()
{
	int c;
	cout << "*************************************************" << endl;
	cout << "         BOOKSHOP MANGEMENT SYSTEM" << endl;
	cout << "*************************************************" << endl;
	cout << "   1. BOOKS" << endl;
	cout << "   2. SUPPLIERS" << endl;
	cout << "   3. PURCHASES" << endl;
	cout << "   4. EMPLOYEES" << endl;
	cout << "   5. MEMBERS" << endl;
	cout << "   6. SALES" << endl;
	cout << "   7. EXIT" << endl << endl << endl;
	cout << "Enter Your Choice : ";
	cin >> c;
	switch (c)
	{
		case 1:
			system("cls");
			book_menu();
			getch();
			break;
		case 2:
			system("cls");
			sup_menu();
			getch();
			break;
		case 3:
			system("cls");
			pur_menu();
			getch();
			break;
		case 4:
			system("cls");
			emp_menu();
			getch();
			break;
		case 5:
			system("cls");
			mem_menu();
			getch();
			break;
		case 6:
			system("cls");
			sal_menu();
			getch();
			break;
		case 7:
			exit(1);
		default:
			system("cls");
			cout << "Wrong Input" << endl << endl << "Invalid input";
			getch();
			break;
	}
	return;
}

// book menu

void book_menu()
{
	int c;
	books b;
	cout << "*************************************************" << endl;
	cout << "                  BOOK MENU" << endl;
	cout << "*************************************************" << endl;
	cout << "   1. ADD" << endl;
	cout << "   2. UPDATE PRICE" << endl;
	cout << "   3. SEARCH" << endl;
	cout << "   4. UPDATE STATUS" << endl;
	cout << "   5. DISPLAY ALL" << endl;
	cout << "   6. RETURN TO MAIN MENU" << endl << endl << endl;
	cout << "Enter Your Choice : ";
	cin >> c;
	switch (c)
	{
		case 1:
			b.add();
			break;
		case 2:
			b.update_price();
			break;
		case 3:
			b.search();
			break;
		case 4:
			b.update();
			break;
		case 5:
			b.display();
			break;
		case 6:
			return;
			break;
		default:
			cout << "Wrong Input" << endl << "Invalid input";
			break;
	}
	return;
}

// supp menu

void sup_menu()
{
	int c;
	suppliers s;
	cout << "*************************************************" << endl;
	cout << "                SUPPLIER MENU" << endl;
	cout << "*************************************************" << endl;
	cout << "   1. ADD" << endl;
	cout << "   2. REMOVE" << endl;
	cout << "   3. SEARCH" << endl;
	cout << "   4. RETURN TO MAIN MENU" << endl << endl << endl;
	cout << "Enter Your Choice : ";
	cin >> c;
	switch (c)
	{
		case 1:
			s.add_sup();
			break;
		case 2:
			s.remove_supplier();
			break;
		case 3:
			s.search_id();
			break;
		case 4:
			return;
		default:
			cout << "Wrong Input" << endl << "Invalid input";
			break;
	}
}

// purchase menu

void pur_menu()
{
	int c;
	purchases p;
	cout << "*************************************************" << endl;
	cout << "                PURCHASES MENU" << endl;
	cout << "*************************************************" << endl;
	cout << "   1. New Order" << endl;
	cout << "   2. View All" << endl;
	cout << "   3. Cancel Order" << endl;
	cout << "   4. Recieved Order" << endl;
	cout << "   5. RETURN TO MAIN MENU" << endl << endl << endl;
	cout << "Enter Your Choice : ";
	cin >> c;
	switch (c)
	{
		case 1:
			p.new_ord();
			break;
		case 2:
			p.view();
			break;
		case 3:
			p.mar_cancel();
			break;
		case 4:
			p.mark_reciv();
			break;
		case 5:
			return;
		default:
			cout << "Wrong Input" << endl << "Invalid input";
			break;
	}
}

// emp_menu

void emp_menu()
{
	int c;
	employees e;
	cout << "*************************************************" << endl;
	cout << "                 EMPLOYEE MENU" << endl;
	cout << "*************************************************" << endl;
	cout << "   1. New Employee" << endl;
	cout << "   2. Search Employee" << endl;
	cout << "   3. Assign Manager" << endl;
	cout << "   4. View All" << endl;
	cout << "   5. Update Salary" << endl;
	cout << "   6. RETURN TO MAIN MENU" << endl << endl << endl;
	cout << "Enter Your Choice : ";
	cin >> c;
	switch (c)
	{
		case 1:
			e.add_emp();
			break;
		case 2:
			e.search_emp();
			break;
		case 3:
			e.assign_mgr_stat();
			break;
		case 4:
			e.display();
			break;
		case 5:
			e.update_sal();
			break;
		case 6:
			return;
		default:
			cout << "Wrong Input" << endl << "Invalid input";
			break;
	}
}


// mem menu

void mem_menu()
{
	int c;
	members m;
	m.refresh();
	cout << "*************************************************" << endl;
	cout << "                 MEMBERS MENU" << endl;
	cout << "*************************************************" << endl;
	cout << "   1. New Member" << endl;
	cout << "   2. Search Member" << endl;
	cout << "   3. RETURN TO MAIN MENU" << endl << endl << endl;
	cout << "Enter Your Choice : ";
	cin >> c;
	switch (c)
	{
		case 1:
			m.add_mem();
			break;
		case 2:
			m.search_mem();
			break;
		case 3:
			return;
		default:
			cout << "Wrong Input" << endl << "Invalid input";
			break;
	}
}

// sal_menu

void sal_menu()
{
	int c;
	sales s;
	cout << "*************************************************" << endl;
	cout << "                 SALES MENU" << endl;
	cout << "*************************************************" << endl;
	cout << "   1. Add New Bill" << endl;
	cout << "   2. Total Sales Of The Year" << endl;
	cout << "   3. RETURN TO MAIN MENU" << endl << endl << endl;
	cout << "Enter Your Choice : ";
	cin >> c;
	switch (c)
	{
		case 1:
			s.add();
			break;
		case 2:
			s.find_total_sales();
			break;
		case 3:
			return;
		default:
			cout << "Wrong Input" << endl << "Invalid input";
			break;
	}
}


// Password Function

void pass()
{
	int num = 0 ;
	cout << "Enter password : ";
	for(int i = 0; i < 4; i++)
	{
		num=num*10+(getch()-48);
		cout<<"*";
	}
	if(num == PASSWORD)
	{
		cout << endl << endl << "Correct Password" << endl << endl;
		cout << "Press any key...";
		getch();
	}
	else
	{
		cout << endl << endl << "Incorrect Password" << endl << endl;
		cout << "Press any key...";
		getch();
		exit(1);
	}
	return;
}

// +----------------------------------+
// |		Main Function 	      |
// +----------------------------------+

int main()
{
	pass();
	conn = mysql_init(0);
	conn = mysql_real_connect(conn, HOST, USER, PASS, DATABASE, PORT, NULL, 0);
	int choice;
	if(conn)
    {
		while(1)
		{
			system("cls");
			main_menu();
		}
    }
	else
	{
	    system("cls");
		cout << "Error While connection to database." << endl << "Contact Tech Expert." << endl;
		getch();
	}
	return 0;
}
