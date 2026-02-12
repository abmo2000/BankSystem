#ifndef ADMIN_H
#define ADMIN_H

#include "Employee.h"

class Admin : public Employee {
public:
    // Constructors
    Admin();
    Admin(int id, string name, string password, double salary);
    
    // Display
    void display() override;
};

#endif