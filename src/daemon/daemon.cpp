#include <iostream>

// original
#include "daemon.hpp"


namespace bunnyboard::daemon
{
Daemon::Daemon(std::string input_device_path)
    : input_device_path_(std::move(input_device_path)) 
{
}

Daemon::~Daemon()
{
}

bool Daemon::initialize()
{
    if (!input_.openDevice(input_device_path_)) {
        std::cerr << "Failed to open input device: " << input_device_path_ << std::endl;
        return false;
    }

    if (!input_.grab()) {
        std::cerr << "Failed to grab input device" << std::endl;
        return false;
    }

    if (!output_.initialize()) {
        std::cerr << "Failed to initialize uinput output device" << std::endl;
        return false;
    }

    running_ = true;
    return true;
}

void Daemon::run()
{
    while (running_) {
        struct input_event event {};
        if (!input_.readEvent(event)) {
            std::cerr << "Failed to read input event" << std::endl;
            break;
        }

        const InputAction action = input_.translateEvent(event);
        handleAction(action);
    }
}

void Daemon::stop()
{
    running_ = false;
}

BunnyMode Daemon::mode()
{
    return state_.getMode();
}

TextBufferView Daemon::bufferView()
{
    return buffer_.view();
}

void Daemon::handleAction(const InputAction& action)
{
    switch (action.type) {
    case InputActionType::None:
        break;

    case InputActionType::ToggleMode:
        state_.toggleMode();
        std::cout << "Mode: " << (state_.checkBunny() ? "Bunny" : "Normal") << std::endl;
        break;

    case InputActionType::InsertChar:
        if (state_.checkBunny() && action.ch.has_value()) {
            buffer_.insertChar(*action.ch);
            std::cout << "Buffer: " << buffer_.renderForDisplay() << std::endl;
        }
        break;

    case InputActionType::Backspace:
        if (state_.checkBunny()) {
            buffer_.backSpace();
            std::cout << "Buffer: " << buffer_.renderForDisplay() << std::endl;
        }
        break;

    case InputActionType::DeleteChar:
        if (state_.checkBunny()) {
            buffer_.deleteChar();
            std::cout << "Buffer: " << buffer_.renderForDisplay() << std::endl;
        }
        break;

    case InputActionType::MoveLeft:
        if (state_.checkBunny()) {
            buffer_.moveLeft();
            std::cout << "Buffer: " << buffer_.renderForDisplay() << std::endl;
        }
        break;

    case InputActionType::MoveRight:
        if (state_.checkBunny()) {
            buffer_.moveRight();
            std::cout << "Buffer: " << buffer_.renderForDisplay() << std::endl;
        }
        break;

    case InputActionType::EmitBuffer:
        if (state_.checkBunny() && !buffer_.empty()) {
            const std::string text = buffer_.consume();
            output_.emitText(text);
            std::cout << "Emit: " << text << std::endl;
        }
        break;

    case InputActionType::ClearBuffer:
        if (state_.checkBunny()) {
            buffer_.clear();
            std::cout << "Buffer cleared" << std::endl;
        }
        break;

    case InputActionType::Quit:
        stop();
        break;
    }
}
}
