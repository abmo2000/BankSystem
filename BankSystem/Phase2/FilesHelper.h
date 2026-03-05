#pragma once
#include "Parser.h"
#include <fstream>
#include <iostream>
#include <vector>

// File name constants
const string CLIENTS_FILE    = "Clients.txt";
const string EMPLOYEES_FILE  = "Employee.txt";
const string ADMINS_FILE     = "Admin.txt";
const string LAST_CLIENT_ID  = "last_client_id.txt";
const string LAST_EMPLOYEE_ID= "last_employee_id.txt";
const string LAST_ADMIN_ID   = "last_admin_id.txt";

class FilesHelper {
public:
    // Save last used id to a tracking file
    static void saveLast(const string& fileName, int id) {
        ofstream f(fileName, ios::trunc);
        if (f.is_open()) f << id;
    }

    // Read last used id from a tracking file (0 if not found)
    static int getLast(const string& fileName) {
        ifstream f(fileName);
        int id = 0;
        if (f.is_open()) f >> id;
        return id;
    }

    // Append a client record to Clients.txt
    static void saveClient(Client c) {
        ofstream f(CLIENTS_FILE, ios::app);
        if (f.is_open())
            f << Parser::clientToString(c) << "\n";
        else
            cerr << "Cannot open " << CLIENTS_FILE << "\n";
    }

    // Append an employee record (generic – works for both Employee and Admin)
    static void saveEmployee(const string& fileName, const string& lastIdFile, Employee e) {
        ofstream f(fileName, ios::app);
        if (f.is_open())
            f << Parser::employeeToString(e) << "\n";
        else
            cerr << "Cannot open " << fileName << "\n";
        saveLast(lastIdFile, e.getId());
    }

    // Read all clients from Clients.txt and return as vector
    static vector<Client> getClients() {
        vector<Client> list;
        ifstream f(CLIENTS_FILE);
        string line;
        while (getline(f, line))
            if (!line.empty())
                list.push_back(Parser::parseToClient(line));
        return list;
    }

    // Read all employees from Employee.txt and return as vector
    static vector<Employee> getEmployees() {
        vector<Employee> list;
        ifstream f(EMPLOYEES_FILE);
        string line;
        while (getline(f, line))
            if (!line.empty())
                list.push_back(Parser::parseToEmployee(line));
        return list;
    }

    // Read all admins from Admin.txt and return as vector
    static vector<Admin> getAdmins() {
        vector<Admin> list;
        ifstream f(ADMINS_FILE);
        string line;
        while (getline(f, line))
            if (!line.empty())
                list.push_back(Parser::parseToAdmin(line));
        return list;
    }

    // Clear a data file and its id-tracking file
    static void clearFile(const string& fileName, const string& lastIdFile) {
        ofstream f(fileName, ios::trunc);
        ofstream lid(lastIdFile, ios::trunc);
    }

    // ── Helpers used by FileManager to rewrite entire file ──────────────
    static void rewriteClients(const vector<Client>& list) {
        ofstream f(CLIENTS_FILE, ios::trunc);
        for (const auto& c : list)
            f << Parser::clientToString(c) << "\n";
    }

    static void rewriteEmployees(const vector<Employee>& list) {
        ofstream f(EMPLOYEES_FILE, ios::trunc);
        for (const auto& e : list)
            f << Parser::employeeToString(e) << "\n";
    }

    static void rewriteAdmins(const vector<Admin>& list) {
        ofstream f(ADMINS_FILE, ios::trunc);
        for (const auto& a : list)
            f << Parser::adminToString(a) << "\n";
    }
};
