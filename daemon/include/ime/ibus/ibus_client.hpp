#ifndef IBUS_CLIENT_H
#define IBUS_CLIENT_H


#include <cstdint>
#include <string>

//pkg
#include <ibus.h>

//original
#include "ime/ibus/ibus_callbacks.hpp"


namespace phantomboard::daemon
{
class IBusClient
{
public:
    IBusClient();
    ~IBusClient();

    bool initialize(const std::string& client_name);
    void shutdown();

    void focusIn();
    void focusOut();

    bool sendKey(std::uint32_t keyval,
                 std::uint32_t keycode = 0,
                 std::uint32_t state = 0);

    const std::string& preedit() { return ime_state_.preedit; };
    bool preeditVisible() { return ime_state_.preedit_visible; };
    const std::string& committed() { return ime_state_.committed; };

    std::string takeCommittedText();

    bool checkImeActive() { return ime_state_.ime_active; } ;

    void toggleImeActive();

private:
    struct ImeState ime_state_;

    IBusBus* bus_ = nullptr;
    IBusInputContext* ctx_ = nullptr;
};
}

#endif
