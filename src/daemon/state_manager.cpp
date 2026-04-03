//original
#include "state_manager.hpp"


namespace bunnyboard::daemon
{
StateManager::StateManager() 
{
}

StateManager::~StateManager() 
{
}

BunnyMode StateManager::getMode() 
{
    return mode_;
}

bool StateManager::checkNormal() 
{
    return mode_ == BunnyMode::Normal;
}

bool StateManager::checkBunny() 
{
    return mode_ == BunnyMode::Bunny;
}

void StateManager::setMode(BunnyMode mode) 
{
    mode_ = mode;
}

void StateManager::toggleMode() 
{
    mode_ = (mode_ == BunnyMode::Normal) ? BunnyMode::Bunny : BunnyMode::Normal;
}
}
