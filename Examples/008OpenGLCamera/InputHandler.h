#ifndef INPUTHANDLER_H
#define INPUTHANDLER_H

#include <SDL3/SDL.h>
#include <iostream>
#include <vector>

#include "glm/ext/scalar_uint_sized.hpp"

class InputHandler {

public:
    explicit InputHandler(SDL_Window* window, bool useJoystick) : e(), m_window(window), m_useJoystick(useJoystick)
    {
        initialize(m_useJoystick);
    };

    ~InputHandler()
    {
        SDL_CloseJoystick(joystick);
    };

    bool isKeyPressed(int keycode) const;
    bool isKeyReleased(int keycode) const;
    bool isKeyHeld(int keycode) const;

    bool isMouseButtonPressed(int keycode) const;
    bool isMouseButtonReleased(int keycode) const;
    bool isMouseButtonHeld(int keycode) const;

    float getMouseX() const;
    float getMouseY() const;

    float getMouseScrollWheel() const;

    bool isJoystickButtonPressed(int keycode);
    bool isJoystickButtonReleased(int keycode);
    bool isJoystickButtonHeld(int keycode) const;

    float getAxis(int axisID) const;

    void updateInput(bool& isRunning);

private:
    void initialize(bool useJoystick);

    SDL_Event e;
    std::vector<SDL_Event> events;

    int windowWidth = 0;
    int windowHeight = 0;
    SDL_Window* m_window = nullptr;

    bool* previousKeyboardState = static_cast<bool*>(malloc(sizeof(bool) * 512));
    bool* currentKeyboardState = static_cast<bool*>(malloc(sizeof(bool) * 512));

    uint32_t previousMouseState = 0;
    uint32_t currentMouseState = 0;

    float mouseX = 0;
    float mouseY = 0;

    float mouseWheel = 0.0f;
    float mouseWheelScaler = 5.0f;

    bool m_useJoystick = false;
    SDL_Joystick* joystick = nullptr;

    bool previousJoystickButtonState[512] = {false};
    bool currentJoystickButtonState[512] = {false};

    int numOfAxes = 0;
};

#endif
