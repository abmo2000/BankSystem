#pragma once

#include "BankRepository.h"

#include <filesystem>
#include <string>
#include <unordered_map>

class HttpServer {
private:
    struct Session {
        std::string token;
        std::string role;
        int userId;
    };

    struct HttpRequest {
        std::string method;
        std::string path;
        std::unordered_map<std::string, std::string> headers;
        std::string body;
    };

    struct HttpResponse {
        int status;
        std::string contentType;
        std::string body;
    };

    BankRepository& repository;
    std::filesystem::path webRoot;
    std::unordered_map<std::string, Session> sessions;

    HttpRequest parseRequest(int clientSocket) const;
    HttpResponse route(const HttpRequest& request);
    HttpResponse serveStaticFile(const std::string& requestPath) const;
    HttpResponse handleDashboard() const;
    HttpResponse handleSession(const HttpRequest& request) const;
    HttpResponse handleLogin(const HttpRequest& request);
    HttpResponse handleLogout(const HttpRequest& request);
    HttpResponse handleClientDeposit(const HttpRequest& request);
    HttpResponse handleClientWithdraw(const HttpRequest& request);
    HttpResponse handleClientTransfer(const HttpRequest& request);
    HttpResponse handleClientCreate(const HttpRequest& request);
    HttpResponse handleClientUpdate(const HttpRequest& request);
    HttpResponse handleEmployeeCreate(const HttpRequest& request);
    HttpResponse handleEmployeeUpdate(const HttpRequest& request);

    std::string makeHttpResponse(const HttpResponse& response) const;
    std::unordered_map<std::string, std::string> parseJsonObject(const std::string& body) const;
    Session* requireSession(const HttpRequest& request);
    const Session* requireSession(const HttpRequest& request) const;
    std::string generateToken();

public:
    HttpServer(BankRepository& repository, const std::filesystem::path& webRoot);
    void start(int port);
};