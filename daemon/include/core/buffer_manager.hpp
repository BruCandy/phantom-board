#ifndef BUFFER_MANAGER_H
#define BUFFER_MANAGER_H


#include <cstddef>
#include <string>

// original
#include "core/types.hpp"


namespace phantomboard::daemon
{
class BufferManager {
public:
    BufferManager();
    ~BufferManager();

    void insertChar(char c);
    void insertString(const std::string& s);
    bool backSpace();
    bool deleteChar();
    bool moveLeft();
    bool moveRight();

    void setPreedit(const std::string& s, bool visible);
    void clearPreedit();
    bool checkPreedit();

    void clear();
    bool empty();

    const std::string& committed();
    const std::string& preedit();
    std::size_t cursor();

    std::string renderForDisplay();
    std::string consume();

private:
    std::string committed_{};
    std::string preedit_{};
    std::size_t cursor_ = 0;
    bool preedit_visible_ = false;
};
}

#endif
