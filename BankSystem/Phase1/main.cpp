#include <iostream>
#include "Client.h"
#include "Employee.h"
#include "Admin.h"
using namespace std;

int main() {
    cout << "========== Bank System - Phase 1 Testing ==========" << endl << endl;
    
    // Test Client
    cout << "***** Testing Client *****" << endl;
    Client client1(1, "Ahmed Hassan", "pass1234", 5000);
    client1.display();
    
    cout << "\n--- Testing Deposit ---" << endl;
    client1.deposit(1000);
    
    cout << "\n--- Testing Withdraw ---" << endl;
    client1.withdraw(500);
    
    cout << "\n--- Testing Check Balance ---" << endl;
    client1.checkBalance();
    
    cout << "\n--- Testing Transfer ---" << endl;
    Client client2(2, "Sara Ali", "mypass123", 3000);
    client1.transferTo(1000, client2);
    client2.checkBalance();
    
    cout << "\n\n***** Testing Employee *****" << endl;
    Employee emp1(100, "Khaled Ibrahim", "emp12345", 7000);
    emp1.display();
    
    cout << "\n\n***** Testing Admin *****" << endl;
    Admin admin1(1000, "Omar Yasser", "admin9999", 10000);
    admin1.display();
    
    // Test Validation
    cout << "\n\n***** Testing Validation *****" << endl;
    
    cout << "\n--- Invalid Name (too short) ---" << endl;
    Client client3(3, "Ab", "pass1234", 2000);
    
    cout << "\n--- Invalid Password (has spaces) ---" << endl;
    Client client4(4, "Mohamed Ali", "pass 1234", 2000);
    
    cout << "\n--- Invalid Balance (less than 1500) ---" << endl;
    Client client5(5, "Nour Ahmed", "pass1234", 1000);
    
    cout << "\n--- Invalid Salary (less than 5000) ---" << endl;
    Employee emp2(101, "Ali Hassan", "emp12345", 3000);
    
    cout << "\n========== Testing Complete ==========" << endl;
    
    return 0;
}