#include <iostream>
#include <utility>

//original
#include "dbus/dbus_service.hpp"
#include "dbus/dbus_configure.hpp"


namespace phantomboard::daemon
{
DbusService::DbusService(DbusCallbacks callbacks)
    : callbacks_(std::move(callbacks))
{
}

DbusService::~DbusService()
{
    stop();
}

bool DbusService::start()
{
    if (running_) {
        return true;
    }

    try {
        connection_ = sdbus::createSessionBusConnection(sdbus::ServiceName{kServiceName});
        adaptor_ = std::make_unique<DbusAdaptor>(*connection_, callbacks_);
        connection_->enterEventLoopAsync();
    }
    catch (const sdbus::Error& e) {
        std::cerr << "Failed to start D-Bus service" << std::endl;
        adaptor_.reset();
        connection_.reset();
        return false;
    }

    running_ = true;
    return true;
}

void DbusService::stop()
{
    if (!running_) {
        return;
    }

    connection_->leaveEventLoop();
    adaptor_.reset();
    connection_.reset();

    running_ = false;
}

void DbusService::emitModeChanged(const std::string& mode)
{
    if (adaptor_) {
        adaptor_->emitModeChanged(mode);
    }
}
}
