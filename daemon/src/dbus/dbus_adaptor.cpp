#include <utility>

//original
#include <dbus_configure.hpp>
#include "dbus/dbus_adaptor.hpp"


namespace phantomboard::daemon
{
DbusAdaptor::DbusAdaptor(sdbus::IConnection& connection, DbusCallbacks callbacks)
    : callbacks_(std::move(callbacks)), object_(sdbus::createObject(connection, sdbus::ObjectPath{kObjectPath}))
{
    object_->addVTable(
        sdbus::registerMethod("GetMode").implementedAs([this]() -> std::string {
            return callbacks_.get_mode ? callbacks_.get_mode() : "";
        }),
        sdbus::registerSignal("ModeChanged").withParameters<std::string>()
    ).forInterface(kInterfaceName);
}

void DbusAdaptor::emitModeChanged(const std::string& mode)
{
    object_->emitSignal("ModeChanged")
        .onInterface(kInterfaceName)
        .withArguments(mode);
}
}
