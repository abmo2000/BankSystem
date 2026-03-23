#include "HttpServer.h"

#include <arpa/inet.h>
#include <cctype>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <netinet/in.h>
#include <random>
#include <sstream>
#include <stdexcept>
#include <sys/socket.h>
#include <unistd.h>

namespace {
std::string trim(const std::string& value) {
    std::size_t start = 0;
    while (start < value.size() && std::isspace(static_cast<unsigned char>(value[start]))) {
        ++start;
    }

    std::size_t end = value.size();
    while (end > start && std::isspace(static_cast<unsigned char>(value[end - 1]))) {
        --end;
    }

    return value.substr(start, end - start);
}

std::string jsonEscape(const std::string& value) {
    std::ostringstream output;
    for (char ch : value) {
        switch (ch) {
            case '\\':
                output << "\\\\";
                break;
            case '"':
                output << "\\\"";
                break;
            case '\n':
                output << "\\n";
                break;
            case '\r':
                output << "\\r";
                break;
            case '\t':
                output << "\\t";
                break;
            default:
                output << ch;
                break;
        }
    }
    return output.str();
}

std::string getMimeType(const std::string& extension) {
    if (extension == ".css") {
        return "text/css";
    }
    if (extension == ".js") {
        return "application/javascript";
    }
    if (extension == ".html") {
        return "text/html";
    }
    return "text/plain";
}

int toInt(const std::unordered_map<std::string, std::string>& payload, const std::string& key) {
    return std::stoi(payload.at(key));
}

double toDouble(const std::unordered_map<std::string, std::string>& payload, const std::string& key) {
    return std::stod(payload.at(key));
}

std::string clientToJson(const Client& client) {
    std::ostringstream output;
    output << "{"
           << "\"id\":" << client.getId() << ","
           << "\"name\":\"" << jsonEscape(client.getName()) << "\","
           << "\"balance\":" << std::fixed << std::setprecision(2) << client.getBalance()
           << "}";
    return output.str();
}

std::string employeeToJson(const Employee& employee) {
    std::ostringstream output;
    output << "{"
           << "\"id\":" << employee.getId() << ","
           << "\"name\":\"" << jsonEscape(employee.getName()) << "\","
           << "\"salary\":" << std::fixed << std::setprecision(2) << employee.getSalary()
           << "}";
    return output.str();
}

std::string adminToJson(const Admin& admin) {
    std::ostringstream output;
    output << "{"
           << "\"id\":" << admin.getId() << ","
           << "\"name\":\"" << jsonEscape(admin.getName()) << "\","
           << "\"salary\":" << std::fixed << std::setprecision(2) << admin.getSalary()
           << "}";
    return output.str();
}

std::string buildErrorJson(const std::string& message) {
    return std::string("{\"success\":false,\"message\":\"") + jsonEscape(message) + "\"}";
}

std::string buildSuccessJson(const std::string& message) {
    return std::string("{\"success\":true,\"message\":\"") + jsonEscape(message) + "\"}";
}
}

HttpServer::HttpServer(BankRepository& repository, const std::filesystem::path& webRoot)
    : repository(repository), webRoot(webRoot) {}

HttpServer::HttpRequest HttpServer::parseRequest(int clientSocket) const {
    std::string rawRequest;
    char buffer[4096];
    ssize_t received = 0;

    while ((received = recv(clientSocket, buffer, sizeof(buffer), 0)) > 0) {
        rawRequest.append(buffer, static_cast<std::size_t>(received));
        if (rawRequest.find("\r\n\r\n") != std::string::npos) {
            break;
        }
    }

    if (rawRequest.empty()) {
        return {};
    }

    HttpRequest request;
    const std::size_t headerEnd = rawRequest.find("\r\n\r\n");
    std::string headerSection = rawRequest.substr(0, headerEnd);
    std::istringstream stream(headerSection);
    std::string requestLine;

    std::getline(stream, requestLine);
    if (!requestLine.empty() && requestLine.back() == '\r') {
        requestLine.pop_back();
    }

    std::istringstream lineStream(requestLine);
    lineStream >> request.method >> request.path;

    std::string headerLine;
    while (std::getline(stream, headerLine)) {
        if (!headerLine.empty() && headerLine.back() == '\r') {
            headerLine.pop_back();
        }
        const std::size_t delimiter = headerLine.find(':');
        if (delimiter == std::string::npos) {
            continue;
        }
        request.headers[trim(headerLine.substr(0, delimiter))] = trim(headerLine.substr(delimiter + 1));
    }

    std::size_t contentLength = 0;
    const auto lengthIt = request.headers.find("Content-Length");
    if (lengthIt != request.headers.end()) {
        contentLength = static_cast<std::size_t>(std::stoul(lengthIt->second));
    }

    const std::size_t bodyStart = headerEnd == std::string::npos ? rawRequest.size() : headerEnd + 4;
    if (bodyStart < rawRequest.size()) {
        request.body = rawRequest.substr(bodyStart);
    }

    while (request.body.size() < contentLength && (received = recv(clientSocket, buffer, sizeof(buffer), 0)) > 0) {
        request.body.append(buffer, static_cast<std::size_t>(received));
    }

    if (request.body.size() > contentLength) {
        request.body.resize(contentLength);
    }

    const std::size_t queryStart = request.path.find('?');
    if (queryStart != std::string::npos) {
        request.path = request.path.substr(0, queryStart);
    }

    return request;
}

