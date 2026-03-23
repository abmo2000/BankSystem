#pragma once

#include "Person.h"

class Employee : public Person {
protected:
    double salary;

public:
    Employee();
    Employee(int id, const std::string& name, const std::string& password, double salary);

    double getSalary() const;
    bool setSalary(double newSalary);
};