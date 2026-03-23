# Bank System - Phase 3

Phase 3 turns the earlier C++ exercises into a usable application.

## What changed

- Reused the core inheritance model from Phase 1: `Person -> Client -> Employee -> Admin`
- Reused the persistence approach from Phase 2: text files with auto-increment IDs
- Added a standalone C++ HTTP server that serves a browser UI
- Added role-based workflows for clients, employees, and admins
- Bootstraps initial data from `../Phase2` on first run so your old sample records still work

## Features

- Client login
- Employee login
- Admin login
- Deposit, withdraw, and transfer for clients
- Create and update clients for employees and admins
- Create and update employees for admins
- Dashboard metrics for the whole system
- Data persistence in `Phase3/data`

## Default imported credentials

If Phase 2 data exists, Phase 3 imports it on first run.

- Client: `1 / pass1234`
- Employee: `1 / emp12345`
- Admin: `1 / admin999`

If no admin data exists, Phase 3 creates this fallback admin:

- Admin: `1 / admin123`

## Build

From the `Phase3` folder:

```bash
g++ -std=c++17 -Wall -Wextra -pedantic -o BankSystem main.cpp Person.cpp Client.cpp Employee.cpp Admin.cpp BankRepository.cpp HttpServer.cpp
```

## Run

```bash
./BankSystem
```

Then open:

```text
http://localhost:8080
```

## Data layout

Phase 3 keeps its own files under `Phase3/data`:

- `Clients.txt`
- `Employee.txt`
- `Admin.txt`
- `last_client_id.txt`
- `last_employee_id.txt`
- `last_admin_id.txt`

## Notes

- This server is intentionally lightweight and uses raw sockets to avoid external dependencies.
- Authentication is session-token based in memory and intended for local use.
- CSV persistence is kept for continuity with Phase 2.