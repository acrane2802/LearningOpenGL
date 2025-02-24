#ifndef INPUTHANDLER_H
#define INPUTHANDLER_H

#include <SDL3/SDL.h>
#include <iostream>
#include <vector>
#include <glad/gl.h>

class InputHandler {

public:
    // constructor to initialize all basic data
    explicit InputHandler(SDL_Window* window, bool useJoystick) : e(), m_window(window), m_useJoystick(useJoystick)
    {
        initialize(m_useJoystick);
    };

    // deconstructor to close the joystick
    ~InputHandler()
    {
        SDL_CloseJoystick(joystick);
    };

    // functions for the program to use to check input state
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
    // private initialization that is called in the constructor
    void initialize(bool useJoystick);

    // the empty event to poll and an array to not miss inputs
    SDL_Event e;
    std::vector<SDL_Event> events;

    // window data
    int windowWidth = 0;
    int windowHeight = 0;
    SDL_Window* m_window = nullptr;

    // keyboard states to check all key values
    bool* previousKeyboardState = static_cast<bool*>(malloc(sizeof(bool) * 512));
    bool* currentKeyboardState = static_cast<bool*>(malloc(sizeof(bool) * 512));

    // refer to the keyboard state. this is a button mask that sdl provides
    uint32_t previousMouseState = 0;
    uint32_t currentMouseState = 0;

    // private mouse coordinates
    float mouseX = 0;
    float mouseY = 0;

    // the mouse wheel value and the value by which to scale its reading
    float mouseWheel = 0.0f;
    float mouseWheelScaler = 5.0f;

    // joystick bool to do different stuff at runtime and the joystick pointer
    bool m_useJoystick = false;
    SDL_Joystick* joystick = nullptr;

    // all states of the joystick buttons for a specific gamepad
    bool previousJoystickButtonState[512] = {false};
    bool currentJoystickButtonState[512] = {false};

    // the number of axes, for bounds checking
    int numOfAxes = 0;
};

#endif
