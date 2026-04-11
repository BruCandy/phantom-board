#include <iostream>

//pkg
#include <ibus.h>

//originals
#include "ime/ibus/ibus_callbacks.hpp"


namespace phantomboard::daemon
{
void handleUpdatePreeditText(IBusInputContext*,
                             IBusText* text,
                             guint cursor_pos,
                             gboolean visible,
                             ImeState* ime_state)
{
    if (ime_state == nullptr) {
        return;
    }

    const gchar* s = text ? ibus_text_get_text(text) : "";
    ime_state->preedit = (s != nullptr) ? s : "";
    ime_state->preedit_visible = (visible == TRUE);
}

void handleHidePreeditText(IBusInputContext*,
                           ImeState* ime_state)
{
    if (ime_state == nullptr) {
        return;
    }

    ime_state->preedit.clear();
    ime_state->preedit_visible = false;
}

void handleCommitText(IBusInputContext*,
                      IBusText* text,
                      ImeState* ime_state)
{
    if (ime_state == nullptr) {
        return;
    }

    const gchar* s = text ? ibus_text_get_text(text) : "";
    if (s != nullptr) {
        ime_state->committed += s;
    }
}
}
