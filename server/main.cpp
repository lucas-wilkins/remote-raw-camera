#include <iostream>
#include <thread>

#include "lib/cxxopts.hpp"

#include "constants.h"
#include "servers.h"


int main(int argc, char* argv[]) {
    cxxopts::Options options("raw-camera", "Serves raw camera data");

    options.add_options()
        ("h,help", "Print usage")
        ("c,control", "TCP port for control signals",
            cxxopts::value<int>()->default_value(std::to_string(DEFAULT_CONTROL_PORT)))
        ("d,data", "TCP port for data",
            cxxopts::value<int>()->default_value(std::to_string(DEFAULT_DATA_PORT)));

    auto result = options.parse(argc, argv);

    options.parse_positional({"control", "data"});

    // Help
    if (result.count("help")) {
        std::cout << options.help() << "\n";
        return 0;
    }

    // Get the positional integer (uses default if not provided)
    int control_port = result["control"].as<int>();
    int data_port = result["data"].as<int>();


    ControlServer control_server = ControlServer(control_port);
    DataServer data_server = DataServer(data_port);

    // data_server->bind_control_server(control_server);
    // control_server->bind_data_server(data_server);
    //
    // control_server->start();
    // data_server->start();
    //
    // std::this_thread::sleep_for(std::chrono::seconds(5));
    //
    // control_server->stop();
    // data_server->stop();

    //bufferSystemTest();

    while (true)
    {
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}
