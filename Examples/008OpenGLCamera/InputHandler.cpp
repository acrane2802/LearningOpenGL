#include "InputHandler.h"

#include <vector>
#include <glad/gl.h>

void InputHandler::initialize(const bool useJoystick)
{
    windowHeight = SDL_GetWindowSurface(m_window)->h;
    windowWidth = SDL_GetWindowSurface(m_window)->w;

    int joystickID = 0;
    if (useJoystick)
    {
        int joystickNum;
        SDL_GetJoysticks(&joystickNum);

        for (int i = 0; i < joystickNum; ++i)
        {
            SDL_PollEvent(&e);
            std::cout << e.jdevice.which << ": " << SDL_GetJoystickNameForID(e.jdevice.which) << std::endl;
        }

        std::cout << "Enter Joystick ID from List: ";
        std::cin >> joystickID;
    }

    joystick = SDL_OpenJoystick(joystickID);

    numOfAxes = SDL_GetNumJoystickAxes(joystick);
}

bool InputHandler::isKeyPressed(const int keycode) const
{
    return !previousKeyboardState[keycode] && currentKeyboardState[keycode];
}

bool InputHandler::isKeyReleased(const int keycode) const
{
    return previousKeyboardState[keycode] && !currentKeyboardState[keycode];
}

bool InputHandler::isKeyHeld(const int keycode) const
{
    return previousKeyboardState[keycode] && currentKeyboardState[keycode];
}

bool InputHandler::isMouseButtonPressed(const int keycode) const
{
    if (mouseButtonState[keycode] == 1)
    {
        return true;
    }
    return false;
}

bool InputHandler::isMouseButtonReleased(const int keycode) const
{
    if (mouseButtonState[keycode] == -1)
    {
        return true;
    }
    return false;
}

bool InputHandler::isMouseButtonHeld(const int keycode) const
{
    if (mouseButtonHeld[keycode] == true)
    {
        return true;
    }
    return false;
}

float InputHandler::getMouseX() const
{
    return mouseX;
}

float InputHandler::getMouseY() const
{
    return (mouseY * -1.0f);
}

float InputHandler::getMouseScrollWheel() const
{
    return mouseWheel;
}

bool InputHandler::isJoystickButtonPressed(const int keycode)
{
    if (currentJoystickButtonState[keycode] && !previousJoystickButtonState[keycode])
    {
        currentJoystickButtonState[keycode] = true;
        return true;
    }
    return false;
}

bool InputHandler::isJoystickButtonReleased(const int keycode)
{
    if (!currentJoystickButtonState[keycode] && previousJoystickButtonState[keycode])
    {
        currentJoystickButtonState[keycode] = false;
        return true;
    }
    return false;
}

bool InputHandler::isJoystickButtonHeld(const int keycode) const
{
    return currentJoystickButtonState[keycode] && previousJoystickButtonState[keycode];
}

float InputHandler::getAxis(const int axisID) const
{
    if (axisID > numOfAxes)
    {
        std::cout << "Joystick Axis out of Bounds! Returning -1.0\n";
        return -1.0f;
    }
    return SDL_GetJoystickAxis(joystick, axisID);
}

void InputHandler::updateInput(bool& isRunning)
{
    std::memset(mouseButtonState, 0, 10 * sizeof(*mouseButtonState));
    mouseWheel = 0.0f;

    std::memcpy(previousJoystickButtonState, currentJoystickButtonState, sizeof(bool) * 512);

    for (int i = 0; i < 512; ++i)
    {
        currentJoystickButtonState[i] = SDL_GetJoystickButton(joystick, i);
    }

    while (SDL_PollEvent(&e))
    {
        switch (e.type)
        {
            case SDL_EVENT_QUIT:
                isRunning = false;
                break;
            case SDL_EVENT_WINDOW_RESIZED:
                windowHeight = SDL_GetWindowSurface(m_window)->h;
                windowWidth = SDL_GetWindowSurface(m_window)->w;
                glViewport(0, 0, windowWidth, windowHeight);
                break;
            default:
                break;
        }
        events.push_back(e);
    }

    std::memcpy(previousKeyboardState, currentKeyboardState, sizeof(bool) * 512);
    std::memcpy(currentKeyboardState,SDL_GetKeyboardState(nullptr),sizeof(bool) * 512);

    SDL_GetRelativeMouseState(&mouseX, &mouseY);

    handleMouse();
}

void InputHandler::handleMouse()
{
    for (const SDL_Event & event : events)
    {
        mouseIndex = event.button.button;
        switch(event.type)
        {
            case SDL_EVENT_MOUSE_BUTTON_DOWN:
                mouseButtonState[mouseIndex] = 1;
                mouseButtonHeld[mouseIndex] = true;
            break;
            case SDL_EVENT_MOUSE_BUTTON_UP:
                mouseButtonState[mouseIndex] = -1;
                mouseButtonHeld[mouseIndex] = false;
                break;
            case SDL_EVENT_MOUSE_WHEEL:
                mouseWheel += event.wheel.y * mouseWheelScaler;
                break;
            default:
                break;
        }
    }
    events.clear();
}