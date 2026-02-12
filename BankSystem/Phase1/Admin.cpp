#include "Admin.h"
#include <iostream>
#include <iomanip>
using namespace std;

// Default Constructor
Admin::Admin() : Employee() {}

// Parameterized Constructor
Admin::Admin(int id, string name, string password, double salary) 
    : Employee(id, name, password, salary) {}

// Display admin info
void Admin::display() {
    cout << "========== Admin Information ==========" << endl;
    cout << "ID: " << getId() << endl;
    cout << "Name: " << getName() << endl;
    cout << "Salary: " << fixed << setprecision(2) << getSalary() << " EGP" << endl;
    cout << "=======================================" << endl;
}