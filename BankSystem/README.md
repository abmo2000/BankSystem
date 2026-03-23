# BankSystem - Multi-Phase Banking Application

A comprehensive banking system developed in C++ with progressive enhancements across three phases. This project demonstrates OOP principles, file persistence, and modern web UI integration.

## Project Overview

**BankSystem** is a multi-phase project that evolves from a console-based validation framework into a full-featured web application with persistent storage and role-based access control.

- **Phase 1**: Core OOP classes and business logic validation
- **Phase 2**: File-based persistence layer and data abstraction
- **Phase 3**: Web interface with integrated HTTP server and browser UI

## Architecture

### Phase 1: Foundation (Console-Based)
**Location**: `Phase1/`

Core banking domain model with validation rules:

- **Classes**: `Person` → `Client` / `Employee` → `Admin`
- **Features**:
  - Client account operations (deposit, withdraw, transfer)
  - Employee and admin management
  - Input validation (name, password, balance, salary)
  - Console-based testing harness

**Key Files**:
- `Person.h/cpp` - Base class for all users
- `Client.h/cpp` - Client banking operations
- `Employee.h/cpp` - Staff management
- `Admin.h/cpp` - Administrative privileges
- `Validation.h` - Shared validation utilities

**Build**:
```bash
cd Phase1
g++ -o BankSystem main.cpp Person.cpp Client.cpp Employee.cpp Admin.cpp
./BankSystem
```

### Phase 2: Persistence (File-Based Storage)
**Location**: `Phase2/`

Adds file I/O and data abstraction layer on top of Phase 1:

- **Architecture**:
  - `DataSourceInterface` - Abstract contract for storage backends
  - `FileManager` - CSV file-based implementation
  - `FilesHelper` - Low-level file I/O utilities
  - `Parser` - CSV serialization/deserialization
  - Auto-increment ID tracking across sessions

- **Storage Format**:
  - `Clients.txt` - Client records (id, name, password, balance)
  - `Employee.txt` - Employee records
  - `Admin.txt` - Admin records
  - `last_*_id.txt` - ID counter files

**Key Features**:
- Multi-session persistence
- Transaction-safe ID generation
- Easy backend swapping (implement `DataSourceInterface`)

**Build**:
```bash
cd Phase2
g++ -std=c++17 -o BankSystem main.cpp Person.cpp Client.cpp Employee.cpp Admin.cpp
./BankSystem
```

### Phase 3: Web Application (Full-Featured)
**Location**: `Phase3/`

Complete web-based banking application with built-in HTTP server and modern UI.

#### Architecture

**Backend** (C++):
- `BankRepository` - Session management and business logic orchestration
- `HttpServer` - Raw socket-based HTTP server with routing
- Role-based access control (client, employee, admin)
- In-memory session tokens for authentication

**Frontend** (JavaScript + CSS):
- Responsive web interface with modern design
- Real-time dashboard with system metrics
- Role-specific workflows and data visibility
- Persistent session management

#### Features by Role

**Client**:
- Login with ID and password
- View account balance
- Deposit funds
- Withdraw funds (minimum balance: 1500)
- Transfer to other clients
- Real-time balance updates

**Employee**:
- Login with ID and password
- View all clients
- Create new client accounts
- Update client information
- Manage client data
- Live client list with search

**Admin**:
- All employee capabilities plus:
- Create and manage employees
- View all admins
- System-wide dashboard
- Employee and admin records
- Full data oversight

#### Data Flow

```
Phase2 Data          Phase3 Bootstrap      Phase3 Operations
─────────────        ────────────────      ─────────────────
Clients.txt    ──►  Phase3/data/      ──►  Web UI
Employee.txt        Clients.txt            ↓
Admin.txt           Employee.txt      ←──  BankRepository
                    Admin.txt              ↓
                                      HTTP Server
                                           ↓
                                      Browser
```

On first run:
1. Phase3 creates `Phase3/data/` directory
2. Copies data from `Phase2/` if it exists
3. Bootstraps default admin if no admin data found
4. All subsequent operations use `Phase3/data/` files

**Build**:
```bash
cd Phase3
g++ -std=c++17 -Wall -Wextra -pedantic -o BankSystem \
  main.cpp Person.cpp Client.cpp Employee.cpp Admin.cpp \
  BankRepository.cpp HttpServer.cpp
```

**Run**:
```bash
./BankSystem
```

Open: `http://localhost:8080`

## Getting Started

