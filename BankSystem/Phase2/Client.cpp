#include "Client.h"
#include "Validation.h"
#include <iostream>
using namespace std;

bool Client::setBalance(double balance) {
    if (!Validation::isValidBalance(balance)) return false;
    this->balance = balance;
    return true;
}

void Client::deposit(double amount) {
    if (!Validation::isPositiveAmount(amount)) {
        cout << "Invalid deposit amount.\n";
        return;
    }
    balance += amount;
    cout << "Deposited " << amount << ". New balance: " << balance << "\n";
}

bool Client::withdraw(double amount) {
    if (!Validation::isPositiveAmount(amount)) {
        cout << "Invalid withdrawal amount.\n";
        return false;
    }
    if (balance - amount < 1500.0) {
        cout << "Insufficient funds. Minimum balance is 1500.\n";
        return false;
    }
    balance -= amount;
    cout << "Withdrew " << amount << ". New balance: " << balance << "\n";
    return true;
}

bool Client::transferTo(double amount, Client& recipient) {
    if (withdraw(amount)) {
        recipient.deposit(amount);
        return true;
    }
    return false;
}

void Client::checkBalance() const {
    cout << "Current balance: " << balance << "\n";
}

void Client::display() const {
    Person::display();
    cout << "Balance: " << balance << "\n";
}
