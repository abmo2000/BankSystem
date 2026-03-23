#pragma once

#include <cctype>
#include <string>

class Validation {
public:
    static bool isValidName(const std::string& name) {
        if (name.size() < 3 || name.size() > 20) {
            return false;
        }

        for (unsigned char ch : name) {
            if (!std::isalpha(ch) && ch != ' ') {
                return false;
            }
        }

        return true;
    }

    static bool isValidPassword(const std::string& password) {
        if (password.size() < 8 || password.size() > 20) {
            return false;
        }

        for (unsigned char ch : password) {
            if (std::isspace(ch)) {
                return false;
            }
        }

        return true;
    }

    static bool isValidBalance(double balance) {
        return balance >= 1500.0;
    }

    static bool isValidSalary(double salary) {
        return salary >= 5000.0;
    }

    static bool isPositiveAmount(double amount) {
        return amount > 0.0;
    }
};