HttpServer::HttpResponse HttpServer::route(const HttpRequest& request) {
    if (request.method.empty()) {
        return {400, "application/json", buildErrorJson("Malformed request.")};
    }

    if (request.method == "GET" && request.path == "/api/dashboard") {
        return handleDashboard();
    }
    if (request.method == "GET" && request.path == "/api/session") {
        return handleSession(request);
    }
    if (request.method == "POST" && request.path == "/api/login") {
        return handleLogin(request);
    }
    if (request.method == "POST" && request.path == "/api/logout") {
        return handleLogout(request);
    }
    if (request.method == "POST" && request.path == "/api/client/deposit") {
        return handleClientDeposit(request);
    }
    if (request.method == "POST" && request.path == "/api/client/withdraw") {
        return handleClientWithdraw(request);
    }
    if (request.method == "POST" && request.path == "/api/client/transfer") {
        return handleClientTransfer(request);
    }
    if (request.method == "POST" && request.path == "/api/employee/client/create") {
        return handleClientCreate(request);
    }
    if (request.method == "POST" && request.path == "/api/employee/client/update") {
        return handleClientUpdate(request);
    }
    if (request.method == "POST" && request.path == "/api/admin/employee/create") {
        return handleEmployeeCreate(request);
    }
    if (request.method == "POST" && request.path == "/api/admin/employee/update") {
        return handleEmployeeUpdate(request);
    }

    if (request.method == "GET") {
        return serveStaticFile(request.path);
    }

    return {404, "application/json", buildErrorJson("Route not found.")};
}

HttpServer::HttpResponse HttpServer::serveStaticFile(const std::string& requestPath) const {
    std::string normalizedPath = requestPath == "/" ? "/index.html" : requestPath;
    if (normalizedPath.find("..") != std::string::npos) {
        return {403, "text/plain", "Forbidden"};
    }

    std::filesystem::path filePath = webRoot / normalizedPath.substr(1);
    std::ifstream input(filePath, std::ios::binary);
    if (!input.is_open()) {
        return {404, "text/plain", "Not found"};
    }

    std::ostringstream buffer;
    buffer << input.rdbuf();
    return {200, getMimeType(filePath.extension().string()), buffer.str()};
}

HttpServer::HttpResponse HttpServer::handleDashboard() const {
    std::ostringstream output;
    output << "{"
           << "\"success\":true,"
           << "\"stats\":{"
           << "\"clients\":" << repository.getClients().size() << ","
           << "\"employees\":" << repository.getEmployees().size() << ","
           << "\"admins\":" << repository.getAdmins().size() << ","
           << "\"totalUsers\":" << repository.totalUsers() << ","
           << "\"clientFunds\":" << std::fixed << std::setprecision(2) << repository.totalClientBalances()
           << "}"
           << "}";
    return {200, "application/json", output.str()};
}

