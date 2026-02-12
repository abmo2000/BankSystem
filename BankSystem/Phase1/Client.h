#ifndef CLIENT_H
#define CLIENT_H

#include "Person.h"

class Client : public Person {
private:
    double balance;

public:
    // Constructors
    Client();
    Client(int id, string name, string password, double balance);
    
    // Setter
    void setBalance(double balance);
    
    // Getter
    double getBalance();
    
    // Banking Operations
    void deposit(double amount);
    void withdraw(double amount);
    void transferTo(double amount, Client& recipient);
    void checkBalance();
    
    // Display
    void display() override;
};

#endif