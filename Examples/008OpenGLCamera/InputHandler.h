#ifndef INPUTHANDLER_H
#define INPUTHANDLER_H

#include <SDL3/SDL.h>
#include <iostream>
#include <vector>

class InputHandler {

public:
    explicit InputHandler(SDL_Window* window) : e(), m_window(window) {};

    ~InputHandler()
    {
        SDL_CloseJoystick(joystick);
    };

    void initialize(bool useJoystick);

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
    void handleMouse();

    SDL_Event e;
    std::vector<SDL_Event> events;

    int windowWidth = 0;
    int windowHeight = 0;
    SDL_Window* m_window = nullptr;

    bool* previousKeyboardState = static_cast<bool*>(malloc(sizeof(bool) * 512));
    bool* currentKeyboardState = static_cast<bool*>(malloc(sizeof(bool) * 512));

    int mouseButtonState[10] = {0};
    bool mouseButtonHeld[10] = {false};

    int mouseIndex = 0;

    float mouseX = 0;
    float mouseY = 0;

    float mouseWheel = 0.0f;
    float mouseWheelScaler = 5.0f;

    SDL_Joystick* joystick = nullptr;

    bool previousJoystickButtonState[512] = {false};
    bool currentJoystickButtonState[512] = {false};

    int numOfAxes = 0;
};

#endif