HttpServer::HttpResponse HttpServer::handleSession(const HttpRequest& request) const {
    const Session* session = requireSession(request);
    if (session == nullptr) {
        return {401, "application/json", buildErrorJson("You need to log in first.")};
    }

    std::ostringstream output;
    output << "{\"success\":true,\"role\":\"" << jsonEscape(session->role) << "\",";

    if (session->role == "client") {
        const Client* client = repository.findClientById(session->userId);
        if (client == nullptr) {
            return {404, "application/json", buildErrorJson("Client session is no longer valid.")};
        }
        output << "\"user\":" << clientToJson(*client);
    } else if (session->role == "employee") {
        const Employee* employee = repository.findEmployeeById(session->userId);
        if (employee == nullptr) {
            return {404, "application/json", buildErrorJson("Employee session is no longer valid.")};
        }
        output << "\"user\":" << employeeToJson(*employee) << ",\"clients\":[";
        const auto& clients = repository.getClients();
        for (std::size_t index = 0; index < clients.size(); ++index) {
            if (index > 0) {
                output << ',';
            }
            output << clientToJson(clients[index]);
        }
        output << ']';
    } else {
        const Admin* admin = repository.findAdminById(session->userId);
        if (admin == nullptr) {
            return {404, "application/json", buildErrorJson("Admin session is no longer valid.")};
        }

        output << "\"user\":" << adminToJson(*admin) << ",\"clients\":[";
        const auto& clients = repository.getClients();
        for (std::size_t index = 0; index < clients.size(); ++index) {
            if (index > 0) {
                output << ',';
            }
            output << clientToJson(clients[index]);
        }
        output << "],\"employees\":[";

        const auto& employees = repository.getEmployees();
        for (std::size_t index = 0; index < employees.size(); ++index) {
            if (index > 0) {
                output << ',';
            }
            output << employeeToJson(employees[index]);
        }

        output << "],\"admins\":[";
        const auto& admins = repository.getAdmins();
        for (std::size_t index = 0; index < admins.size(); ++index) {
            if (index > 0) {
                output << ',';
            }
            output << adminToJson(admins[index]);
        }
        output << ']';
    }

    output << '}';
    return {200, "application/json", output.str()};
}

HttpServer::HttpResponse HttpServer::handleLogin(const HttpRequest& request) {
    try {
        const std::unordered_map<std::string, std::string> payload = parseJsonObject(request.body);
        const std::string role = payload.at("role");
        const int id = toInt(payload, "id");
        const std::string password = payload.at("password");

        OperationResult result{false, "Unsupported role."};
        if (role == "client") {
            const Client* client = nullptr;
            result = repository.authenticateClient(id, password, client);
        } else if (role == "employee") {
            const Employee* employee = nullptr;
            result = repository.authenticateEmployee(id, password, employee);
        } else if (role == "admin") {
            const Admin* admin = nullptr;
            result = repository.authenticateAdmin(id, password, admin);
        }

        if (!result.success) {
            return {401, "application/json", buildErrorJson(result.message)};
        }

        const std::string token = generateToken();
        sessions[token] = Session{token, role, id};

        std::ostringstream output;
        output << "{\"success\":true,\"message\":\"" << jsonEscape(result.message)
               << "\",\"token\":\"" << token << "\",\"role\":\"" << role << "\"}";
        return {200, "application/json", output.str()};
    } catch (const std::exception&) {
        return {400, "application/json", buildErrorJson("Login request is missing required fields.")};
    }
}

HttpServer::HttpResponse HttpServer::handleLogout(const HttpRequest& request) {
    Session* session = requireSession(request);
    if (session == nullptr) {
        return {401, "application/json", buildErrorJson("You need to log in first.")};
    }

    sessions.erase(session->token);
    return {200, "application/json", buildSuccessJson("Session closed." )};
}

HttpServer::HttpResponse HttpServer::handleClientDeposit(const HttpRequest& request) {
    try {
        Session* session = requireSession(request);
        if (session == nullptr || session->role != "client") {
            return {403, "application/json", buildErrorJson("Only clients can deposit into their own account.")};
        }

        const std::unordered_map<std::string, std::string> payload = parseJsonObject(request.body);
        const int clientId = toInt(payload, "clientId");
        if (clientId != session->userId) {
            return {403, "application/json", buildErrorJson("You can only modify your own account.")};
        }

        const OperationResult result = repository.depositToClient(clientId, toDouble(payload, "amount"));
        return {result.success ? 200 : 400, "application/json",
                result.success ? buildSuccessJson(result.message) : buildErrorJson(result.message)};
    } catch (const std::exception&) {
        return {400, "application/json", buildErrorJson("Deposit request is invalid.")};
    }
}

HttpServer::HttpResponse HttpServer::handleClientWithdraw(const HttpRequest& request) {
    try {
        Session* session = requireSession(request);
        if (session == nullptr || session->role != "client") {
            return {403, "application/json", buildErrorJson("Only clients can withdraw from their own account.")};
        }

        const std::unordered_map<std::string, std::string> payload = parseJsonObject(request.body);
        const int clientId = toInt(payload, "clientId");
        if (clientId != session->userId) {
            return {403, "application/json", buildErrorJson("You can only modify your own account.")};
        }

        const OperationResult result = repository.withdrawFromClient(clientId, toDouble(payload, "amount"));
        return {result.success ? 200 : 400, "application/json",
                result.success ? buildSuccessJson(result.message) : buildErrorJson(result.message)};
    } catch (const std::exception&) {
        return {400, "application/json", buildErrorJson("Withdrawal request is invalid.")};
    }
}