### Prerequisites
- GCC 7.0+ with C++17 support
- Linux/macOS (uses POSIX sockets)
- A modern web browser (for Phase 3)

### Installation

1. Clone the repository:
```bash
git clone https://github.com/yourusername/BankSystem.git
cd BankSystem/BankSystem
```

2. Navigate to the phase you want to run:
```bash
cd Phase3  # Recommended for full experience
```

3. Build the project:
```bash
g++ -std=c++17 -Wall -Wextra -pedantic -o BankSystem \
  main.cpp Person.cpp Client.cpp Employee.cpp Admin.cpp \
  BankRepository.cpp HttpServer.cpp
```

4. Run it:
```bash
./BankSystem
```

5. For Phase 3, open your browser:
```
http://localhost:8080
```

## Default Test Accounts

Imported from Phase 2 on first Phase 3 run:

| Role | ID | Password | Purpose |
|------|----|-----------| --------|
| Client | 1 | pass1234 | Test client operations |
| Employee | 1 | emp12345 | Test employee features |
| Admin | 1 | admin999 | Test admin capabilities |

## Validation Rules

All input is validated according to these rules:

| Field | Rule |
|-------|------|
| Name | Alphabetic characters only, 3-20 characters |
| Password | 8-20 characters, no spaces |
| Client Balance | Minimum 1500 EGP |
| Employee/Admin Salary | Minimum 5000 EGP |
| Transfer Amount | Sender must maintain minimum balance |

## Phase Comparison

| Feature | Phase 1 | Phase 2 | Phase 3 |
|---------|---------|---------|---------|
| Core Domain Model | ✓ | ✓ | ✓ |
| Input Validation | ✓ | ✓ | ✓ |
| File Persistence | ✗ | ✓ | ✓ |
| Abstract Interfaces | ✗ | ✓ | ✓ |
| Web UI | ✗ | ✗ | ✓ |
| HTTP API | ✗ | ✗ | ✓ |
| Session Management | ✗ | ✗ | ✓ |
| Role-Based Access | ✗ | ✗ | ✓ |
| Browser Support | ✗ | ✗ | ✓ |
| Multi-User | ✗ | ✗ | ✓ |

## File Structure

```
BankSystem/
├── Phase1/                  # Foundation & validation
│   ├── Person.h/cpp
│   ├── Client.h/cpp
│   ├── Employee.h/cpp
│   ├── Admin.h/cpp
│   ├── Validation.h
│   ├── main.cpp
│   └── README
│
├── Phase2/                  # Persistence & data abstraction
│   ├── Person.h/cpp
│   ├── Client.h/cpp
│   ├── Employee.h/cpp
│   ├── Admin.h/cpp
│   ├── Validation.h
│   ├── Parser.h
│   ├── FilesHelper.h
│   ├── FileManager.h
│   ├── DataSourceInterface.h
│   ├── main.cpp
│   ├── Clients.txt          # Sample data
│   ├── Employee.txt
│   ├── Admin.txt
│   ├── last_*_id.txt        # ID counters
│   └── README.md
│
├── Phase3/                  # Web application
│   ├── Person.h/cpp
│   ├── Client.h/cpp
│   ├── Employee.h/cpp
│   ├── Admin.h/cpp
│   ├── Validation.h
│   ├── Parser.h
│   ├── BankRepository.h/cpp # Session & orchestration
│   ├── HttpServer.h/cpp     # HTTP routing & responses
│   ├── main.cpp
│   ├── data/                # Persistent storage (created at runtime)
│   │   ├── Clients.txt
│   │   ├── Employee.txt
│   │   ├── Admin.txt
│   │   └── last_*_id.txt
│   ├── web/                 # Browser UI
│   │   ├── index.html
│   │   ├── styles.css
│   │   ├── app.js
│   │    └── BankSystem      # Built executable
│   └── README.md
│
└── README.md               # This file
```

## API Reference (Phase 3)

### Public Endpoints

**`GET /api/dashboard`**
- Returns system-wide statistics (clients, employees, admins, total users, client funds)
- No authentication required

### Authentication

**`POST /api/login`**
```json
{
  "role": "client|employee|admin",
  "id": 123,
  "password": "secret123"
}
```
Returns: `{ "success": true, "token": "...", "role": "..." }`

**`POST /api/logout`**
- Clears session token
- Header: `X-Session-Token: <token>`

### Session

**`GET /api/session`**
- Returns authenticated user data and role-specific resources
- Header: `X-Session-Token: <token>`

