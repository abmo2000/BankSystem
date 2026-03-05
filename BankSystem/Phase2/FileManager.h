#pragma once
#include "DataSourceInterface.h"
#include "FilesHelper.h"
#include <iostream>

class FileManager : public DataSourceInterface {
public:
    // ── Add ─────────────────────────────────────────────────────────────
    void addClient(Client obj) override {
        FilesHelper::saveClient(obj);
        FilesHelper::saveLast(LAST_CLIENT_ID, obj.getId());
    }

    void addEmployee(Employee obj) override {
        FilesHelper::saveEmployee(EMPLOYEES_FILE, LAST_EMPLOYEE_ID, obj);
    }

    void addAdmin(Admin obj) override {
        // Admins are also stored with employee serialisation format
        ofstream f(ADMINS_FILE, ios::app);
        if (f.is_open())
            f << Parser::adminToString(obj) << "\n";
        FilesHelper::saveLast(LAST_ADMIN_ID, obj.getId());
    }

    // ── Get all (prints to console) ──────────────────────────────────────
    void getAllClients() override {
        auto list = FilesHelper::getClients();
        if (list.empty()) { cout << "No clients found.\n"; return; }
        cout << "=== All Clients ===\n";
        for (auto& c : list) { c.display(); cout << "---\n"; }
    }

    void getAllEmployees() override {
        auto list = FilesHelper::getEmployees();
        if (list.empty()) { cout << "No employees found.\n"; return; }
        cout << "=== All Employees ===\n";
        for (auto& e : list) { e.display(); cout << "---\n"; }
    }

    void getAllAdmins() override {
        auto list = FilesHelper::getAdmins();
        if (list.empty()) { cout << "No admins found.\n"; return; }
        cout << "=== All Admins ===\n";
        for (auto& a : list) { a.display(); cout << "---\n"; }
    }

    // ── Remove all ───────────────────────────────────────────────────────
    void removeAllClients() override {
        FilesHelper::clearFile(CLIENTS_FILE, LAST_CLIENT_ID);
        cout << "All clients removed.\n";
    }

    void removeAllEmployees() override {
        FilesHelper::clearFile(EMPLOYEES_FILE, LAST_EMPLOYEE_ID);
        cout << "All employees removed.\n";
    }

    void removeAllAdmins() override {
        FilesHelper::clearFile(ADMINS_FILE, LAST_ADMIN_ID);
        cout << "All admins removed.\n";
    }

    // ── Utility: next auto-increment IDs ────────────────────────────────
    int nextClientId()   { return FilesHelper::getLast(LAST_CLIENT_ID)   + 1; }
    int nextEmployeeId() { return FilesHelper::getLast(LAST_EMPLOYEE_ID) + 1; }
    int nextAdminId()    { return FilesHelper::getLast(LAST_ADMIN_ID)    + 1; }
};