HttpServer::HttpResponse HttpServer::handleClientTransfer(const HttpRequest& request) {
    try {
        Session* session = requireSession(request);
        if (session == nullptr || session->role != "client") {
            return {403, "application/json", buildErrorJson("Only clients can transfer from their own account.")};
        }

        const std::unordered_map<std::string, std::string> payload = parseJsonObject(request.body);
        const int senderId = toInt(payload, "senderId");
        if (senderId != session->userId) {
            return {403, "application/json", buildErrorJson("You can only transfer from your own account.")};
        }

        const OperationResult result = repository.transferBetweenClients(
            senderId,
            toInt(payload, "recipientId"),
            toDouble(payload, "amount"));
        return {result.success ? 200 : 400, "application/json",
                result.success ? buildSuccessJson(result.message) : buildErrorJson(result.message)};
    } catch (const std::exception&) {
        return {400, "application/json", buildErrorJson("Transfer request is invalid.")};
    }
}

HttpServer::HttpResponse HttpServer::handleClientCreate(const HttpRequest& request) {
    try {
        Session* session = requireSession(request);
        if (session == nullptr || (session->role != "employee" && session->role != "admin")) {
            return {403, "application/json", buildErrorJson("Employees or admins can create clients.")};
        }

        const std::unordered_map<std::string, std::string> payload = parseJsonObject(request.body);
        const OperationResult result = repository.createClient(payload.at("name"), payload.at("password"), toDouble(payload, "balance"));
        return {result.success ? 200 : 400, "application/json",
                result.success ? buildSuccessJson(result.message) : buildErrorJson(result.message)};
    } catch (const std::exception&) {
        return {400, "application/json", buildErrorJson("Create client request is invalid.")};
    }
}

HttpServer::HttpResponse HttpServer::handleClientUpdate(const HttpRequest& request) {
    try {
        Session* session = requireSession(request);
        if (session == nullptr || (session->role != "employee" && session->role != "admin")) {
            return {403, "application/json", buildErrorJson("Employees or admins can update clients.")};
        }

        const std::unordered_map<std::string, std::string> payload = parseJsonObject(request.body);
        const OperationResult result = repository.updateClient(
            toInt(payload, "id"),
            payload.at("name"),
            payload.at("password"),
            toDouble(payload, "balance"));
        return {result.success ? 200 : 400, "application/json",
                result.success ? buildSuccessJson(result.message) : buildErrorJson(result.message)};
    } catch (const std::exception&) {
        return {400, "application/json", buildErrorJson("Update client request is invalid.")};
    }
}

HttpServer::HttpResponse HttpServer::handleEmployeeCreate(const HttpRequest& request) {
    try {
        Session* session = requireSession(request);
        if (session == nullptr || session->role != "admin") {
            return {403, "application/json", buildErrorJson("Only admins can create employees.")};
        }

        const std::unordered_map<std::string, std::string> payload = parseJsonObject(request.body);
        const OperationResult result = repository.createEmployee(payload.at("name"), payload.at("password"), toDouble(payload, "salary"));
        return {result.success ? 200 : 400, "application/json",
                result.success ? buildSuccessJson(result.message) : buildErrorJson(result.message)};
    } catch (const std::exception&) {
        return {400, "application/json", buildErrorJson("Create employee request is invalid.")};
    }
}

HttpServer::HttpResponse HttpServer::handleEmployeeUpdate(const HttpRequest& request) {
    try {
        Session* session = requireSession(request);
        if (session == nullptr || session->role != "admin") {
            return {403, "application/json", buildErrorJson("Only admins can update employees.")};
        }

        const std::unordered_map<std::string, std::string> payload = parseJsonObject(request.body);
        const OperationResult result = repository.updateEmployee(
            toInt(payload, "id"),
            payload.at("name"),
            payload.at("password"),
            toDouble(payload, "salary"));
        return {result.success ? 200 : 400, "application/json",
                result.success ? buildSuccessJson(result.message) : buildErrorJson(result.message)};
    } catch (const std::exception&) {
        return {400, "application/json", buildErrorJson("Update employee request is invalid.")};
    }
}

