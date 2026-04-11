#include <cerrno>
#include <iostream>

// linux
#include <linux/input.h>

// original
#include "core/daemon.hpp"


namespace phantomboard::daemon
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

    if (!ime_.initialize("phantomboard")) {
        std::cerr << "Failed to initialize IBus client" << std::endl;
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

        if (!state_.checkPhantom()) {
            if (!output_.throwEvent(event)) {
                std::cerr << "Failed to throw event" << std::endl;
            }
        } else {
            if (input_.checkPassThroughPress(event)) {
                output_.emitPassThroughKey(event.code, 1);
                continue;
            }

            if (input_.checkPassThroughRelease(event)) {
                output_.emitPassThroughKey(event.code, 0);
                continue;
            }
        }
    }
}

void Daemon::stop()
{
    running_ = false;
}

PhantomMode Daemon::mode()
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
        if (state_.checkPhantom()) {
            output_.emitKeyEvent(KEY_LEFTSHIFT, 0);
            output_.emitKeyEvent(KEY_RIGHTSHIFT, 0);
            output_.emitSyn();
        }
        std::cout << std::endl << "Mode: " << (state_.checkPhantom() ? "Phantom" : "Normal") << std::endl;
        break;

    case InputActionType::InsertChar:
        if (state_.checkPhantom() && action.ch.has_value()) {
            if (ime_.checkImeActive()) {
                ime_.sendKey(static_cast<std::uint32_t>(*action.ch), 0, 0);
                updateBufferfromIme();
            } else {
                buffer_.insertChar(*action.ch);
            }
            std::cout << "Buffer: " << buffer_.renderForDisplay() << std::endl;
        }
        break;

    case InputActionType::Backspace:
        if (state_.checkPhantom()) {
            if (ime_.checkImeActive() && buffer_.checkPreedit()) {
                ime_.sendKey(IBUS_KEY_BackSpace, 0, 0);
                updateBufferfromIme();
            } else {
                buffer_.backSpace();
            }
            std::cout << "Buffer: " << buffer_.renderForDisplay() << std::endl;
        }
        break;

    case InputActionType::DeleteChar:
        if (state_.checkPhantom()) {
            buffer_.deleteChar();
            std::cout << "Buffer: " << buffer_.renderForDisplay() << std::endl;
        }
        break;

    case InputActionType::MoveLeft:
        if (state_.checkPhantom()) {
            buffer_.moveLeft();
            std::cout << "Buffer: " << buffer_.renderForDisplay() << std::endl;
        }
        break;

    case InputActionType::MoveRight:
        if (state_.checkPhantom()) {
            buffer_.moveRight();
            std::cout << "Buffer: " << buffer_.renderForDisplay() << std::endl;
        }
        break;

    case InputActionType::EmitBuffer:
        if (state_.checkPhantom()) {
            if (ime_.checkImeActive() && buffer_.checkPreedit()) {
                ime_.sendKey(IBUS_KEY_Return, 0, 0);
                updateBufferfromIme();
                std::cout << "Buffer: " << buffer_.renderForDisplay() << std::endl;
            } else if (!buffer_.empty()) {
                const std::string text = buffer_.consume();
                output_.emitText(text);
                std::cout << "Emit: " << text << std::endl;
            }
        }
        break;

    case InputActionType::ClearBuffer:
        if (state_.checkPhantom()) {
            buffer_.clear();
            std::cout << "Buffer cleared" << std::endl;
        }
        break;

    case InputActionType::Quit:
        stop();
        break;
    }
}

void Daemon::updateBufferfromIme()
{
    buffer_.setPreedit(ime_.preedit(), ime_.preeditVisible());

    const std::string committed = ime_.takeCommittedText();
    if (!committed.empty()) {
        buffer_.insertString(committed);
    }
}
}
