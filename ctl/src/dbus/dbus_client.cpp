#include <iostream>

//sdbus-c++
#include <sdbus-c++/sdbus-c++.h>

//original
#include <dbus_configure.hpp>
#include "dbus/dbus_client.hpp"


using namespace phantomboard::daemon;

namespace phantomboard::ctl
{
DbusClient::DbusClient(DbusCallbacks callbacks)
    : callbacks_(std::move(callbacks))
{
    connection_ = sdbus::createSessionBusConnection();
    proxy_ = sdbus::createProxy(*connection_, sdbus::ServiceName{kServiceName}, sdbus::ObjectPath{kObjectPath});

    proxy_->uponSignal("ModeChanged")
        .onInterface(kInterfaceName)
        .call([this](const std::string& mode) {
            if (callbacks_.on_mode_changed) {
                callbacks_.on_mode_changed(mode);
            }
        });

    proxy_->uponSignal("BufferChanged")
        .onInterface(kInterfaceName)
        .call([this](const std::string& committed, const std::string& preedit, std::uint32_t cursor) {
            if (callbacks_.on_buffer_changed) {
                callbacks_.on_buffer_changed(committed, preedit, cursor);
            }
        });

    connection_->enterEventLoopAsync();
}

DbusClient::~DbusClient()
{
    connection_->leaveEventLoop();
}

bool DbusClient::fetchState(ViewState& state)
{
    try {
        proxy_->callMethod("GetMode")
            .onInterface(kInterfaceName)
            .storeResultsTo(state.mode);

        proxy_->callMethod("GetBufferState")
            .onInterface(kInterfaceName)
            .storeResultsTo(state.committed, state.preedit, state.cursor);
        
        return true;
    } catch (const std::exception& e) {
        std::cerr << "failed to fetch state" << std::endl;
        return false;
    }
}
}
