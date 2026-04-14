#include <csignal>

//original
#include "app.hpp"


static void signalHandler(const int signum)
{
    phantomboard::ctl::App::stop();
}

int main()
{
    std::signal(SIGINT,  signalHandler);
    std::signal(SIGTERM, signalHandler);

    phantomboard::ctl::App app;
    return app.run();
}
