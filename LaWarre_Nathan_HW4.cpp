#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <iostream>
#include <fstream>
#include <cstring>
#include <time.h>
#include <mysql_connection.h>
#include <mysql_driver.h>
#include <mysql_error.h>
#include <cppconn/statement.h>

int getUserInput(void);
void addCustomer(sql::Statement * stmt);
void addOrder(sql::Statement * stmt);
void removeOrder(sql::Statement * stmt);
void shipOrder(sql::Statement * stmt);
void printPending(sql::Statement * stmt);
void restockParts(sql::Statement * stmt);

using namespace std;

int main(void){
  int choice = 0;
  try{
    sql::Driver * driver = get_driver_instance();
    sql::Connection * con = driver->connect("localhost", "cs4430","cs4430");
    con->setSchema("northwind");
    sql::Statement * stmt0 = con->createStatement();
    sql::Statement * stmt1 = con->createStatement();
    sql::Statement * stmt2 = con->createStatement();
    sql::Statement * stmt3 = con->createStatement();

    sql::ResultSet * res = stmt0->executeQuery("SELECT * FROM customers");
    srand(time(0));
    while(choice != 7){
      choice = getUserInput();
      switch(choice){
        case 1:
          addCustomer(stmt0);
        break;
        case 2:
          addOrder(stmt0);
        break;
        case 3:
          removeOrder(stmt0);
        break;
        case 4:
          shipOrder(stmt0);
        break;
        case 5:
          printPending(stmt0);
        break;
        case 6:
          restockParts(stmt0);
        break;
        case 7:
          printf("Goodbye :) \n");
        break;
        default:
          printf("This is not an option. Please choose again\n");
        break;
      }
    }
    //end program with these on exit
    delete res;
    delete stmt0;
    delete stmt1;
    delete stmt2;
    delete stmt3;
    delete con;
  }
  catch(sql::SQLException &e){
    cout<<__FILE__<<__FUNCTION__<<__LINE__<<endl;
    cout<<e.what()<<e.getErrorCode()<<e.getSQLState()<<endl;
  }
}

int getUserInput(void){
  int choice;
  printf("Please slect an option using its corresponding number\n");
  printf("1. Add a customer\n");
  printf("2. Add an order\n");
  printf("3. Remove an order\n");
  printf("4. Ship an order\n");
  printf("5. Print pending order\n");
  printf("6. Restock parts\n");
  printf("7. EXIT\n");

  scanf("%d", &choice);
  return choice;
}

void restockParts(sql::Statement * stmt){
  try{
    int prodID, numStock;
    string restock;
    printf("ProductID to restock: ");
    scanf("%d", &prodID);

    printf("Number of units to stock: ");
    scanf("%d", &numStock);

    restock = "UPDATE products SET UnitsInStock = " + to_string(numStock) + " WHERE ProductID = " + to_string(prodID);

    stmt->executeUpdate(restock);

    printf("Restocked\n");

  }
  catch(sql::SQLException &e){
    cout<<__FILE__<<__FUNCTION__<<__LINE__<<endl;
    cout<<e.what()<<e.getErrorCode()<<e.getSQLState()<<endl;
  }
}

void printPending(sql::Statement * stmt){
  string cusID, compName;
  int orderID;
  string pendquery = "SELECT O.CustomerID, C.CompanyName, C.Address, C.City, C.PostalCode, O.OrderID FROM orders O, customers C WHERE O.ShippedDate IS NULL";
  sql::ResultSet * res = stmt->executeQuery(pendquery);
  printf("CustomerID |                    CompanyName                   | OrderID |");

  while(res->next()){
    cusID = res->getString("CustomerID");
    compName = res->getString("CompanyName");
    orderID = res->getInt("OrderID");
    cout<<"    "<<cusID<<"             "<<compName<<"         "<<orderID<<endl;
  }
}

void shipOrder(sql::Statement * stmt){
  try{
    int id = 0;
    string shipOrder = "";
    printf("Order number to be shipped: ");
    scanf("%d", &id);
    shipOrder = "UPDATE orders SET ShippedDate = NOW() WHERE orderID = " + to_string(id);
    stmt->executeUpdate(shipOrder);
    printf("Order shipped\n");
  }
  catch(sql::SQLException &e){
    cout<<__FILE__<<__FUNCTION__<<__LINE__<<endl;
    cout<<e.what()<<e.getErrorCode()<<e.getSQLState()<<endl;
  }
}

void removeOrder(sql::Statement * stmt){
  try{
    int id = 0;
    string deletOrderDetails = "DELETE FROM order_details WHERE orderID = " + to_string(id);
    string deleteOrder = "DELETE FROM orders WHERE orderID = " + to_string(id);
    printf("Order ID to be removed: ");
    scanf("%d", &id);

    stmt->executeUpdate(deletOrderDetails);
    stmt->executeUpdate(deleteOrder);

    printf("Order Deleted\n");
  }
  catch(sql::SQLException &e){
    cout<<__FILE__<<__FUNCTION__<<__LINE__<<endl;
    cout<<e.what()<<e.getErrorCode()<<e.getSQLState()<<endl;
  }
}

