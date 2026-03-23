#pragma once

#include "Employee.h"

class Admin : public Employee {
public:
    Admin();
    Admin(int id, const std::string& name, const std::string& password, double salary);
};