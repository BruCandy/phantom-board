#ifndef APP_H
#define APP_H


#include <atomic>
#include <cstdint>
#include <mutex>
#include <string>

//original
#include "state.hpp"


namespace phantomboard::ctl
{
class App
{
public:
    App();
    ~App();

    App(const App&) = delete;
    App& operator=(const App&) = delete;

    static void stop();
    int run();

private:
   static std::string buildBufferView(const std::string& committed,
                                      const std::string& preedit,
                                      std::uint32_t      cursor);

    void render();
    void updateMode(const std::string& mode);
    void updateBuffer(const std::string& committed,
                      const std::string& preedit,
                      std::uint32_t      cursor);

    std::mutex mutex_;
    ViewState state_;
    bool first_render_ = true;

    static std::atomic<bool> running_;
};
}

#endif
