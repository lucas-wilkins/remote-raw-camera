
#include "DataServer.h"
#include "ControlServer.h"

#include <atomic>
#include <chrono>
#include <iostream>
#include <thread>

DataServer::DataServer(const int data_port)
{
    port = data_port;
}

DataServer::~DataServer() {
    stop();
}

void DataServer::start() {
    bool expected = false;
    if (!running_.compare_exchange_strong(expected, true)) {
        return; // already running
    }

    worker_ = std::thread(&DataServer::run, this);
}

void DataServer::stop() {
    if (!running_) {
        return;
    }

    running_ = false;

    if (worker_.joinable()) {
        worker_.join();
    }
}

void DataServer::bind_control_server(ControlServer* control_server)
{
    control_server_ = control_server;
}

bool DataServer::is_ready()
{
    return QUEUE_READY;
}

int DataServer::queue_data(int data)
{
    if (QUEUE_READY)
    {
        receiving_buffer = data; // Dummy data for now
        QUEUE_READY = false;

        try_move_to_send_buffer();

        return 0;
    } else
    {
        return -1;
    }

}

int DataServer::send_data()
{
    // Require buffer to be full
    if (!SEND_READY)
    {
        // Do send here

        SEND_READY = true;

        try_move_to_send_buffer();

        return 0;
    }

    return -1;
}


int DataServer::try_move_to_send_buffer()
{
    if (SEND_READY && !QUEUE_READY)
    {
        sending_buffer = receiving_buffer;
        QUEUE_READY = true;
        SEND_READY = false;

        return 0;
    } else
    {
        return -1;
    }

}

int DataServer::transmit()
{
    return 0;
}



void DataServer::run() {
    std::cout << "Data server started\n";

    while (running_) {
        // Handle connections, requests, timers, etc.
        std::cout << "Data server working...\n";

        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    std::cout << "Data server stopped\n";

}
