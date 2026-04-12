#ifndef OUTPUT_MANAGER_H
#define OUTPUT_MANAGER_H


#include <string>


namespace phantomboard::daemon
{
class OutputManager {
public:
    OutputManager();
    ~OutputManager();

    OutputManager(const OutputManager&) = delete;
    OutputManager& operator=(const OutputManager&) = delete;

    bool initialize();
    void shutdown();

    bool writeEvent(int fd, int type, int code, int value);
    bool emitKey(int keycode);
    bool emitKeyEvent(int keycode, int value);
    bool emitSyn();
    bool emitText(const std::string& text);
    bool emitPassThroughKey(int keycode, int value);

    bool throwEvent(struct input_event event);

    bool writeClipboard(const std::string& text);
    bool clearClipboard();

private:
    int fd_ = -1;
};
}

#endif
