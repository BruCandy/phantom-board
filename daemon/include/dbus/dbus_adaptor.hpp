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
};

class DbusAdaptor
{
public:
    DbusAdaptor(sdbus::IConnection& connection, DbusCallbacks callbacks);
    ~DbusAdaptor() = default;

    DbusAdaptor(const DbusAdaptor&) = delete;
    DbusAdaptor& operator=(const DbusAdaptor&) = delete;

    void emitModeChanged(const std::string& mode);

private:
    DbusCallbacks callbacks_;
    std::unique_ptr<sdbus::IObject> object_;
};
}

#endif
