
#ifndef SERVER_DATASERVER_H
#define SERVER_DATASERVER_H

#include <thread>
#include <atomic>

class ControlServer; // Need to declare here to avoid circular dependencies

class DataServer
{
public:
    DataServer(int data_port);
    ~DataServer();

    void start();
    void stop();

    void bind_control_server(ControlServer* control_server);

private:
    void run();

    int port;
    ControlServer* control_server_ = nullptr;

    std::atomic<bool> running_{false};
    std::thread worker_;
};

#endif //SERVER_DATASERVER_H