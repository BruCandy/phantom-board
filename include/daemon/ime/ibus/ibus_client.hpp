#ifndef IBUS_CLIENT
#define IBUS_CLIENT


#include <cstdint>
#include <string>

//pkg
#include <ibus.h>
#include <glib.h>


namespace phantomboard::daemon
{
class IBusClient
{
public:
    IBusClient();
    ~IBusClient();

    IBusClient(const IBusClient&) = delete;
    IBusClient& operator=(const IBusClient&) = delete;

    bool initialize(const std::string& client_name = "phantom-board");
    void shutdown();

    void focusIn();
    void focusOut();

    bool sendKey(std::uint32_t keyval,
                 std::uint32_t keycode = 0,
                 std::uint32_t state = 0);

    const std::string& preedit() { return preedit_ };
    bool preeditVisible() { return preedit_visible_ };
    const std::string& committed() { return committed_ };
    
    void clearForwardedKey();

private:
    struct ImeState {
        bool preedit_visible_;
        std::string preedit;
        std::string committed;
    };

    void connectSignals();
    void setCapabilities();

    void handleUpdatePreeditText(IBusInputContext*, 
                                 IBusText* text, 
                                 guint cursor_pos, 
                                 gboolean visible,
                                 ImeState* ime_state);
    void handleHidePreeditText(IBusInputContext*,
                               ImeState* ime_state);
    void handleCommitText(IBusInputContext*,
                          IBusText* text,
                          ImeState* ime_state);
    void handleForwardKeyEvent(IBusInputContext*,
                               guint keyval, 
                               guint keycode, 
                               guint state,
                               ImeState* ime_state);

    struct ImeState ime_state_;

    IBusBus* bus_;
    IBusInputContext* ctx_;

    std::uint32_t last_forward_keyval_;
    std::uint32_t last_forward_keycode_;
    std::uint32_t last_forward_state_;
};
}

#endif
