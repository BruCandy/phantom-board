#ifndef BUFFER_MANAGER_H
#define BUFFER_MANAGER_H


#include <cstddef>
#include <string>

// original
#include "types.hpp"


namespace phantomboard::daemon
{
class BufferManager {
public:
    BufferManager();
    ~BufferManager();

    bool insertChar(char c);
    bool backSpace();
    bool deleteChar();
    bool moveLeft();
    bool moveRight();

    void clear();
    bool empty();

    const std::string& text();
    std::size_t cursor();
    TextBufferView view();

    std::string renderForDisplay();
    std::string consume();

private:
    std::string text_{};
    std::size_t cursor_ = 0;
};
}

#endif
