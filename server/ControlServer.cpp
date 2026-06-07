
#include "ControlServer.h"
#include "DataServer.h"

#include "constants.h"

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
            int message_type = buffer[0];

            std::string msg;

            switch (message_type)
            {
            case ControlMessageType::STATUS:
                msg = "Status requested";

                write(client_fd, msg.data(), msg.size());
                break;

            case ControlMessageType::ACQUIRE:
                msg = "Acquire";

                write(client_fd, msg.data(), msg.size());
                break;

            case ControlMessageType::SET_EXPOSURE:
                msg = "Set Exposure";

                write(client_fd, msg.data(), msg.size());
                break;

            case ControlMessageType::SET_GAIN:
                msg = "Set Gain";

                write(client_fd, msg.data(), msg.size());
                break;

            default:
                msg = "Unknown Command";
            }

            std::cout << msg << std::endl;
        }

        std::cout << "Client Disconnected" << std::endl;


        close(client_fd);
    }

    std::cout << "Control server stopped\n";

}
