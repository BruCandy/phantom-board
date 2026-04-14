#ifndef DBUS_SERVICE_H
#define DBUS_SERVICE_H


#include <cstdint>
#include <memory>
#include <string>

//sdbus-c++
#include <sdbus-c++/sdbus-c++.h>

//original
#include "dbus/dbus_adaptor.hpp"


namespace phantomboard::daemon
{
class DbusService
{
public:
    DbusService(DbusCallbacks callbacks);
    ~DbusService();

    DbusService(const DbusService&) = delete;
    DbusService& operator=(const DbusService&) = delete;

    bool start();
    void stop();

    void emitModeChanged(const std::string& mode);
    void emitBufferChanged(const std::string& committed, const std::string& preedit, std::uint32_t cursor);

private:
    DbusCallbacks callbacks_;
    std::unique_ptr<sdbus::IConnection> connection_;
    std::unique_ptr<DbusAdaptor> adaptor_;
    bool running_ = false;
};
}

#endif
