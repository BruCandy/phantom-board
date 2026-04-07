#ifndef INPUT_MANAGER_H
#define INPUT_MANAGER_H


#include <string>

// linux
#include <linux/input.h>

// original
#include "types.hpp"


namespace phantomboard::daemon 
{
class InputManager {
public:
    InputManager();
    ~InputManager();

    InputManager(const InputManager&) = delete;
    InputManager& operator=(const InputManager&) = delete;

    bool openDevice(const std::string& device_path);
    void closeDevice();

    bool grab();
    bool ungrab();

    bool checkOpen();
    int fd();

    bool readEvent(struct input_event& event);
    InputAction translateEvent(const struct input_event& event);

private:
    int fd_ = -1;
    bool grabbed_ = false;
};
}

#endif
