#ifndef TYPES_H
#define TYPES_H


#include <cstddef>
#include <optional>
#include <string>


namespace bunnyboard::daemon
{
enum class BunnyMode {
    Normal,
    Bunny,
};

enum class InputActionType {
    None,
    InsertChar,
    Backspace,
    DeleteChar,
    MoveLeft,
    MoveRight,
    ToggleMode,
    EmitBuffer,
    ClearBuffer,
    Quit,
};

struct InputAction {
    InputActionType type = InputActionType::None;
    std::optional<char> ch{};
};

struct TextBufferView {
    std::string text;
    std::size_t cursor = 0;
};
}

#endif
