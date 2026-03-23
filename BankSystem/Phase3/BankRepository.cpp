#include "BankRepository.h"

#include "Parser.h"
#include "Validation.h"

#include <fstream>
#include <numeric>

namespace fs = std::filesystem;

namespace {
int readIdValue(const fs::path& filePath) {
    std::ifstream input(filePath);
    int value = 0;
    if (input.is_open()) {
        input >> value;
    }
    return value;
}

void writeIdValue(const fs::path& filePath, int value) {
    std::ofstream output(filePath, std::ios::trunc);
    output << value;
}
}

BankRepository::BankRepository(const fs::path& root)
    : phaseRoot(root),
      dataRoot(root / "data"),
      seedRoot(root.parent_path() / "Phase2"),
      lastClientId(0),
      lastEmployeeId(0),
      lastAdminId(0) {
    ensureDataFiles();
    loadData();

    if (admins.empty()) {
        admins.emplace_back(1, "System Admin", "admin123", 12000.0);
        lastAdminId = 1;
        saveAdmins();
        saveCounters();
    }
}

void BankRepository::ensureDataFiles() {
    fs::create_directories(dataRoot);
    bootstrapDataFile("Clients.txt");
    bootstrapDataFile("Employee.txt");
    bootstrapDataFile("Admin.txt");
    bootstrapIdFile("last_client_id.txt");
    bootstrapIdFile("last_employee_id.txt");
    bootstrapIdFile("last_admin_id.txt");
}

void BankRepository::bootstrapDataFile(const std::string& fileName, const std::string& fallbackContent) {
    const fs::path target = dataRoot / fileName;
    if (fs::exists(target)) {
        return;
    }

    const fs::path seed = seedRoot / fileName;
    if (fs::exists(seed)) {
        fs::copy_file(seed, target, fs::copy_options::overwrite_existing);
        return;
    }

    std::ofstream output(target, std::ios::trunc);
    output << fallbackContent;
}

void BankRepository::bootstrapIdFile(const std::string& fileName) {
    const fs::path target = dataRoot / fileName;
    if (fs::exists(target)) {
        return;
    }

    const fs::path seed = seedRoot / fileName;
    if (fs::exists(seed)) {
        fs::copy_file(seed, target, fs::copy_options::overwrite_existing);
        return;
    }

    writeIdValue(target, 0);
}

void BankRepository::loadData() {
    clients.clear();
    employees.clear();
    admins.clear();

    {
        std::ifstream input(dataRoot / "Clients.txt");
        std::string line;
        while (std::getline(input, line)) {
            if (!line.empty()) {
                clients.push_back(Parser::parseClient(line));
            }
        }
    }

    {
        std::ifstream input(dataRoot / "Employee.txt");
        std::string line;
        while (std::getline(input, line)) {
            if (!line.empty()) {
                employees.push_back(Parser::parseEmployee(line));
            }
        }
    }

    {
        std::ifstream input(dataRoot / "Admin.txt");
        std::string line;
        while (std::getline(input, line)) {
            if (!line.empty()) {
                admins.push_back(Parser::parseAdmin(line));
            }
        }
    }

    lastClientId = readIdValue(dataRoot / "last_client_id.txt");
    lastEmployeeId = readIdValue(dataRoot / "last_employee_id.txt");
    lastAdminId = readIdValue(dataRoot / "last_admin_id.txt");

    for (const Client& client : clients) {
        if (client.getId() > lastClientId) {
            lastClientId = client.getId();
        }
    }

    for (const Employee& employee : employees) {
        if (employee.getId() > lastEmployeeId) {
            lastEmployeeId = employee.getId();
        }
    }

    for (const Admin& admin : admins) {
        if (admin.getId() > lastAdminId) {
            lastAdminId = admin.getId();
        }
    }

    saveCounters();
}

void BankRepository::saveClients() const {
    std::ofstream output(dataRoot / "Clients.txt", std::ios::trunc);
    for (const Client& client : clients) {
        output << Parser::toClientString(client) << '\n';
    }
}

