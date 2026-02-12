#include "Client.h"
#include "Validation.h"
#include <iostream>
#include <iomanip>
using namespace std;

// Default Constructor
Client::Client() : Person(), balance(0) {}

// Parameterized Constructor
Client::Client(int id, string name, string password, double balance) 
    : Person(id, name, password) {
    setBalance(balance);
}

// Setter
void Client::setBalance(double balance) {
    if (Validation::isValidBalance(balance)) {
        this->balance = balance;
    } else {
        cout << "Invalid balance! Minimum balance is 1500." << endl;
        this->balance = 0;
    }
}

// Getter
double Client::getBalance() {
    return balance;
}

// Deposit money
void Client::deposit(double amount) {
    if (amount > 0) {
        balance += amount;
        cout << "Deposited: " << fixed << setprecision(2) << amount << " EGP" << endl;
        cout << "New Balance: " << balance << " EGP" << endl;
    } else {
        cout << "Invalid amount! Amount must be positive." << endl;
    }
}

// Withdraw money
void Client::withdraw(double amount) {
    if (amount <= 0) {
        cout << "Invalid amount! Amount must be positive." << endl;
        return;
    }
    
    if (balance - amount < 1500) {
        cout << "Insufficient balance! Minimum balance must be 1500 EGP." << endl;
        return;
    }
    
    balance -= amount;
    cout << "Withdrawn: " << fixed << setprecision(2) << amount << " EGP" << endl;
    cout << "New Balance: " << balance << " EGP" << endl;
}

// Transfer money to another client
void Client::transferTo(double amount, Client& recipient) {
    if (amount <= 0) {
        cout << "Invalid amount! Amount must be positive." << endl;
        return;
    }
    
    if (balance - amount < 1500) {
        cout << "Insufficient balance! Minimum balance must be 1500 EGP." << endl;
        return;
    }
    
    balance -= amount;
    recipient.balance += amount;
    
    cout << "Transferred: " << fixed << setprecision(2) << amount << " EGP" << endl;
    cout << "To: " << recipient.getName() << endl;
    cout << "Your New Balance: " << balance << " EGP" << endl;
}

// Check balance
void Client::checkBalance() {
    cout << "Current Balance: " << fixed << setprecision(2) << balance << " EGP" << endl;
}

// Display client info
void Client::display() {
    cout << "========== Client Information ==========" << endl;
    Person::display();
    cout << "Balance: " << fixed << setprecision(2) << balance << " EGP" << endl;
    cout << "========================================" << endl;
}