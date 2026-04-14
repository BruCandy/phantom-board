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
        sdbus::registerMethod("GetBufferState").implementedAs(
            [this]() -> std::tuple<std::string, std::string, std::uint32_t> {
                return callbacks_.get_buffer_state
                     ? callbacks_.get_buffer_state()
                     : std::make_tuple(std::string{}, std::string{}, std::uint32_t{0});
        }),
        sdbus::registerSignal("ModeChanged").withParameters<std::string>(),
        sdbus::registerSignal("BufferChanged").withParameters<std::string, std::string, std::uint32_t>()
    ).forInterface(kInterfaceName);
}

void DbusAdaptor::emitModeChanged(const std::string& mode)
{
    object_->emitSignal("ModeChanged")
        .onInterface(kInterfaceName)
        .withArguments(mode);
}

void DbusAdaptor::emitBufferChanged(const std::string& committed, const std::string& preedit, std::uint32_t cursor)
{
    object_->emitSignal("BufferChanged")
        .onInterface(kInterfaceName)
        .withArguments(committed, preedit, cursor);
}
}
