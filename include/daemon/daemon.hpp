#ifndef DAEMON_H
#define DAEMON_H


#include <string>

// original
#include "buffer_manager.hpp"
#include "input_manager.hpp"
#include "output_manager.hpp"
#include "state_manager.hpp"
#include "types.hpp"


namespace phantomboard::daemon
{
class Daemon {
public:
    Daemon(std::string input_device_path);
    ~Daemon();

    Daemon(const Daemon&) = delete;
    Daemon& operator=(const Daemon&) = delete;

    bool initialize();
    void run();
    void stop();

    PhantomMode mode();
    TextBufferView bufferView();

private:
    void handleAction(const InputAction& action);

    std::string input_device_path_;
    bool running_ = false;

    StateManager state_{};
    BufferManager buffer_{};
    InputManager input_{};
    OutputManager output_{};
};
}

#endif
