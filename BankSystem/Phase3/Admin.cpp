#include "Admin.h"

Admin::Admin() : Employee() {}

Admin::Admin(int id, const std::string& name, const std::string& password, double salary)
    : Employee(id, name, password, salary) {}