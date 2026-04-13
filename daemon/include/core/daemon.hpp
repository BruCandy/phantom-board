#ifndef DAEMON_H
#define DAEMON_H


#include <optional>
#include <string>

// original
#include "core/buffer_manager.hpp"
#include "core/input_manager.hpp"
#include "core/output_manager.hpp"
#include "core/state_manager.hpp"
#include "core/types.hpp"
#include "dbus/dbus_service.hpp"
#include "ime/ibus/ibus_client.hpp"


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
    void updateBufferfromIme();

    std::string input_device_path_;
    bool running_ = false;

    StateManager state_{};
    BufferManager buffer_{};
    InputManager input_{};
    OutputManager output_{};
    IBusClient ime_{};
    std::optional<DbusService> dbus_service_;
};
}

#endif
