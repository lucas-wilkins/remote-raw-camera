

#include <iostream>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>

#include "server.h"
#include "constants.h"

int run_server(const int port)
{

    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        perror("socket");
        return 1;
    }

    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(port);

    if (bind(server_fd, (sockaddr*)&addr, sizeof(addr)) < 0) {
        perror("bind");
        return 1;
    }

    if (listen(server_fd, 5) < 0) {
        perror("listen");
        return 1;
    }

    sockaddr_in client{};
    socklen_t client_len = sizeof(client);

    char buffer[1024];

    while (true) {

        std::cout << "TCP server listening on port " << port << std::endl;

        int client_fd = accept(server_fd, (sockaddr*)&client, &client_len);
        if (client_fd < 0) {
            perror("accept");
            return 1;
        }

        std::cout << "Client Connected" << std::endl;

        ssize_t n;
        while ((n = read(client_fd, buffer, sizeof(buffer))) > 0) {
            int message_type = buffer[0];

            std::string msg;

            switch (message_type)
            {
            case MessageType::STATUS:
                msg = "Status requested";

                write(client_fd, msg.data(), msg.size());
                break;

            case MessageType::ACQUIRE:
                msg = "Acquire";

                write(client_fd, msg.data(), msg.size());
                break;

            case MessageType::SET_EXPOSURE:
                msg = "Set Exposure";

                write(client_fd, msg.data(), msg.size());
                break;

            case MessageType::SET_GAIN:
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

    close(server_fd);
    return 0;
}