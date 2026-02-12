#ifndef EMPLOYEE_H
#define EMPLOYEE_H

#include "Person.h"

class Employee : public Person {
protected:
    double salary;

public:
    // Constructors
    Employee();
    Employee(int id, string name, string password, double salary);
    
    // Setter
    void setSalary(double salary);
    
    // Getter
    double getSalary();
    
    // Display
    void display() override;
};

#endif