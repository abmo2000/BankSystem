#include "Employee.h"

#include "Validation.h"

Employee::Employee() : Person(), salary(5000.0) {}

Employee::Employee(int id, const std::string& name, const std::string& password, double salary)
    : Person(id, name, password), salary(5000.0) {
    setSalary(salary);
}

double Employee::getSalary() const {
    return salary;
}

bool Employee::setSalary(double newSalary) {
    if (!Validation::isValidSalary(newSalary)) {
        return false;
    }

    salary = newSalary;
    return true;
}