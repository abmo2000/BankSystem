#pragma once
#include <string>
#include <cctype>
using namespace std;

class Validation {
public:
    // Name: alphabetic only, 3–20 chars
    static bool isValidName(const string& name) {
        if (name.size() < 3 || name.size() > 20) return false;
        for (char c : name)
            if (!isalpha(c) && c != ' ') return false;
        return true;
    }

    // Password: 8–20 chars, no spaces
    static bool isValidPassword(const string& pwd) {
        if (pwd.size() < 8 || pwd.size() > 20) return false;
        for (char c : pwd)
            if (isspace(c)) return false;
        return true;
    }

    static bool isValidBalance(double balance) {
        return balance >= 1500.0;
    }

    static bool isValidSalary(double salary) {
        return salary >= 5000.0;
    }

    static bool isPositiveAmount(double amount) {
        return amount > 0;
    }
};
