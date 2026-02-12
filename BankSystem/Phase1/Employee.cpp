#include "Employee.h"
#include "Validation.h"
#include <iostream>
#include <iomanip>
using namespace std;

// Default Constructor
Employee::Employee() : Person(), salary(0) {}

// Parameterized Constructor
Employee::Employee(int id, string name, string password, double salary) 
    : Person(id, name, password) {
    setSalary(salary);
}

// Setter
void Employee::setSalary(double salary) {
    if (Validation::isValidSalary(salary)) {
        this->salary = salary;
    } else {
        cout << "Invalid salary! Minimum salary is 5000." << endl;
        this->salary = 0;
    }
}

// Getter
double Employee::getSalary() {
    return salary;
}

// Display employee info
void Employee::display() {
    cout << "========== Employee Information ==========" << endl;
    Person::display();
    cout << "Salary: " << fixed << setprecision(2) << salary << " EGP" << endl;
    cout << "==========================================" << endl;
}