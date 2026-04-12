#include <fcntl.h>
#include <unistd.h>
#include <cstdio>
#include <cstdlib>

// linux
#include <linux/input.h>
#include <linux/uinput.h>

// sys
#include <sys/ioctl.h>
#include <sys/wait.h>

// original
#include "core/output_manager.hpp"


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

bool OutputManager::writeEvent(int fd, int type, int code, int value)
{
    struct input_event event {};
    event.type = static_cast<__u16>(type);
    event.code = static_cast<__u16>(code);
    event.value = value;

    const ssize_t n = ::write(fd, &event, sizeof(event));
    return n == static_cast<ssize_t>(sizeof(event));
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

    if (!writeClipboard(text)) {
        return false;
    }
    usleep(50000);

    if (!emitKeyEvent(KEY_LEFTCTRL, 1)) return false;
    if (!emitSyn()) return false;

    if (!emitKeyEvent(KEY_V, 1)) return false;
    if (!emitSyn()) return false;

    if (!emitKeyEvent(KEY_V, 0)) return false;
    if (!emitSyn()) return false;

    if (!emitKeyEvent(KEY_LEFTCTRL, 0)) return false;
    if (!emitSyn()) return false;

    usleep(100000);
    clearClipboard();

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

bool OutputManager::writeClipboard(const std::string& text)
{
    int pipefd[2];
    if (::pipe(pipefd) < 0) {
        return false;
    }

    pid_t pid = ::fork();
    if (pid < 0) {
        ::close(pipefd[0]);
        ::close(pipefd[1]);
        return false;
    } else if (pid == 0) {
        ::close(pipefd[1]);
        ::dup2(pipefd[0], STDIN_FILENO);
        ::close(pipefd[0]);
        ::execlp("xclip", "xclip", "-selection", "clipboard", "-in", nullptr);
        ::_exit(1);
    } else {
        ::close(pipefd[0]);
        const ssize_t written = ::write(pipefd[1], text.data(), text.size());
        ::close(pipefd[1]);

        int status = 0;
        ::waitpid(pid, &status, 0);

        return (written == static_cast<ssize_t>(text.size())) && WIFEXITED(status) && (WEXITSTATUS(status) == 0);
    }
}

bool OutputManager::clearClipboard()
{
    return std::system("printf '' | xclip -selection clipboard") == 0;
}
}
