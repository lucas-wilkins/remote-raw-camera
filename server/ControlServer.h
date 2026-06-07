
#ifndef SERVER_CONTROLSERVER_H
#define SERVER_CONTROLSERVER_H

#include <thread>
#include <atomic>

class DataServer; // Need to declare here to avoid circular dependencies

class ControlServer
{
public:
    ControlServer(int control_port);
    ~ControlServer();

    void start();
    void stop();

    void bind_data_server(DataServer* data_server);

private:
    void run();

    int port;
    DataServer* data_server_ = nullptr;

    std::atomic<bool> running_{false};
    std::thread worker_;



};

#endif //SERVER_CONTROLSERVER_H