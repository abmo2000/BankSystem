#pragma once
#include "Person.h"

class Client : public Person {
private:
    double balance;

public:
    Client() : Person(), balance(1500.0) {}
    Client(int id, string name, string password, double balance)
        : Person(id, name, password), balance(balance) {}

    // Getter / Setter
    double getBalance() const { return balance; }
    bool   setBalance(double balance);

    // Operations
    void deposit(double amount);
    bool withdraw(double amount);
    bool transferTo(double amount, Client& recipient);
    void checkBalance() const;

    void display() const override;
};
