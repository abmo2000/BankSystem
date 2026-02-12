#include "Validation.h"
#include <cctype>

bool Validation::isValidName(string name) {
    // Check length
    if (name.length() < 3 || name.length() > 20) {
        return false;
    }
    
    // Check if alphabetic
    return isAlphabetic(name);
}

bool Validation::isValidPassword(string password) {
    // Check length
    if (password.length() < 8 || password.length() > 20) {
        return false;
    }
    
    // Check for spaces
    for (char c : password) {
        if (c == ' ') {
            return false;
        }
    }
    
    return true;
}

bool Validation::isValidBalance(double balance) {
    return balance >= 1500;
}

bool Validation::isValidSalary(double salary) {
    return salary >= 5000;
}

bool Validation::isAlphabetic(string str) {
    for (char c : str) {
        if (!isalpha(c) && c != ' ') {
            return false;
        }
    }
    return true;
}