### Client Operations

**`POST /api/client/deposit`**
```json
{ "clientId": 1, "amount": 1000.00 }
```

**`POST /api/client/withdraw`**
```json
{ "clientId": 1, "amount": 500.00 }
```

**`POST /api/client/transfer`**
```json
{ "senderId": 1, "recipientId": 2, "amount": 800.00 }
```

### Employee Operations

**`POST /api/employee/client/create`**
```json
{ "name": "John Doe", "password": "pass1234", "balance": 2000.00 }
```

**`POST /api/employee/client/update`**
```json
{ "id": 1, "name": "Jane Doe", "password": "pass1234", "balance": 3000.00 }
```

### Admin Operations

**`POST /api/admin/employee/create`**
```json
{ "name": "Manager", "password": "pass1234", "salary": 8000.00 }
```

**`POST /api/admin/employee/update`**
```json
{ "id": 1, "name": "Manager Updated", "password": "pass1234", "salary": 9000.00 }
```

## Development

### Adding Features

1. **New business rule**: Add validation to `Validation.h`
2. **New endpoint**: Add routing in `HttpServer::route()`
3. **New API response**: Modify `BankRepository` methods
4. **New UI component**: Update templates in `web/index.html`
5. **New styling**: Extend `web/styles.css`
6. **New client logic**: Extend `web/app.js`

### Database Migration

To replace file-based storage:

1. Create new class implementing `DataSourceInterface` (if it existed in Phase 2)
2. Update `BankRepository` to use your backend
3. Keep `Parser` utilities for serialization
4. No changes needed to domain classes or HTTP server

## Testing Guide

### Phase 1 Testing
1. Run the executable
2. Review console output for validation test results
3. Check that all validation rules are enforced

### Phase 2 Testing
1. Run the executable
2. Create client, employee, and admin records
3. Verify files appear in working directory
4. Restart and verify data is persistent

### Phase 3 Testing (Recommended)

1. **Start server**:
   ```bash
   cd Phase3
   ./BankSystem
   ```

2. **Open UI**: `http://localhost:8080`

3. **Test client flow**:
   - Login as client (ID: 1 / pass1234)
   - Deposit 1000
   - Withdraw 500
   - Transfer 300 to client 2

4. **Test employee flow**:
   - Logout
   - Login as employee (ID: 1 / emp12345)
   - Create new client
   - Update client data

5. **Test admin flow**:
   - Logout
   - Login as admin (ID: 1 / admin999)
   - Create new employee
   - Update employee
   - View dashboard stats

## Troubleshooting

### Build Issues

**Error**: `error: #include <filesystem> not found`
- Solution: Use `-std=c++17` flag: `g++ -std=c++17 ...`

**Error**: `undefined reference to socket`
- Solution: This is normal on non-POSIX systems. Phase 3 requires Linux/macOS.

### Runtime Issues

**Error**: `Unable to bind to the requested port`
- Solution: Port 8080 is in use. Kill the process or specify a different port.
```bash
lsof -i :8080
kill -9 PID
```

**Error**: `Cannot open Clients.txt`
- Solution: Make sure you run from the Phase 3 directory so relative paths work.
```bash
cd Phase3
./BankSystem
```

## Future Enhancements

- [ ] Database backend (SQLite/PostgreSQL) replacing file storage
- [ ] User authentication with hashing
- [ ] Transaction history and audit logs
- [ ] Email notifications
- [ ] Mobile app (React Native/Flutter)
- [ ] REST API documentation (Swagger)
- [ ] Unit tests and integration tests
- [ ] Docker containerization
- [ ] Multi-server deployment

## Learning Outcomes

This project demonstrates:

1. **Object-Oriented Programming**: Inheritance, polymorphism, encapsulation
2. **Design Patterns**: Abstract interfaces, factory pattern, session management
3. **File I/O**: CSV parsing, persistence, auto-increment patterns
4. **Web Development**: HTTP protocol, routing, JSON serialization
5. **Frontend Development**: HTML/CSS/JavaScript, async API calls, responsive design
6. **System Programming**: Socket programming, multi-client handling
7. **Software Architecture**: Layered architecture, separation of concerns, abstraction

## License

This project is available for educational purposes.

## Author

BankSystem Team - Educational Project

## Support

For issues or questions, please refer to the individual phase README files or the inline code documentation.

---

**Last Updated**: March 2026
**C++ Standard**: C++17
**Platform**: Linux/macOS with POSIX sockets
