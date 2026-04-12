// original
#include "core/buffer_manager.hpp"


namespace phantomboard::daemon
{
BufferManager::BufferManager()
{
}

BufferManager::~BufferManager()
{
}

void BufferManager::insertChar(char c) 
{
    text_.insert(cursor_, 1, c);
    ++cursor_;
}

void BufferManager::insertString(const std::string& s)
{
    text_.insert(cursor_, s);
    cursor_ += s.size();
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

    if (preedit_visible_) {
        s.insert(cursor_ + 1, " < " + preedit_ + " > ");
    }

    return s;
}

void BufferManager::setPreedit(const std::string& s, bool visible)
{
    preedit_ = s;
    preedit_visible_ = visible;
}

void BufferManager::clearPreedit()
{
    preedit_.clear();
    preedit_visible_ = false;
}

bool BufferManager::checkPreedit()
{
    return preedit_visible_ && !preedit_.empty();
}

std::string BufferManager::consume()
{
    std::string out = text_;
    clear();
    return out;
}
}