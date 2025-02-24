#include "InputHandler.h"

void InputHandler::initialize(const bool useJoystick)
{
    // retrieve initial window size
    windowHeight = SDL_GetWindowSurface(m_window)->h;
    windowWidth = SDL_GetWindowSurface(m_window)->w;

    // joystick id default. 0 is SDL's "no controller" id
    int joystickID = 0;
    if (useJoystick)
    {
        // get the number of joysticks and print each joystick and its id
        int joystickNum;
        SDL_GetJoysticks(&joystickNum);

        for (int i = 0; i < joystickNum; ++i)
        {
            SDL_PollEvent(&e);
            std::cout << e.jdevice.which << ": " << SDL_GetJoystickNameForID(e.jdevice.which) << std::endl;
        }

        // poll user for the id to use
        std::cout << "Enter Joystick ID from List: ";
        std::cin >> joystickID;
    }

    // open joystick regardless if one is connected
    joystick = SDL_OpenJoystick(joystickID);

    // get the number of axes on a specific joystick
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
    return previousMouseState != SDL_BUTTON_MASK(keycode) && currentMouseState == SDL_BUTTON_MASK(keycode);
}

bool InputHandler::isMouseButtonReleased(const int keycode) const
{
    return previousMouseState == SDL_BUTTON_MASK(keycode) && currentMouseState != SDL_BUTTON_MASK(keycode);
}

bool InputHandler::isMouseButtonHeld(const int keycode) const
{
    return previousMouseState == SDL_BUTTON_MASK(keycode) && currentMouseState == SDL_BUTTON_MASK(keycode);
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
        // save the button state and return
        currentJoystickButtonState[keycode] = true;
        return true;
    }
    return false;
}

bool InputHandler::isJoystickButtonReleased(const int keycode)
{
    if (!currentJoystickButtonState[keycode] && previousJoystickButtonState[keycode])
    {
        // save the button state and return
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
    // make sure the provided axis is actually valid
    if (axisID > numOfAxes)
    {
        std::cout << "Joystick Axis out of Bounds! Returning -1.0\n";
        return -1.0f;
    }
    // return the float of the axis polled
    return SDL_GetJoystickAxis(joystick, axisID);
}

void InputHandler::updateInput(bool& isRunning)
{
    // reset mouse wheel every frame
    mouseWheel = 0.0f;

    // copy the current button state to the old state
    std::memcpy(previousJoystickButtonState, currentJoystickButtonState, sizeof(bool) * 512);

    // get the new button state for all valid options
    for (int i = 0; i < 512; ++i)
    {
        currentJoystickButtonState[i] = SDL_GetJoystickButton(joystick, i);
    }

    // poll sdl events
    while (SDL_PollEvent(&e))
    {
        switch (e.type)
        {
            // set the program's while loop to begin shutdown if the x is clicked
            case SDL_EVENT_QUIT:
                isRunning = false;
                break;
            // get new window size and reset the gl viewport
            case SDL_EVENT_WINDOW_RESIZED:
                windowHeight = SDL_GetWindowSurface(m_window)->h;
                windowWidth = SDL_GetWindowSurface(m_window)->w;
                glViewport(0, 0, windowWidth, windowHeight);
                break;
            // only the mouse wheel cant seem to be checked except in SDL's event queue
            /* TO-DO: Find a better solution if one exists */
            case SDL_EVENT_MOUSE_WHEEL:
                mouseWheel += e.wheel.y * mouseWheelScaler;
                break;
            default:
                break;
        }
    }

    // copy current state into the previous one then get new state
    std::memcpy(previousKeyboardState, currentKeyboardState, sizeof(bool) * 512);
    std::memcpy(currentKeyboardState,SDL_GetKeyboardState(nullptr),sizeof(bool) * 512);

    // copy current state into the previous one then get new state. no memcpy as it is a simple integer instead of a pointer to array[0] of some size
    previousMouseState = currentMouseState;
    currentMouseState = SDL_GetRelativeMouseState(&mouseX, &mouseY);
}