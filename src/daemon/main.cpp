#include <iostream>
#include <sstream>
#include <string>

// original
#include "daemon.hpp"


int main(int argc, char *argv[]) {
    constexpr const char* kInputDevicePrefix = "/dev/input/phantom-kbd-";

    if (argc < 2) {
        std::cerr << "Not enough arguments were given" << std::endl;
        return 1;
    }

    std::stringstream ss;
    ss << kInputDevicePrefix << argv[1];
    std::string input_device_path = ss.str();

    phantomboard::daemon::Daemon daemon{input_device_path};

    if (!daemon.initialize()) {
        std::cerr << "Failed to initialize phantomboard-daemon" << std::endl;
        return 1;
    }

    daemon.run();
    return 0;
}