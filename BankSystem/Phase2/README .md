# 🏦 Bank System - Phase 2

A comprehensive banking system developed in C++ with Object-Oriented Programming principles.

## 📋 Project Overview

This is **Phase 2** of a multi-phase banking system project. It builds on Phase 1 by adding **file persistence**, a full data-source abstraction layer, and client/employee management methods on the Employee and Admin classes.

## ✨ Features

### File Persistence
- ✅ Save client data to `Clients.txt`
- ✅ Save employee data to `Employee.txt`
- ✅ Save admin data to `Admin.txt`
- ✅ Read data back from files on demand
- ✅ Clear all records from files
- ✅ Auto-increment IDs tracked across sessions

### Data Source Abstraction
- ✅ `DataSourceInterface` abstract class defines the contract
- ✅ `FileManager` implements the interface using file storage
- ✅ Easy to swap storage backend in the future (e.g. Database)

### Parser
- ✅ Split CSV lines into tokens
- ✅ Parse lines into Client / Employee / Admin objects
- ✅ Serialize objects back to CSV strings

### Employee Management (added to Employee class)
- ✅ Add a client
- ✅ Search client by ID
- ✅ List all clients
- ✅ Edit client info

### Admin Management (added to Admin class)
- ✅ All Employee features
- ✅ Add an employee
- ✅ Search employee by ID
- ✅ List all employees
- ✅ Edit employee info

## 🏗️ Project Structure

```
BankSystem_Phase2/
├── Validation.h              # Validation utility class
├── Person.h / Person.cpp     # Base Person class
├── Client.h / Client.cpp     # Client class
├── Employee.h / Employee.cpp # Employee class
├── Admin.h / Admin.cpp       # Admin class
├── DataSourceInterface.h     # Abstract data source contract
├── Parser.h                  # CSV parser and serializer
├── FilesHelper.h             # Low-level file I/O operations
├── FileManager.h             # Implements DataSourceInterface
├── main.cpp                  # Testing and demonstration
├── Clients.txt               # Client data file
├── Employee.txt              # Employee data file
├── Admin.txt                 # Admin data file
└── README.md                 # This file
```

## 🏗️ The complete structure of the classes with relationships

```
┌─────────────────────┐
│   Validation        │ (Utility Class)
│   ─────────────     │
│   + static methods  │
└─────────────────────┘
         ↑  all classes use it
         │
┌─────────────────────┐
│      Person         │ (Base Class)
│   ─────────────     │
│   - id              │
│   - name            │
│   - password        │
│   ─────────────     │
│   + setters         │
│   + getters         │
│   + display()       │
└─────────────────────┘
         ↑
         │ Inherits from
    ┌────┴────┐
    │         │
┌───┴───┐ ┌──┴────────┐
│Client │ │ Employee  │
│───────│ │───────────│
│balance│ │  salary   │
│───────│ │───────────│
│deposit│ │addClient()│
│withdrw│ │searchClnt │
│transf │ │listClient │
│check  │ │editClient │
└───────┘ └─────┬─────┘
                │
                │ Inherits from
          ┌─────┴─────┐
          │   Admin   │
          │───────────│
          │  salary   │
          │───────────│
          │addEmployee│
          │searchEmp  │
          │listEmp    │
          │editEmp    │
          └───────────┘

┌─────────────────────────────┐
│  DataSourceInterface        │ (Abstract)
│  ───────────────────────    │
│  + addClient() = 0          │
│  + addEmployee() = 0        │
│  + addAdmin() = 0           │
│  + getAllClients() = 0      │
│  + getAllEmployees() = 0    │
│  + getAllAdmins() = 0       │
│  + removeAllClients() = 0   │
│  + removeAllEmployees() = 0 │
│  + removeAllAdmins() = 0    │
└─────────────────────────────┘
                ↑
                │ Implements
         ┌──────┴──────┐
         │ FileManager │
         │─────────────│
         │ implements  │
         │ all methods │
         └─────────────┘

┌──────────────┐      ┌──────────────────┐
│   Parser     │      │   FilesHelper    │
│──────────────│      │──────────────────│
│+ split()     │      │+ saveLast()      │
│+ parseToClnt │      │+ getLast()       │
│+ parseToEmp  │      │+ saveClient()    │
│+ parseToAdmn │      │+ saveEmployee()  │
└──────────────┘      │+ getClients()    │
                      │+ getEmployees()  │
                      │+ getAdmins()     │
                      │+ clearFile()     │
                      └──────────────────┘
```

