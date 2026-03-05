#pragma once
#include <string>
using namespace std;

class Person {
protected:
    int id;
    string name;
    string password;

public:
    Person() : id(0), name(""), password("") {}
    Person(int id, string name, string password)
        : id(id), name(name), password(password) {}

    // Getters
    int getId() const { return id; }
    string getName() const { return name; }
    string getPassword() const { return password; }

    // Setters
    void setId(int id) { this->id = id; }
    bool setName(const string& name);
    bool setPassword(const string& password);

    virtual void display() const;
};
