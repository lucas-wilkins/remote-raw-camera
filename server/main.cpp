#include <iostream>

#include "lib/cxxopts.hpp"

#include "constants.h"
#include "server.h"


int main(int argc, char* argv[]) {
    cxxopts::Options options("raw-camera", "Serves raw camera data");

    options.add_options()
        ("h,help", "Print usage")
        ("port", "TCP Port to listen on", cxxopts::value<int>()->default_value(std::to_string(DEFAULT_PORT)));

    auto result = options.parse(argc, argv);

    options.parse_positional({"port"});

    // Help
    if (result.count("help")) {
        std::cout << options.help() << "\n";
        return 0;
    }

    // Get the positional integer (uses default if not provided)
    int port = result["port"].as<int>();

    return run_server(port);

}