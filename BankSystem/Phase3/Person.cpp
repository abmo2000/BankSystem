#include "Person.h"

#include "Validation.h"

Person::Person() : id(0), name("Unknown User"), password("password1") {}

Person::Person(int id, const std::string& name, const std::string& password) : Person() {
    this->id = id;
    setName(name);
    setPassword(password);
}

int Person::getId() const {
    return id;
}

const std::string& Person::getName() const {
    return name;
}

const std::string& Person::getPassword() const {
    return password;
}

void Person::setId(int newId) {
    id = newId;
}

bool Person::setName(const std::string& newName) {
    if (!Validation::isValidName(newName)) {
        return false;
    }

    name = newName;
    return true;
}

bool Person::setPassword(const std::string& newPassword) {
    if (!Validation::isValidPassword(newPassword)) {
        return false;
    }

    password = newPassword;
    return true;
}