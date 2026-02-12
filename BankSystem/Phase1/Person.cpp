#include "Person.h"
#include "Validation.h"

// Default Constructor
Person::Person() : id(0), name(""), password("") {}

// Parameterized Constructor
Person::Person(int id, string name, string password) {
    this->id = id;
    setName(name);
    setPassword(password);
}

// Setters
void Person::setId(int id) {
    this->id = id;
}

void Person::setName(string name) {
    if (Validation::isValidName(name)) {
        this->name = name;
    } else {
        cout << "Invalid name! Name must be alphabetic and 3-20 characters." << endl;
        this->name = "";
    }
}

void Person::setPassword(string password) {
    if (Validation::isValidPassword(password)) {
        this->password = password;
    } else {
        cout << "Invalid password! Password must be 8-20 characters without spaces." << endl;
        this->password = "";
    }
}

// Getters
int Person::getId() {
    return id;
}

string Person::getName() {
    return name;
}

string Person::getPassword() {
    return password;
}

// Display
void Person::display() {
    cout << "ID: " << id << endl;
    cout << "Name: " << name << endl;
}