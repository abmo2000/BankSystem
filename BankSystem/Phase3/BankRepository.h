#pragma once

#include "Admin.h"
#include "Client.h"
#include "Employee.h"

#include <filesystem>
#include <string>
#include <vector>

struct OperationResult {
    bool success;
    std::string message;
};

class BankRepository {
private:
    std::filesystem::path phaseRoot;
    std::filesystem::path dataRoot;
    std::filesystem::path seedRoot;

    std::vector<Client> clients;
    std::vector<Employee> employees;
    std::vector<Admin> admins;

    int lastClientId;
    int lastEmployeeId;
    int lastAdminId;

    void ensureDataFiles();
    void bootstrapDataFile(const std::string& fileName, const std::string& fallbackContent = "");
    void bootstrapIdFile(const std::string& fileName);
    void loadData();
    void saveClients() const;
    void saveEmployees() const;
    void saveAdmins() const;
    void saveCounters() const;

public:
    explicit BankRepository(const std::filesystem::path& root);

    const std::vector<Client>& getClients() const;
    const std::vector<Employee>& getEmployees() const;
    const std::vector<Admin>& getAdmins() const;

    Client* findClientById(int id);
    Employee* findEmployeeById(int id);
    Admin* findAdminById(int id);
    const Client* findClientById(int id) const;
    const Employee* findEmployeeById(int id) const;
    const Admin* findAdminById(int id) const;

    OperationResult authenticateClient(int id, const std::string& password, const Client*& client) const;
    OperationResult authenticateEmployee(int id, const std::string& password, const Employee*& employee) const;
    OperationResult authenticateAdmin(int id, const std::string& password, const Admin*& admin) const;

    OperationResult createClient(const std::string& name, const std::string& password, double balance);
    OperationResult updateClient(int id, const std::string& name, const std::string& password, double balance);
    OperationResult depositToClient(int clientId, double amount);
    OperationResult withdrawFromClient(int clientId, double amount);
    OperationResult transferBetweenClients(int senderId, int recipientId, double amount);

    OperationResult createEmployee(const std::string& name, const std::string& password, double salary);
    OperationResult updateEmployee(int id, const std::string& name, const std::string& password, double salary);

    int totalUsers() const;
    double totalClientBalances() const;
};