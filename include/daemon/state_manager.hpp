#ifndef STATE_MANAGER_H
#define STATE_MANAGER_H


// original
#include "types.hpp"


namespace bunnyboard::daemon
{

class StateManager {
public:
    StateManager();
    ~StateManager();

    BunnyMode getMode();
    bool checkNormal();
    bool checkBunny();

    void setMode(BunnyMode mode);
    void toggleMode();

private:
    BunnyMode mode_ = BunnyMode::Normal;
};
}

#endif
