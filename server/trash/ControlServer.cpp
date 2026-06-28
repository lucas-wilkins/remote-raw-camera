
#include "ControlServer.h"
#include "../DataServer.h"

#include "../constants.h"

#include <atomic>
#include <iostream>
#include <thread>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>

ControlServer::ControlServer(const int control_port)
{
    port = control_port;
}

ControlServer::~ControlServer() {
    stop();
}

void ControlServer::start() {
    bool expected = false;
    if (!running_.compare_exchange_strong(expected, true)) {
        return; // already running
    }

    worker_ = std::thread(&ControlServer::run, this);
}

void ControlServer::stop() {
    if (!running_) {
        return;
    }

    running_ = false;

    if (worker_.joinable()) {
        worker_.join();
    }
}

void ControlServer::bind_data_server(DataServer* data_server)
{
    data_server_ = data_server;
}

void ControlServer::process_message(const int message, const int client_fd)
{

    std::string response;

    if (data_server_ == nullptr)
    {
        response = "ERROR: Data server not set up";
    }
    else
    {
        switch (message)
        {
        case ControlMessageType::STATUS:
            response = "Status requested";

            write(client_fd, response.data(), response.size());
            break;

        case ControlMessageType::CAPTURE:
            response = "Acquire";

            write(client_fd, response.data(), response.size());
            break;

        case ControlMessageType::SET_EXPOSURE:
            response = "Set Exposure";

            write(client_fd, response.data(), response.size());
            break;

        case ControlMessageType::SET_GAIN:
            response = "Set Gain";

            write(client_fd, response.data(), response.size());
            break;

        default:
            response = "Unknown Command";
        }
    }

    std::cout << response << std::endl;
}


void ControlServer::run() {
    std::cout << "Control server starting\n";

    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        perror("socket");
        return;
    }

    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(port);

    if (bind(server_fd, (sockaddr*)&addr, sizeof(addr)) < 0) {
        perror("bind");
        return;
    }

    if (listen(server_fd, 5) < 0) {
        perror("listen");
        return;
    }

    sockaddr_in client{};
    socklen_t client_len = sizeof(client);

    char buffer[1024];

    while (running_)
    {
        std::cout << "TCP server listening on port " << port << std::endl;

        int client_fd = accept(server_fd, (sockaddr*)&client, &client_len);
        if (client_fd < 0) {
            perror("accept");
            return;
        }

        std::cout << "Client Connected" << std::endl;

        ssize_t n;
        while ((n = read(client_fd, buffer, sizeof(buffer))) > 0) {
            process_message(buffer[0], client_fd);
        }

        std::cout << "Client Disconnected" << std::endl;


        close(client_fd);
    }

    std::cout << "Control server stopped\n";

}
