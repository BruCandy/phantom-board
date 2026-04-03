#ifndef OUTPUT_MANAGER_H
#define OUTPUT_MANAGER_H


#include <string>


namespace bunnyboard::daemon
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
    bool emitText(const std::string& text);

private:
    bool emitKeyEvent(int keycode, int value);
    bool emitSyn();

private:
    int fd_ = -1;
};
}

#endif
