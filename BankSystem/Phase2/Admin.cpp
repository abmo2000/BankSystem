#include "Admin.h"
#include "Validation.h"
#include <iostream>
using namespace std;

void Admin::addEmployee(Employee& employee) {
    employees.push_back(employee);
    cout << "Employee added successfully.\n";
}

Employee* Admin::searchEmployee(int id) {
    for (auto& e : employees)
        if (e.getId() == id) return &e;
    return nullptr;
}

void Admin::editEmployee(int id, string name, string password, double salary) {
    Employee* e = searchEmployee(id);
    if (!e) {
        cout << "Employee not found.\n";
        return;
    }
    if (!e->setName(name))         cout << "Invalid name.\n";
    if (!e->setPassword(password)) cout << "Invalid password.\n";
    if (!e->setSalary(salary))     cout << "Invalid salary.\n";
    cout << "Employee updated.\n";
}

void Admin::listEmployee() const {
    if (employees.empty()) {
        cout << "No employees found.\n";
        return;
    }
    cout << "=== Employee List ===\n";
    for (const auto& e : employees) {
        e.display();
        cout << "--------------------\n";
    }
}

void Admin::display() const {
    cout << "[Admin]\n";
    Employee::display();
}
