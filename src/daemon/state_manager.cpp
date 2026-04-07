//original
#include "state_manager.hpp"


namespace phantomboard::daemon
{
StateManager::StateManager() 
{
}

StateManager::~StateManager() 
{
}

PhantomMode StateManager::getMode() 
{
    return mode_;
}

bool StateManager::checkNormal() 
{
    return mode_ == PhantomMode::Normal;
}

bool StateManager::checkPhantom() 
{
    return mode_ == PhantomMode::Phantom;
}

void StateManager::setMode(PhantomMode mode) 
{
    mode_ = mode;
}

void StateManager::toggleMode() 
{
    mode_ = (mode_ == PhantomMode::Normal) ? PhantomMode::Phantom : PhantomMode::Normal;
}
}
