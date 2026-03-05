#include "FileManager.h"
#include <iostream>
using namespace std;

int main() {
    FileManager fm;

    // --- Clients ---
    Client c1(1, "Alice Smith",  "pass1234", 5000.0);
    Client c2(2, "Bob Johnson",  "secureABC", 2000.0);
    fm.addClient(c1);
    fm.addClient(c2);

    cout << "\n--- All Clients ---\n";
    fm.getAllClients();

    // --- Employees ---
    Employee e1(1, "John Doe",   "emp12345", 8000.0);
    fm.addEmployee(e1);

    cout << "\n--- All Employees ---\n";
    fm.getAllEmployees();

    // --- Admins ---
    Admin a1(1, "Sara Admin",  "admin999", 12000.0);
    fm.addAdmin(a1);

    cout << "\n--- All Admins ---\n";
    fm.getAllAdmins();

    // --- Employee client management ---
    Employee emp(1, "John Doe", "emp12345", 8000.0);
    emp.setClients(FilesHelper::getClients());

    cout << "\n--- Employee lists clients ---\n";
    emp.listClient();

    cout << "\n--- Search client ID 1 ---\n";
    Client* found = emp.searchClient(1);
    if (found) found->display();

    cout << "\n--- Edit client ID 2 ---\n";
    emp.editClient(2, "Bob Updated", "newpass1", 3000.0);
    FilesHelper::rewriteClients(emp.getClients());

    cout << "\n--- Clients after edit ---\n";
    fm.getAllClients();

    return 0;
}
