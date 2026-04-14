#ifndef STATE_H
#define STATE_H


#include <cstdint>
#include <string>


namespace phantomboard::ctl
{
struct ViewState
{
    std::string mode = "Normal";
    std::string committed;
    std::string preedit;
    std::uint32_t cursor = 0;
};
}

#endif
