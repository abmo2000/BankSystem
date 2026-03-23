#pragma once

#include <string>

class Person {
protected:
    int id;
    std::string name;
    std::string password;

public:
    Person();
    Person(int id, const std::string& name, const std::string& password);
    virtual ~Person() = default;

    int getId() const;
    const std::string& getName() const;
    const std::string& getPassword() const;

    void setId(int newId);
    bool setName(const std::string& newName);
    bool setPassword(const std::string& newPassword);
};