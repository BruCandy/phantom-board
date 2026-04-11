#include <iostream>
#include <utility>
#include <cstdint>

//pkg
#include <ibus.h>
#include <glib.h>

//original
#include "ime/ibus/ibus_client.hpp"
#include "ime/ibus/ibus_callbacks.hpp"


namespace phantomboard::daemon
{
IBusClient::IBusClient()
{
}

IBusClient::~IBusClient()
{
    shutdown();
}

bool IBusClient::initialize(const std::string& client_name)
{
    shutdown();

    ibus_init();

    bus_ = ibus_bus_new();
    if (bus_ == nullptr) {
        std::cerr << "Failed to create IBus bus\n";
        return false;
    }

    if (!ibus_bus_is_connected(bus_)) {
        std::cerr << "Failed to connect IBus bus\n";
        shutdown();
        return false;
    }

    ctx_ = ibus_bus_create_input_context(bus_, client_name.c_str());
    if (ctx_ == nullptr) {
        std::cerr << "Failed to create IBusInputContext\n";
        shutdown();
        return false;
    }

    g_signal_connect(ctx_,
                     "update-preedit-text",
                     G_CALLBACK(handleUpdatePreeditText),
                     &ime_state_);

    g_signal_connect(ctx_,
                     "hide-preedit-text",
                     G_CALLBACK(handleHidePreeditText),
                     &ime_state_);

    g_signal_connect(ctx_,
                     "commit-text",
                     G_CALLBACK(handleCommitText),
                     &ime_state_);

    ibus_input_context_set_capabilities(
        ctx_,
        IBUS_CAP_PREEDIT_TEXT | IBUS_CAP_FOCUS | IBUS_CAP_SURROUNDING_TEXT
    );

    focusIn();

    return true;
}

void IBusClient::shutdown()
{
    if (ctx_ != nullptr) {
        ibus_input_context_focus_out(ctx_);
        g_object_unref(ctx_);
        ctx_ = nullptr;
    }

    if (bus_ != nullptr) {
        g_object_unref(bus_);
        bus_ = nullptr;
    }

    ime_state_.preedit.clear();
    ime_state_.preedit_visible = false;
    ime_state_.committed.clear();
}

void IBusClient::focusIn()
{
    if (ctx_ != nullptr) {
        ibus_input_context_focus_in(ctx_);
        while (g_main_context_pending(nullptr)) {
            g_main_context_iteration(nullptr, FALSE);
        }
    }
}

void IBusClient::focusOut()
{
    if (ctx_ != nullptr) {
        ibus_input_context_focus_out(ctx_);
        while (g_main_context_pending(nullptr)) {
            g_main_context_iteration(nullptr, FALSE);
        }
    }
}

bool IBusClient::sendKey(std::uint32_t keyval,
                         std::uint32_t keycode,
                         std::uint32_t state)
{
    if (ctx_ == nullptr) {
        return false;
    }

    gboolean handled = ibus_input_context_process_key_event(
        ctx_,
        static_cast<guint>(keyval),
        static_cast<guint>(keycode),
        static_cast<guint>(state)
    );

    while (g_main_context_pending(nullptr)) {
        g_main_context_iteration(nullptr, FALSE);
    }

    if (!handled) {
        if (keyval >= 0x20 && keyval <= 0x7e) {
            char c = static_cast<char>(keyval);
            ime_state_.committed += c;
        } else if (keyval == IBUS_KEY_BackSpace) {
            if (!ime_state_.committed.empty()) {
                ime_state_.committed.pop_back();
            }
        }
    }

    return handled == TRUE;
}

std::string IBusClient::takeCommittedText()
{
    std::string out = std::move(ime_state_.committed);
    ime_state_.committed.clear();
    return out;
}
}
