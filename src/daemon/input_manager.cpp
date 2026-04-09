#include <fcntl.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <string.h>

// linux
#include <linux/input.h>

// original
#include "input_manager.hpp"


namespace
{
constexpr int alpha_keys[26] = {
    KEY_A, KEY_B, KEY_C, KEY_D, KEY_E, KEY_F, KEY_G, KEY_H, KEY_I,
    KEY_J, KEY_K, KEY_L, KEY_M, KEY_N, KEY_O, KEY_P, KEY_Q, KEY_R,
    KEY_S, KEY_T, KEY_U, KEY_V, KEY_W, KEY_X, KEY_Y, KEY_Z
};

int alphaIndex(int code)
{
    for (int i = 0; i < 26; i++) {
        if (alpha_keys[i] == code) return i;
    }
    return -1;
}

char keycodeToAscii(int code, bool shift)
{
    const int idx = alphaIndex(code);
    if (idx >= 0) {
        const char base = shift ? 'A' : 'a';
        return static_cast<char>(base + idx);
    }

    if (code >= KEY_1 && code <= KEY_9) {
        if (!shift) {
            return static_cast<char>('1' + (code - KEY_1));
        }
        constexpr char shifted[] = { '!', '"', '#', '$', '%', '&', '\'', '(', ')'};
        return shifted[code - KEY_1];
    }

    if (code == KEY_0) {
        return shift ? '~' : '0';
    }

    switch (code) {
        case KEY_SPACE:      return ' ';
        case KEY_MINUS:      return shift ? '=' : '-';
        case KEY_EQUAL:      return shift ? '+' : '^';
        case KEY_LEFTBRACE:  return shift ? '{' : '@';
        case KEY_RIGHTBRACE: return shift ? '}' : '[';
        case KEY_BACKSLASH:  return shift ? '|' : '\\';
        case KEY_SEMICOLON:  return shift ? '+' : ';';
        case KEY_APOSTROPHE: return shift ? '*' : ':';
        case KEY_GRAVE:      return shift ? '_' : '`';
        case KEY_COMMA:      return shift ? '<' : ',';
        case KEY_DOT:        return shift ? '>' : '.';
        case KEY_SLASH:      return shift ? '?' : '/';
        default:             return '\0';
    }
}
}


namespace phantomboard::daemon
{
InputManager::InputManager()
{
}

InputManager::~InputManager()
{
    ungrab();
    closeDevice();
}

bool InputManager::openDevice(const std::string& device_path)
{
    closeDevice();

    fd_ = ::open(device_path.c_str(), O_RDONLY);
    return fd_ >= 0;
}

void InputManager::closeDevice()
{
    if (fd_ >= 0) {
        ::close(fd_);
        fd_ = -1;
    }
    grabbed_ = false;
}

bool InputManager::grab()
{
    if (fd_ < 0) {
        return false;
    }

    if (ioctl(fd_, EVIOCGRAB, 1) < 0) {
        return false;
    }

    grabbed_ = true;
    return true;
}

bool InputManager::ungrab()
{
    if (fd_ < 0 || !grabbed_) {
        return false;
    }

    if (::ioctl(fd_, EVIOCGRAB, 0) < 0) {
        return false;
    }

    grabbed_ = false;
    return true;
}

bool InputManager::checkOpen()
{
    return fd_ >= 0;
}

int InputManager::fd()
{
    return fd_;
}

bool InputManager::readEvent(struct input_event& event)
{
    if (fd_ < 0) {
        return false;
    }

    const ssize_t n = ::read(fd_, &event, sizeof(event));
    return n == static_cast<ssize_t>(sizeof(event));
}

InputAction InputManager::translateEvent(const struct input_event& event)
{
    static bool left_shift_pressed = false;
    static bool right_shift_pressed = false;
    static bool toggle_manegement = true;

    InputAction action{};

    if (event.type != EV_KEY) {
        return action;
    }

    if (event.code == KEY_LEFTSHIFT) {
        left_shift_pressed = (event.value != 0);
    } else if (event.code == KEY_RIGHTSHIFT) {
        right_shift_pressed = (event.value != 0);
    }

    if (left_shift_pressed && right_shift_pressed && toggle_manegement) {
        action.type = InputActionType::ToggleMode;
        toggle_manegement = false;
        return action;
    }

    if (!left_shift_pressed && !right_shift_pressed) {
        toggle_manegement = true;
    }

    if (event.value != 1) {
        return action;
    }

    if (event.code == KEY_ENTER) {
        action.type = InputActionType::EmitBuffer;
        return action;
    }

    if (event.code == KEY_ESC) {
        action.type = InputActionType::ClearBuffer;
        return action;
    }

    if (event.code == KEY_BACKSPACE) {
        action.type = InputActionType::Backspace;
        return action;
    }

    if (event.code == KEY_DELETE) {
        action.type = InputActionType::DeleteChar;
        return action;
    }

    if (event.code == KEY_LEFT) {
        action.type = InputActionType::MoveLeft;
        return action;
    }

    if (event.code == KEY_RIGHT) {
        action.type = InputActionType::MoveRight;
        return action;
    }

    const char ch = keycodeToAscii(event.code, (left_shift_pressed || right_shift_pressed));
    if (ch != '\0') {
        action.type = InputActionType::InsertChar;
        action.ch = ch;
    }

    return action;
}
}
