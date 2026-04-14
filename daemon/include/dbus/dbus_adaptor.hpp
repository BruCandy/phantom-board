#ifndef DBUS_ADAPTOR_H
#define DBUS_ADAPTOR_H


#include <cstdint>
#include <functional>
#include <memory>
#include <string>
#include <tuple>

//sdbus-c++
#include <sdbus-c++/sdbus-c++.h>


namespace phantomboard::daemon
{
struct DbusCallbacks {
    std::function<std::string()> get_mode;
    std::function<std::tuple<std::string, std::string, std::uint32_t>()> get_buffer_state;
};

class DbusAdaptor
{
public:
    DbusAdaptor(sdbus::IConnection& connection, DbusCallbacks callbacks);
    ~DbusAdaptor() = default;

    DbusAdaptor(const DbusAdaptor&) = delete;
    DbusAdaptor& operator=(const DbusAdaptor&) = delete;

    void emitModeChanged(const std::string& mode);
    void emitBufferChanged(const std::string& committed, const std::string& preedit, std::uint32_t cursor);

private:
    DbusCallbacks callbacks_;
    std::unique_ptr<sdbus::IObject> object_;
};
}

#endif