void BankRepository::saveEmployees() const {
    std::ofstream output(dataRoot / "Employee.txt", std::ios::trunc);
    for (const Employee& employee : employees) {
        output << Parser::toEmployeeString(employee) << '\n';
    }
}

void BankRepository::saveAdmins() const {
    std::ofstream output(dataRoot / "Admin.txt", std::ios::trunc);
    for (const Admin& admin : admins) {
        output << Parser::toAdminString(admin) << '\n';
    }
}

void BankRepository::saveCounters() const {
    writeIdValue(dataRoot / "last_client_id.txt", lastClientId);
    writeIdValue(dataRoot / "last_employee_id.txt", lastEmployeeId);
    writeIdValue(dataRoot / "last_admin_id.txt", lastAdminId);
}

const std::vector<Client>& BankRepository::getClients() const {
    return clients;
}

const std::vector<Employee>& BankRepository::getEmployees() const {
    return employees;
}

const std::vector<Admin>& BankRepository::getAdmins() const {
    return admins;
}

Client* BankRepository::findClientById(int id) {
    for (Client& client : clients) {
        if (client.getId() == id) {
            return &client;
        }
    }
    return nullptr;
}

Employee* BankRepository::findEmployeeById(int id) {
    for (Employee& employee : employees) {
        if (employee.getId() == id) {
            return &employee;
        }
    }
    return nullptr;
}

Admin* BankRepository::findAdminById(int id) {
    for (Admin& admin : admins) {
        if (admin.getId() == id) {
            return &admin;
        }
    }
    return nullptr;
}

const Client* BankRepository::findClientById(int id) const {
    for (const Client& client : clients) {
        if (client.getId() == id) {
            return &client;
        }
    }
    return nullptr;
}

const Employee* BankRepository::findEmployeeById(int id) const {
    for (const Employee& employee : employees) {
        if (employee.getId() == id) {
            return &employee;
        }
    }
    return nullptr;
}

const Admin* BankRepository::findAdminById(int id) const {
    for (const Admin& admin : admins) {
        if (admin.getId() == id) {
            return &admin;
        }
    }
    return nullptr;
}

OperationResult BankRepository::authenticateClient(int id, const std::string& password, const Client*& client) const {
    client = findClientById(id);
    if (client == nullptr) {
        return {false, "Client not found."};
    }
    if (client->getPassword() != password) {
        return {false, "Incorrect client password."};
    }
    return {true, "Client authenticated."};
}

OperationResult BankRepository::authenticateEmployee(int id, const std::string& password, const Employee*& employee) const {
    employee = findEmployeeById(id);
    if (employee == nullptr) {
        return {false, "Employee not found."};
    }
    if (employee->getPassword() != password) {
        return {false, "Incorrect employee password."};
    }
    return {true, "Employee authenticated."};
}

OperationResult BankRepository::authenticateAdmin(int id, const std::string& password, const Admin*& admin) const {
    admin = findAdminById(id);
    if (admin == nullptr) {
        return {false, "Admin not found."};
    }
    if (admin->getPassword() != password) {
        return {false, "Incorrect admin password."};
    }
    return {true, "Admin authenticated."};
}

OperationResult BankRepository::createClient(const std::string& name, const std::string& password, double balance) {
    if (!Validation::isValidName(name)) {
        return {false, "Client name must be 3-20 alphabetic characters."};
    }
    if (!Validation::isValidPassword(password)) {
        return {false, "Client password must be 8-20 characters with no spaces."};
    }
    if (!Validation::isValidBalance(balance)) {
        return {false, "Client balance must be at least 1500."};
    }

    ++lastClientId;
    clients.emplace_back(lastClientId, name, password, balance);
    saveClients();
    saveCounters();
    return {true, "Client created successfully."};
}

