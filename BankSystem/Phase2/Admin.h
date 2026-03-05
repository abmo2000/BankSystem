#pragma once
#include "Employee.h"

class Admin : public Employee {
private:
    vector<Employee> employees;   // in-memory employee list managed by this admin

public:
    Admin() : Employee() {}
    Admin(int id, string name, string password, double salary)
        : Employee(id, name, password, salary) {}

    // Employee management
    void      addEmployee(Employee& employee);
    Employee* searchEmployee(int id);
    void      editEmployee(int id, string name, string password, double salary);
    void      listEmployee() const;

    // Accessor for full list (used by FileManager)
    vector<Employee>& getEmployees() { return employees; }
    void setEmployees(const vector<Employee>& e) { employees = e; }

    void display() const override;
};
