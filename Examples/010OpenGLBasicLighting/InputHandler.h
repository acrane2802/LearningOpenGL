#ifndef INPUTHANDLER_H
#define INPUTHANDLER_H

#include <SDL3/SDL.h>
#include <iostream>
#include <vector>
#include <cstring>
#include <glad/gl.h>
#include <imgui_impl_sdl3.h>

#include "Scancodes.h"

class InputHandler {

public:
    typedef enum ControlDevice
    {
        Controller = 0,
        Keyboard = 1,
        Mouse = 2,
        TrueAxis = 3
    } ControlDevice;

    // constructor to initialize all basic data
    explicit InputHandler(SDL_Window* window) : e(), m_window(window)
    {
        windowHeight = SDL_GetWindowSurface(m_window)->h;
        windowWidth = SDL_GetWindowSurface(m_window)->w;
    };

    // destructor to delete all reserved states and gamepad
    ~InputHandler();

    void askUserIfUsingGamepad();

    // functions for the program to use to check input state
    [[nodiscard]] bool isKeyPressed(Key_Scancode key) const;
    [[nodiscard]] bool isKeyReleased(Key_Scancode key) const;
    [[nodiscard]] bool isKeyHeld(Key_Scancode key) const;

    [[nodiscard]] bool isMouseButtonPressed(Mouse_Scancode button) const;
    [[nodiscard]] bool isMouseButtonReleased(Mouse_Scancode button) const;
    [[nodiscard]] bool isMouseButtonHeld(Mouse_Scancode button) const;

    [[nodiscard]] float getMouseX() const;
    [[nodiscard]] float getMouseY() const;

    [[nodiscard]] float getMouseScrollWheel() const;

    [[nodiscard]] bool isGamepadButtonPressed(ControllerButton button) const;
    [[nodiscard]] bool isGamepadButtonReleased(ControllerButton button) const;
    [[nodiscard]] bool isGamepadButtonHeld(ControllerButton button) const;

    [[nodiscard]] float getAxis(ControllerAxis axis) const;
    void setDeadzone(const float value) { deadzonePercentage = value; }

    [[nodiscard]] int getLastUsedDevice() const { return lastInputDevice; }

    void updateInput(bool& isRunning);

    void setUsingGamepad(bool value) { m_useGamepad = value; }
    [[nodiscard]] bool usingGamepad() const { return m_useGamepad; }

private:
    // the empty event to poll and an array to not miss inputs
    SDL_Event e;
    std::vector<SDL_Event> events;

    int device = 1;

    // window data
    int windowWidth = 0;
    int windowHeight = 0;
    SDL_Window* m_window = nullptr;

    // keyboard states to check all key values
    bool* previousKeyboardState = static_cast<bool*>(operator new(sizeof(bool) * 512));
    bool* currentKeyboardState = static_cast<bool*>(operator new(sizeof(bool) * 512));

    ControlDevice lastInputDevice = Keyboard;

    // refer to the keyboard state. this is a button mask that sdl provides
    uint32_t previousMouseState = 0;
    uint32_t currentMouseState = 0;

    // private mouse coordinates
    float mouseX = 0;
    float mouseY = 0;

    // the mouse wheel value and the value by which to scale its reading
    float mouseWheel = 0.0f;
    float mouseWheelScaler = 5.0f;

    // gamepad bool to do different stuff at runtime and the gamepad pointer
    float deadzonePercentage = 0.15f;
    bool m_useGamepad = false;
    SDL_Gamepad* gamepad = nullptr;
    bool gamepadState = false;

    // the number of axes, for bounds checking
    int gamepadButtonCount = 28;
    int gamepadAxisCount = 8;

    // all states of the gamepad buttons for a specific gamepad
    bool* previousGamepadButtonState = static_cast<bool*>(operator new(sizeof(bool) * gamepadButtonCount));
    bool* currentGamepadButtonState = static_cast<bool*>(operator new(sizeof(bool) * gamepadButtonCount));
};

#endif
