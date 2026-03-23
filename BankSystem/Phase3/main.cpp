#include "BankRepository.h"
#include "HttpServer.h"

#include <filesystem>
#include <iostream>

int main() {
    try {
        const std::filesystem::path root = std::filesystem::current_path();
        BankRepository repository(root);
        HttpServer server(repository, root / "web");

        std::cout << "Loaded Phase3 with " << repository.getClients().size() << " clients, "
                  << repository.getEmployees().size() << " employees, and "
                  << repository.getAdmins().size() << " admins.\n";
        server.start(8080);
    } catch (const std::exception& error) {
        std::cerr << "Phase3 failed to start: " << error.what() << '\n';
        return 1;
    }

    return 0;
}