#ifndef DBUS_CLIENT_H
#define DBUS_CLIENT_H


#include <cstdint>
#include <functional>
#include <memory>
#include <string>

//sdbus-c++
#include <sdbus-c++/sdbus-c++.h>

//original
#include "state.hpp"


namespace phantomboard::ctl
{
struct DbusCallbacks
{
    std::function<void(const std::string&)> on_mode_changed;
    std::function<void(const std::string&, const std::string&, std::uint32_t)> on_buffer_changed;
};

class DbusClient
{
public:
    DbusClient(DbusCallbacks callbacks);
    ~DbusClient();

    DbusClient(const DbusClient&) = delete;
    DbusClient& operator=(const DbusClient&) = delete;

    bool fetchState(ViewState& state);

private:
    DbusCallbacks callbacks_;
    std::unique_ptr<sdbus::IConnection> connection_;
    std::unique_ptr<sdbus::IProxy> proxy_;
};
}

#endif
