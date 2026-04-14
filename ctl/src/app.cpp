#include <chrono>
#include <iostream>
#include <thread>

//original
#include "app.hpp"
#include "dbus/dbus_client.hpp"


namespace phantomboard::ctl
{
std::atomic<bool> App::running_{true};

App::App()
{
}

App::~App()
{
}

void App::stop()
{
    running_ = false;
}

std::string App::buildBufferView(const std::string& committed,
                                  const std::string& preedit,
                                  std::uint32_t      cursor)
{
    std::string visible = committed;

    std::size_t pos = static_cast<std::size_t>(cursor);
    if (pos > committed.size()) {
        pos = committed.size();
    }

    if (pos <= visible.size()) {
        visible.insert(pos, "|");
    } else {
        visible += "|";
    }

    if (!preedit.empty()) {
        visible.insert(pos, "<" + preedit + ">");
    }

    return visible;
}

void App::render()
{
    if (!first_render_) {
        std::cout << "\033[2A";
    }
    std::cout << "\033[2K\rmode   : " << state_.mode << std::endl;
    std::cout << "\033[2K\rbuffer : " << buildBufferView(state_.committed, state_.preedit, state_.cursor) << std::endl;
    first_render_ = false;
}

void App::updateMode(const std::string& mode)
{
    std::lock_guard lock(mutex_);
    state_.mode = mode;
    render();
}

void App::updateBuffer(const std::string& committed, const std::string& preedit, std::uint32_t cursor)
{
    std::lock_guard lock(mutex_);
    state_.committed = committed;
    state_.preedit   = preedit;
    state_.cursor    = cursor;
    render();
}

int App::run()
{
    try {
        DbusClient dbus({
            .on_mode_changed   = [this](const std::string& mode) {
                updateMode(mode);
            },
            .on_buffer_changed = [this](const std::string& committed,
                                        const std::string& preedit,
                                        std::uint32_t      cursor) {
                updateBuffer(committed, preedit, cursor);
            },
        });

        {
            std::lock_guard lock(mutex_);
            if (!dbus.fetchState(state_)) {
                return 1;
            }
            render();
        }

        while (running_.load()) {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }

        return 0;
    } catch (const std::exception& e) {
        std::cerr << "ctl error" << std::endl;
        return 1;
    }
}
}