std::string HttpServer::makeHttpResponse(const HttpResponse& response) const {
    std::ostringstream output;
    const std::string statusText = response.status == 200 ? "OK"
                                 : response.status == 400 ? "Bad Request"
                                 : response.status == 401 ? "Unauthorized"
                                 : response.status == 403 ? "Forbidden"
                                 : response.status == 404 ? "Not Found"
                                 : "Internal Server Error";

    output << "HTTP/1.1 " << response.status << ' ' << statusText << "\r\n"
           << "Content-Type: " << response.contentType << "; charset=utf-8\r\n"
           << "Content-Length: " << response.body.size() << "\r\n"
           << "Connection: close\r\n\r\n"
           << response.body;
    return output.str();
}

std::unordered_map<std::string, std::string> HttpServer::parseJsonObject(const std::string& body) const {
    std::unordered_map<std::string, std::string> values;
    std::string content = trim(body);
    if (content.size() < 2 || content.front() != '{' || content.back() != '}') {
        throw std::runtime_error("Invalid JSON");
    }

    std::size_t index = 1;
    while (index + 1 < content.size()) {
        while (index < content.size() && (std::isspace(static_cast<unsigned char>(content[index])) || content[index] == ',')) {
            ++index;
        }
        if (index >= content.size() - 1 || content[index] == '}') {
            break;
        }
        if (content[index] != '"') {
            throw std::runtime_error("Expected key quote");
        }
        const std::size_t keyEnd = content.find('"', index + 1);
        if (keyEnd == std::string::npos) {
            throw std::runtime_error("Unterminated key");
        }
        const std::string key = content.substr(index + 1, keyEnd - index - 1);
        index = keyEnd + 1;
        while (index < content.size() && (std::isspace(static_cast<unsigned char>(content[index])) || content[index] == ':')) {
            ++index;
        }
        if (index >= content.size()) {
            break;
        }

        std::string value;
        if (content[index] == '"') {
            const std::size_t valueEnd = content.find('"', index + 1);
            if (valueEnd == std::string::npos) {
                throw std::runtime_error("Unterminated value");
            }
            value = content.substr(index + 1, valueEnd - index - 1);
            index = valueEnd + 1;
        } else {
            const std::size_t valueEnd = content.find_first_of(",}", index);
            value = trim(content.substr(index, valueEnd - index));
            index = valueEnd;
        }

        values[key] = value;
    }

    return values;
}

HttpServer::Session* HttpServer::requireSession(const HttpRequest& request) {
    const auto it = request.headers.find("X-Session-Token");
    if (it == request.headers.end()) {
        return nullptr;
    }

    auto sessionIt = sessions.find(it->second);
    if (sessionIt == sessions.end()) {
        return nullptr;
    }

    return &sessionIt->second;
}

const HttpServer::Session* HttpServer::requireSession(const HttpRequest& request) const {
    const auto it = request.headers.find("X-Session-Token");
    if (it == request.headers.end()) {
        return nullptr;
    }

    const auto sessionIt = sessions.find(it->second);
    if (sessionIt == sessions.end()) {
        return nullptr;
    }

    return &sessionIt->second;
}

std::string HttpServer::generateToken() {
    static std::mt19937 generator(std::random_device{}());
    static std::uniform_int_distribution<int> distribution(0, 15);
    std::ostringstream output;
    for (int index = 0; index < 32; ++index) {
        output << std::hex << distribution(generator);
    }
    return output.str();
}

void HttpServer::start(int port) {
    const int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket < 0) {
        throw std::runtime_error("Unable to create socket.");
    }

    int reuseAddress = 1;
    setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, &reuseAddress, sizeof(reuseAddress));

    sockaddr_in address{};
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(static_cast<uint16_t>(port));

    if (bind(serverSocket, reinterpret_cast<sockaddr*>(&address), sizeof(address)) < 0) {
        close(serverSocket);
        throw std::runtime_error("Unable to bind to the requested port.");
    }

    if (listen(serverSocket, 10) < 0) {
        close(serverSocket);
        throw std::runtime_error("Unable to start listening.");
    }

    std::cout << "BankSystem Phase 3 is running on http://localhost:" << port << "\n";

    while (true) {
        sockaddr_in clientAddress{};
        socklen_t clientLength = sizeof(clientAddress);
        const int clientSocket = accept(serverSocket, reinterpret_cast<sockaddr*>(&clientAddress), &clientLength);
        if (clientSocket < 0) {
            continue;
        }

        const HttpRequest request = parseRequest(clientSocket);
        const HttpResponse response = route(request);
        const std::string rawResponse = makeHttpResponse(response);
        send(clientSocket, rawResponse.c_str(), rawResponse.size(), 0);
        close(clientSocket);
    }
}