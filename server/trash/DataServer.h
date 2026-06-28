
#ifndef SERVER_DATASERVER_H
#define SERVER_DATASERVER_H

#include <thread>
#include <atomic>

class ControlServer; // Need to declare here to avoid circular dependencies


constexpr uint8_t SEND_READY = 1 << 0;
constexpr uint8_t QUEUE_READY = 1 << 1;


class DataServer
{
public:
    DataServer(int data_port);
    ~DataServer();

    void start();
    void stop();

    void bind_control_server(ControlServer* control_server);
    bool is_ready();
    int queue_data(int data);
    int try_move_to_send_buffer();
    int transmit();

private:
    void run();

    int port;
    ControlServer* control_server_ = nullptr;

    std::atomic<bool> running_{false};
    std::thread worker_;

    int sending_buffer;

    std::atomic<uint8_t> buffer_flags{SEND_READY | QUEUE_READY};

    int receiving_buffer;

    int send_data();
};

#endif //SERVER_DATASERVER_H