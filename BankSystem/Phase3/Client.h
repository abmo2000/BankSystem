#pragma once

#include "Person.h"

class Client : public Person {
private:
    double balance;

public:
    Client();
    Client(int id, const std::string& name, const std::string& password, double balance);

    double getBalance() const;
    bool setBalance(double newBalance);

    bool deposit(double amount);
    bool withdraw(double amount);
    bool transferTo(double amount, Client& recipient);
};