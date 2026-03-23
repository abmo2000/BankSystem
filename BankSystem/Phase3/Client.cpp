#include "Client.h"

#include "Validation.h"

Client::Client() : Person(), balance(1500.0) {}

Client::Client(int id, const std::string& name, const std::string& password, double balance)
    : Person(id, name, password), balance(1500.0) {
    setBalance(balance);
}

double Client::getBalance() const {
    return balance;
}

bool Client::setBalance(double newBalance) {
    if (!Validation::isValidBalance(newBalance)) {
        return false;
    }

    balance = newBalance;
    return true;
}

bool Client::deposit(double amount) {
    if (!Validation::isPositiveAmount(amount)) {
        return false;
    }

    balance += amount;
    return true;
}

bool Client::withdraw(double amount) {
    if (!Validation::isPositiveAmount(amount)) {
        return false;
    }

    if (balance - amount < 1500.0) {
        return false;
    }

    balance -= amount;
    return true;
}

bool Client::transferTo(double amount, Client& recipient) {
    if (!withdraw(amount)) {
        return false;
    }

    return recipient.deposit(amount);
}