## 🔧 Class Hierarchy

```
Person (Base Class)
├── Client
└── Employee
    └── Admin
```

## 💻 Technologies Used

- **Language:** C++
- **Paradigm:** Object-Oriented Programming (OOP)
- **Compiler:** g++ (GCC)
- **Standard:** C++17

## 🚀 Getting Started

### Prerequisites

- g++ compiler installed
- Basic understanding of C++

### Installation

1. **Clone the repository:**
   ```bash
   git clone 
   cd BankSystem/Phase2
   ```

2. **Compile the project:**
   ```bash
   g++ -std=c++17 -o BankSystem main.cpp Person.cpp Client.cpp Employee.cpp Admin.cpp
   ```

3. **Run the program:**
   ```bash
   ./BankSystem
   ```

## 📖 Usage Examples

### Add and Save a Client
```cpp
FileManager fm;
Client c(1, "Ahmed Hassan", "pass1234", 5000);
fm.addClient(c);
```

### Read All Clients from File
```cpp
fm.getAllClients();
```

### Employee Searches for a Client
```cpp
Employee emp(1, "John Doe", "emp12345", 8000);
emp.setClients(FilesHelper::getClients());
Client* found = emp.searchClient(1);
if (found) found->display();
```

### Employee Edits a Client
```cpp
emp.editClient(1, "Ahmed Updated", "newpass1", 6000);
FilesHelper::rewriteClients(emp.getClients());
```

## 🧪 Testing

The `main.cpp` file includes comprehensive tests for:
- ✅ Adding and saving clients, employees, admins to files
- ✅ Reading all records back from files
- ✅ Searching for a client by ID
- ✅ Editing client info and persisting changes
- ✅ Listing all records

Run the program to see all tests in action.

## 📊 Validation Rules

| Field | Rule |
|-------|------|
| Name | Alphabetic characters only, 3-20 characters |
| Password | 8-20 characters, no spaces |
| Client Balance | Minimum 1500 EGP |
| Employee/Admin Salary | Minimum 5000 EGP |

## 📂 File Format

Data is stored in CSV format:
```
id,name,password,balance_or_salary
```
Example — `Clients.txt`:
```
1,Alice Smith,pass1234,5000.000000
2,Bob Johnson,secureABC,2000.000000
```

## 🛣️ Roadmap

- [x] **Phase 1:** Core classes and validation ✅
- [x] **Phase 2:** File handling and data persistence ✅
- [ ] **Phase 3:** User interface and interaction
- [ ] **Final Project:** Complete banking system

## 🤝 Contributing

This is an educational project. Contributions, issues, and feature requests are welcome!

1. Fork the project
2. Create your feature branch (`git checkout -b feature/AmazingFeature`)
3. Commit your changes (`git commit -m 'Add some AmazingFeature'`)
4. Push to the branch (`git push origin feature/AmazingFeature`)
5. Open a Pull Request

## 📝 License

This project is created for educational purposes.

## 👨‍💻 Author

**Your Name**
- GitHub: [@abmo20000](https://github.com/abmo20000)

## 🙏 Acknowledgments

- CS Final Project - Bank System
- Object-Oriented Programming Principles
- C++ Best Practices

---

⭐ **If you found this project helpful, please give it a star!** ⭐
