#pragma once

#include "Admin.h"
#include "Client.h"
#include "Employee.h"

#include <sstream>
#include <string>
#include <vector>

class Parser {
public:
    static std::vector<std::string> split(const std::string& line) {
        std::vector<std::string> tokens;
        std::stringstream stream(line);
        std::string token;

        while (std::getline(stream, token, ',')) {
            tokens.push_back(token);
        }

        return tokens;
    }

    static Client parseClient(const std::string& line) {
        const std::vector<std::string> fields = split(line);
        if (fields.size() < 4) {
            return Client();
        }

        return Client(std::stoi(fields[0]), fields[1], fields[2], std::stod(fields[3]));
    }

    static Employee parseEmployee(const std::string& line) {
        const std::vector<std::string> fields = split(line);
        if (fields.size() < 4) {
            return Employee();
        }

        return Employee(std::stoi(fields[0]), fields[1], fields[2], std::stod(fields[3]));
    }

    static Admin parseAdmin(const std::string& line) {
        const std::vector<std::string> fields = split(line);
        if (fields.size() < 4) {
            return Admin();
        }

        return Admin(std::stoi(fields[0]), fields[1], fields[2], std::stod(fields[3]));
    }

    static std::string toClientString(const Client& client) {
        return std::to_string(client.getId()) + "," + client.getName() + "," +
               client.getPassword() + "," + std::to_string(client.getBalance());
    }

    static std::string toEmployeeString(const Employee& employee) {
        return std::to_string(employee.getId()) + "," + employee.getName() + "," +
               employee.getPassword() + "," + std::to_string(employee.getSalary());
    }

    static std::string toAdminString(const Admin& admin) {
        return std::to_string(admin.getId()) + "," + admin.getName() + "," +
               admin.getPassword() + "," + std::to_string(admin.getSalary());
    }
};