OperationResult BankRepository::updateClient(int id, const std::string& name, const std::string& password, double balance) {
    Client* client = findClientById(id);
    if (client == nullptr) {
        return {false, "Client not found."};
    }
    if (!Validation::isValidName(name)) {
        return {false, "Client name must be 3-20 alphabetic characters."};
    }
    if (!Validation::isValidPassword(password)) {
        return {false, "Client password must be 8-20 characters with no spaces."};
    }
    if (!Validation::isValidBalance(balance)) {
        return {false, "Client balance must be at least 1500."};
    }

    client->setName(name);
    client->setPassword(password);
    client->setBalance(balance);
    saveClients();
    return {true, "Client updated successfully."};
}

OperationResult BankRepository::depositToClient(int clientId, double amount) {
    Client* client = findClientById(clientId);
    if (client == nullptr) {
        return {false, "Client not found."};
    }
    if (!client->deposit(amount)) {
        return {false, "Deposit amount must be greater than zero."};
    }

    saveClients();
    return {true, "Deposit completed successfully."};
}

OperationResult BankRepository::withdrawFromClient(int clientId, double amount) {
    Client* client = findClientById(clientId);
    if (client == nullptr) {
        return {false, "Client not found."};
    }
    if (!Validation::isPositiveAmount(amount)) {
        return {false, "Withdrawal amount must be greater than zero."};
    }
    if (!client->withdraw(amount)) {
        return {false, "Withdrawal failed because the minimum balance is 1500."};
    }

    saveClients();
    return {true, "Withdrawal completed successfully."};
}

OperationResult BankRepository::transferBetweenClients(int senderId, int recipientId, double amount) {
    Client* sender = findClientById(senderId);
    if (sender == nullptr) {
        return {false, "Sender client was not found."};
    }

    Client* recipient = findClientById(recipientId);
    if (recipient == nullptr) {
        return {false, "Recipient client was not found."};
    }

    if (senderId == recipientId) {
        return {false, "Sender and recipient must be different clients."};
    }

    if (!Validation::isPositiveAmount(amount)) {
        return {false, "Transfer amount must be greater than zero."};
    }

    if (!sender->transferTo(amount, *recipient)) {
        return {false, "Transfer failed because the sender must keep at least 1500."};
    }

    saveClients();
    return {true, "Transfer completed successfully."};
}

OperationResult BankRepository::createEmployee(const std::string& name, const std::string& password, double salary) {
    if (!Validation::isValidName(name)) {
        return {false, "Employee name must be 3-20 alphabetic characters."};
    }
    if (!Validation::isValidPassword(password)) {
        return {false, "Employee password must be 8-20 characters with no spaces."};
    }
    if (!Validation::isValidSalary(salary)) {
        return {false, "Employee salary must be at least 5000."};
    }

    ++lastEmployeeId;
    employees.emplace_back(lastEmployeeId, name, password, salary);
    saveEmployees();
    saveCounters();
    return {true, "Employee created successfully."};
}

OperationResult BankRepository::updateEmployee(int id, const std::string& name, const std::string& password, double salary) {
    Employee* employee = findEmployeeById(id);
    if (employee == nullptr) {
        return {false, "Employee not found."};
    }
    if (!Validation::isValidName(name)) {
        return {false, "Employee name must be 3-20 alphabetic characters."};
    }
    if (!Validation::isValidPassword(password)) {
        return {false, "Employee password must be 8-20 characters with no spaces."};
    }
    if (!Validation::isValidSalary(salary)) {
        return {false, "Employee salary must be at least 5000."};
    }

    employee->setName(name);
    employee->setPassword(password);
    employee->setSalary(salary);
    saveEmployees();
    return {true, "Employee updated successfully."};
}

int BankRepository::totalUsers() const {
    return static_cast<int>(clients.size() + employees.size() + admins.size());
}

double BankRepository::totalClientBalances() const {
    return std::accumulate(clients.begin(), clients.end(), 0.0, [](double total, const Client& client) {
        return total + client.getBalance();
    });
}