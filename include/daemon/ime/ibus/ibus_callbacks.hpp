#ifndef IBUS_CALLBACKS_H
#define IBUS_CALLBACKS_H

#include <cstdint>
#include <string>
#include <ibus.h>

namespace phantomboard::daemon
{
struct ImeState
{
    std::string preedit;
    bool preedit_visible = false;
    std::string committed;
    bool ime_active = false;
};

void handleUpdatePreeditText(IBusInputContext* context,
                             IBusText* text,
                             guint cursor_pos,
                             gboolean visible,
                             ImeState* ime_state);

void handleHidePreeditText(IBusInputContext* context,
                           ImeState* ime_state);

void handleCommitText(IBusInputContext* context,
                      IBusText* text,
                      ImeState* ime_state);
}

#endif
