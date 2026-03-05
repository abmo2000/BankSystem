#include "Person.h"
#include "Validation.h"
#include <iostream>

bool Person::setName(const string& name) {
    if (!Validation::isValidName(name)) return false;
    this->name = name;
    return true;
}

bool Person::setPassword(const string& password) {
    if (!Validation::isValidPassword(password)) return false;
    this->password = password;
    return true;
}

void Person::display() const {
    cout << "ID: "       << id       << "\n"
         << "Name: "     << name     << "\n"
         << "Password: " << password << "\n";
}
