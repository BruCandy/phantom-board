// original
#include "buffer_manager.hpp"


namespace phantomboard::daemon
{
BufferManager::BufferManager()
{
}

BufferManager::~BufferManager()
{
}

bool BufferManager::insertChar(char c) 
{
    text_.insert(cursor_, 1, c);
    ++cursor_;
    return true;
}

bool BufferManager::backSpace() 
{
    if (cursor_ == 0) {
        return false;
    }

    text_.erase(cursor_ - 1, 1);
    --cursor_;
    return true;
}

bool BufferManager::deleteChar() 
{
    if (cursor_ >= text_.size()) {
        return false;
    }

    text_.erase(cursor_, 1);
    return true;
}

bool BufferManager::moveLeft() 
{
    if (cursor_ == 0) {
        return false;
    }

    --cursor_;
    return true;
}

bool BufferManager::moveRight() 
{
    if (cursor_ >= text_.size()) {
        return false;
    }

    ++cursor_;
    return true;
}

void BufferManager::clear()
{
    text_.clear();
    cursor_ = 0;
}

bool BufferManager::empty() 
{
    return text_.empty();
}

const std::string& BufferManager::text() 
{
    return text_;
}

std::size_t BufferManager::cursor() 
{
    return cursor_;
}

TextBufferView BufferManager::view()
{
    return TextBufferView{ text_, cursor_ };
}

std::string BufferManager::renderForDisplay()
{
    std::string s = text_;
    s.insert(cursor_, "|");
    return s;
}

std::string BufferManager::consume()
{
    std::string out = text_;
    clear();
    return out;
}
}