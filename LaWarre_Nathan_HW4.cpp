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
        break;
        case 4:
        break;
        case 5:
        break;
        case 6:
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

void addOrder(sql::Statement * stmt){
  int prodID1 = 0, amount = 0, cusID = 0;
  string discont = "";
  string query1;
  sql::ResultSet * res;
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