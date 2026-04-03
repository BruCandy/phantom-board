#include <iostream>

// original
#include "daemon.hpp"


int main() {
    constexpr const char* kInputDevice = "/dev/input/bunny-kbd-event3";

    bunnyboard::daemon::Daemon daemon{kInputDevice};

    if (!daemon.initialize()) {
        std::cerr << "Failed to initialize bunnyboard-daemon\n";
        return 1;
    }

    daemon.run();
    return 0;
}