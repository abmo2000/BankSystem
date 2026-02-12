#ifndef VALIDATION_H
#define VALIDATION_H

#include <string>
using namespace std;

class Validation {
public:
    // Check if name is valid (alphabetic, 3-20 chars)
    static bool isValidName(string name);
    
    // Check if password is valid (8-20 chars, no spaces)
    static bool isValidPassword(string password);
    
    // Check if balance meets minimum requirement
    static bool isValidBalance(double balance);
    
    // Check if salary meets minimum requirement
    static bool isValidSalary(double salary);
    
private:
    // Helper: check if string contains only alphabetic characters and spaces
    static bool isAlphabetic(string str);
};

#endif