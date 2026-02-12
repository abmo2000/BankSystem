#ifndef PERSON_H
#define PERSON_H

#include <string>
#include <iostream>
using namespace std;

class Person {
protected:
    int id;
    string name;
    string password;

public:
    // Constructors
    Person();
    Person(int id, string name, string password);
    
    // Setters
    void setId(int id);
    void setName(string name);
    void setPassword(string password);
    
    // Getters
    int getId();
    string getName();
    string getPassword();
    
    // Display
    virtual void display();
    
    // Destructor
    virtual ~Person() {}
};

#endif