void addOrder(sql::Statement * stmt){
  int shipVia = 1 + rand() % ((3+1)-1);
  int empID = rand() % 10;
  int prodID1 = 0, amount = 0, cusID = 0, id = 0, id2 = 0, price = 0;
  string discont = "";
  string query1;
  string query2 = "SELECT MAX(OrderID) FROM orders";
  string query3 = "SELECT MAX(ID) FROM order_details";
  string query4 = "";
  string addOrder = "";
  string orderDetailsInsert = "";
  string orderInsert = "";
  sql::ResultSet * res;
  sql::ResultSet * res1;
  sql::ResultSet * res2;
  sql::ResultSet * res3;
  try{
    printf("Enter product ID you would like to order (must be a number): ");
    scanf("%d", &prodID1);
    query1 = "SELECT Discontinued FROM products WHERE ProductID = " + to_string(prodID1);

    res = stmt->executeQuery(query1);

    //get discontinue query
    while(res->next()){
      discont = res->getString("Discontinued");
    }

    if(discont.compare("y")==0){
      printf("Sorry, product is discontinued\n");
      return;
    }

    printf("Number of units to order: ");
    scanf("%d", &amount);

    printf("Customer ID: ");
    scanf("%d", &cusID);

    query4 = "SELECT Unitprice FROM products WHERE ProductID = " + to_string(prodID1);

    res1 = stmt->executeQuery(query2);
    res2 = stmt->executeQuery(query3);
    res3 = stmt->executeQuery(query4);

    addOrder = "UPDATE products SET UnitsOnOrder = UnitsOnOrder + " + to_string(amount) + " WHERE ProductID = " + to_string(prodID1);
    stmt->executeUpdate(addOrder);

    while(res1->next()){
      id = res1->getInt("MAX(OrderID)");
    }
    free(res1);

    while(res2->next()){
      id2 = res2->getInt("MAX(ID)");
    }
    free(res2);

    while(res3->next()){
      price = res3->getInt("UnitPrice");
    }
    free(res3);

    id = id + 1;
    id2 = id2 + 1;

    orderDetailsInsert = "INSERT INTO order_details (ID, OrderID, ProductID, UnitPrice, Quantity) VALUES(" + to_string(id2) + ", " + to_string(id) + ", " + to_string(prodID1) + ", " + to_string(price) + ", " + to_string(amount) + ")";
    orderInsert = "INSERT INTO orders (OrderID, CustomerID, EmployeeID, OrderDate, ShipVia) VALUES (" + to_string(id) + ", " + to_string(cusID) + ", " + to_string(empID) + ", " + to_string(shipVia) + ")";

    stmt->execute("SET foreign_key_checks = 0");
    stmt->execute(orderDetailsInsert);
    stmt->execute(orderInsert);

    printf("Order added\n");
  }
  catch(sql::SQLException &e){
    cout<<__FILE__<<__FUNCTION__<<__LINE__<<endl;
    cout<<e.what()<<e.getErrorCode()<<e.getSQLState()<<endl;
    printf("\nI dont know how to insert the datetime values\n\n");
  }

}

void addCustomer(sql::Statement * stmt){

  string customerID, companyName, contactName, contactTitle, address, city, region, postalCode, country, phone, fax, insertion;

  printf("Enter customer's ID: ");
  while(customerID.length()==0)
    getline(cin, customerID);

  printf("Enter company's name: ");
  while(companyName.length()==0)
    getline(cin, companyName);

  printf("Enter contact's name: ");
  while(contactName.length()==0)
    getline(cin, contactName);

  printf("Enter contact's title: ");
  while(contactTitle.length()==0)
    getline(cin, contactTitle);

  printf("Enter address: ");
  while(address.length()==0)
    getline(cin, address);

  printf("Enter city: ");
  while(city.length()==0)
    getline(cin, city);

  printf("Enter region: ");
  while(region.length()==0)
    getline(cin, region);

  printf("Enter postal code: ");
  while(postalCode.length()==0)
    getline(cin, postalCode);

  printf("Enter country: ");
  while(country.length()==0)
    getline(cin, country);

  printf("Enter phone number: ");
  while(phone.length()==0)
    getline(cin, phone);

  printf("Enter fax: ");
  while(fax.length()==0)
    getline(cin, fax);

  insertion = "INSERT INTO customers (CustomerID, CompanyName, ContactName, ContactTitle, Address, City, Region, PostalCode, Country, Phone, Fax) VALUES (" + customerID + ", " + companyName + ", " + contactName + ", " + contactTitle + ", " + address + ", " + city + ", " + region + ", " + postalCode + ", " + country + ", " + phone + ", " + fax + " )";
  stmt->executeUpdate(insertion);

  printf("Customer added\n\n");

}
