#include "Employee.h"
#include "Validation.h"
#include <iostream>
using namespace std;

bool Employee::setSalary(double salary) {
    if (!Validation::isValidSalary(salary)) return false;
    this->salary = salary;
    return true;
}

void Employee::addClient(Client& client) {
    clients.push_back(client);
    cout << "Client added successfully.\n";
}

Client* Employee::searchClient(int id) {
    for (auto& c : clients)
        if (c.getId() == id) return &c;
    return nullptr;
}

void Employee::listClient() const {
    if (clients.empty()) {
        cout << "No clients found.\n";
        return;
    }
    cout << "=== Client List ===\n";
    for (const auto& c : clients) {
        c.display();
        cout << "-------------------\n";
    }
}

void Employee::editClient(int id, string name, string password, double balance) {
    Client* c = searchClient(id);
    if (!c) {
        cout << "Client not found.\n";
        return;
    }
    if (!c->setName(name))         cout << "Invalid name.\n";
    if (!c->setPassword(password)) cout << "Invalid password.\n";
    if (!c->setBalance(balance))   cout << "Invalid balance.\n";
    cout << "Client updated.\n";
}

void Employee::display() const {
    Person::display();
    cout << "Salary: " << salary << "\n";
}
