#include <fcntl.h>
#include <unistd.h>
#include <cstdio>

// linux
#include <linux/input.h>
#include <linux/uinput.h>

// sys
#include <sys/ioctl.h>

// original
#include "core/output_manager.hpp"


namespace
{
constexpr int alphabet_keys[26] = {
    KEY_A, KEY_B, KEY_C, KEY_D, KEY_E, KEY_F, KEY_G, KEY_H, KEY_I,
    KEY_J, KEY_K, KEY_L, KEY_M, KEY_N, KEY_O, KEY_P, KEY_Q, KEY_R,
    KEY_S, KEY_T, KEY_U, KEY_V, KEY_W, KEY_X, KEY_Y, KEY_Z
};

int asciiToKeycode(char c, bool& need_shift)
{
    need_shift = false;

    if (c >= 'a' && c <= 'z') {
        return alphabet_keys[c - 'a'];
    }

    if (c >= 'A' && c <= 'Z') {
        need_shift = true;
        return alphabet_keys[c - 'A'];
    }

    if (c >= '1' && c <= '9') {
        return KEY_1 + (c - '1');
    }

    if (c == '0') {
        return KEY_0;
    }

    switch (c) {
        case ' ':   return KEY_SPACE;

        case '-':   return KEY_MINUS;
        case '=':   need_shift = true; return KEY_MINUS;

        case '^':   return KEY_EQUAL;
        case '~':   need_shift = true; return KEY_EQUAL;

        case '\\':  return KEY_YEN;
        case '|':   need_shift = true; return KEY_YEN; 

        case '@':   return KEY_LEFTBRACE;
        case '`':   need_shift = true; return KEY_LEFTBRACE;

        case '[':   return KEY_RIGHTBRACE;
        case '{':   need_shift = true; return KEY_RIGHTBRACE;

        case ';':   return KEY_SEMICOLON;
        case '+':   need_shift = true; return KEY_SEMICOLON;

        case ':':   return KEY_APOSTROPHE;
        case '*':   need_shift = true; return KEY_APOSTROPHE;

        case ']':   return KEY_BACKSLASH;
        case '}':   need_shift = true; return KEY_BACKSLASH;

        case ',':   return KEY_COMMA;
        case '<':   need_shift = true; return KEY_COMMA;

        case '.':   return KEY_DOT;
        case '>':   need_shift = true; return KEY_DOT;

        case '/':   return KEY_SLASH;
        case '?':   need_shift = true; return KEY_SLASH;

        // case '\\':  return KEY_RO;
        case '_':   need_shift = true; return KEY_RO;

        case '!':   need_shift = true; return KEY_1;
        case '"':   need_shift = true; return KEY_2;
        case '#':   need_shift = true; return KEY_3;
        case '$':   need_shift = true; return KEY_4;
        case '%':   need_shift = true; return KEY_5;
        case '&':   need_shift = true; return KEY_6;
        case '\'':  need_shift = true; return KEY_7;
        case '(':   need_shift = true; return KEY_8;
        case ')':   need_shift = true; return KEY_9;
        // case '~':   need_shift = true; return KEY_0;

        default:
            return -1;
    }
}

bool writeEvent(int fd, int type, int code, int value) {
    struct input_event event {};
    event.type = static_cast<__u16>(type);
    event.code = static_cast<__u16>(code);
    event.value = value;

    const ssize_t n = ::write(fd, &event, sizeof(event));
    return n == static_cast<ssize_t>(sizeof(event));
}
}


namespace phantomboard::daemon 
{
OutputManager::OutputManager()
{
}

OutputManager::~OutputManager() 
{
    shutdown();
}

bool OutputManager::initialize()
{
    shutdown();

    fd_ = ::open("/dev/uinput", O_WRONLY | O_NONBLOCK);
    if (fd_ < 0) {
        std::perror("Failed to open /dev/uinput");
        return false;
    }

    if (::ioctl(fd_, UI_SET_EVBIT, EV_KEY) < 0) {
        shutdown();
        return false;
    }

    if (::ioctl(fd_, UI_SET_EVBIT, EV_SYN) < 0) {
        shutdown();
        return false;
    }

    for (int key = 0; key < 256; ++key) {
        ::ioctl(fd_, UI_SET_KEYBIT, key);
    }

    struct uinput_user_dev uidev {};
    std::snprintf(uidev.name, UINPUT_MAX_NAME_SIZE, "phantomboard-virtual-keyboard");
    uidev.id.bustype = BUS_USB;
    uidev.id.vendor = 0x1234;
    uidev.id.product = 0x5678;
    uidev.id.version = 1;

    if (::write(fd_, &uidev, sizeof(uidev)) < 0) {
        shutdown();
        return false;
    }

    if (::ioctl(fd_, UI_DEV_CREATE) < 0) {
        shutdown();
        return false;
    }

    return true;
}

void OutputManager::shutdown()
{
    if (fd_ >= 0) {
        ::ioctl(fd_, UI_DEV_DESTROY);
        ::close(fd_);
        fd_ = -1;
    }
}

bool OutputManager::checkReady()
{
    return fd_ >= 0;
}

int OutputManager::fd()
{
    return fd_;
}

bool OutputManager::emitKeyEvent(int keycode, int value)
{
    if (fd_ < 0) {
        return false;
    }
    return writeEvent(fd_, EV_KEY, keycode, value);
}

bool OutputManager::emitSyn()
{
    if (fd_ < 0) {
        return false;
    }
    return writeEvent(fd_, EV_SYN, SYN_REPORT, 0);
}

bool OutputManager::emitKey(int keycode)
{
    if (!emitKeyEvent(keycode, 1)) return false;
    if (!emitSyn()) return false;
    if (!emitKeyEvent(keycode, 0)) return false;
    if (!emitSyn()) return false;
    return true;
}

bool OutputManager::emitText(const std::string& text)
{
    if (fd_ < 0) {
        return false;
    }

    for (char c : text) {
        bool need_shift = false;
        const int keycode = asciiToKeycode(c, need_shift);
        if (keycode < 0) {
            continue;
        }

        if (need_shift) {
            if (!emitKeyEvent(KEY_LEFTSHIFT, 1)) return false;
            if (!emitSyn()) return false;
        }

        if (!emitKey(keycode)) return false;

        if (need_shift) {
            if (!emitKeyEvent(KEY_LEFTSHIFT, 0)) return false;
            if (!emitSyn()) return false;
        }
    }

    return true;
}

bool OutputManager::throwEvent(struct input_event event)
{
    if (fd_ < 0) {
        return false;
    }

    const ssize_t n = ::write(fd_, &event, sizeof(event));
    return n == static_cast<ssize_t>(sizeof(event));
}

bool OutputManager::emitPassThroughKey(int keycode, int value)
{
    return emitKeyEvent(keycode, value) && emitSyn();
}
}
