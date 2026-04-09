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

    bool checkReady();
    int fd();

    bool emitKey(int keycode);
    bool emitKeyEvent(int keycode, int value);
    bool emitSyn();
    bool emitText(const std::string& text);

    bool throwEvent(struct input_event event);

private:
    int fd_ = -1;
};
}

#endif
