
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


void DataServer::run() {
    std::cout << "Data server started\n";

    while (running_) {
        // Handle connections, requests, timers, etc.
        std::cout << "Data server working...\n";

        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    std::cout << "Data server stopped\n";

}
