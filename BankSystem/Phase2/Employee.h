#pragma once
#include "Person.h"
#include "Client.h"
#include <vector>

class Employee : public Person {
protected:
    double salary;
    vector<Client> clients;   // in-memory client list managed by this employee

public:
    Employee() : Person(), salary(5000.0) {}
    Employee(int id, string name, string password, double salary)
        : Person(id, name, password), salary(salary) {}

    // Getter / Setter
    double getSalary() const { return salary; }
    bool   setSalary(double salary);

    // Client management
    void    addClient(Client& client);
    Client* searchClient(int id);
    void    listClient() const;
    void    editClient(int id, string name, string password, double balance);

    // Accessor for full list (used by FileManager)
    vector<Client>& getClients() { return clients; }
    void setClients(const vector<Client>& c) { clients = c; }

    void display() const override;
};
