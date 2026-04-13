#include <utility>

//original
#include <dbus_configure.hpp>
#include "dbus/dbus_adaptor.hpp"


namespace phantomboard::daemon
{
DbusAdaptor::DbusAdaptor(sdbus::IConnection& connection, DbusCallbacks callbacks)
    : callbacks_(std::move(callbacks)), object_(sdbus::createObject(connection, kObjectPath))
{
    object_->registerMethod("GetMode")
        .onInterface(kInterfaceName)
        .implementedAs([this]() -> std::string {
            return callbacks_.get_mode ? callbacks_.get_mode() : "";
        });

    object_->registerSignal("ModeChanged")
        .onInterface(kInterfaceName)
        .withParameters<std::string>();

    object_->finishRegistration();
}

void DbusAdaptor::emitModeChanged(const std::string& mode)
{
    object_->emitSignal("ModeChanged")
        .onInterface(kInterfaceName)
        .withArguments(mode);
}
}
