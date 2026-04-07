#ifndef STATE_MANAGER_H
#define STATE_MANAGER_H


// original
#include "types.hpp"


namespace phantomboard::daemon
{

class StateManager {
public:
    StateManager();
    ~StateManager();

    PhantomMode getMode();
    bool checkNormal();
    bool checkPhantom();

    void setMode(PhantomMode mode);
    void toggleMode();

private:
    PhantomMode mode_ = PhantomMode::Normal;
};
}

#endif
