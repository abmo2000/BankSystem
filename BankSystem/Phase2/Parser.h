#pragma once
#include "Client.h"
#include "Employee.h"
#include "Admin.h"
#include <vector>
#include <string>
#include <sstream>

// Delimiter used when writing records: comma
class Parser {
public:
    // Split a CSV line into tokens
    static vector<string> split(string line) {
        vector<string> tokens;
        stringstream ss(line);
        string token;
        while (getline(ss, token, ','))
            tokens.push_back(token);
        return tokens;
    }

    // Format: id,name,password,balance
    static Client parseToClient(string line) {
        auto t = split(line);
        if (t.size() < 4) return Client();
        return Client(stoi(t[0]), t[1], t[2], stod(t[3]));
    }

    // Format: id,name,password,salary
    static Employee parseToEmployee(string line) {
        auto t = split(line);
        if (t.size() < 4) return Employee();
        return Employee(stoi(t[0]), t[1], t[2], stod(t[3]));
    }

    // Format: id,name,password,salary
    static Admin parseToAdmin(string line) {
        auto t = split(line);
        if (t.size() < 4) return Admin();
        return Admin(stoi(t[0]), t[1], t[2], stod(t[3]));
    }

    // Serialisers (used by FilesHelper)
    static string clientToString(const Client& c) {
        return to_string(c.getId()) + "," + c.getName() + "," +
               c.getPassword() + "," + to_string(c.getBalance());
    }

    static string employeeToString(const Employee& e) {
        return to_string(e.getId()) + "," + e.getName() + "," +
               e.getPassword() + "," + to_string(e.getSalary());
    }

    static string adminToString(const Admin& a) {
        return to_string(a.getId()) + "," + a.getName() + "," +
               a.getPassword() + "," + to_string(a.getSalary());
    }
};
