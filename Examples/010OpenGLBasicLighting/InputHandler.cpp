#include "InputHandler.h"

InputHandler::~InputHandler()
{
    delete previousKeyboardState;
    delete currentKeyboardState;
    delete previousGamepadButtonState;
    delete currentGamepadButtonState;

    SDL_CloseGamepad(gamepad);
}

void InputHandler::askUserIfUsingGamepad()
{
    // gamepad id default. 0 is SDL's "no controller" id
    char answer;
    std::cout << "Use Gamepad? (Y / N) ";
    std::cin >> answer;

    bool pollUser = true;
    while (pollUser)
    {
        switch (answer)
        {
            case 'Y':
                m_useGamepad = true;
            pollUser = false;
            break;
            case 'N':
                m_useGamepad = false;
            pollUser = false;
            break;
            default:
                std::cout << "Please retry your answer: ";
            break;
        }
    }

    if (m_useGamepad)
    {
        int gamepadID = 0;
        // get the number of gamepads and print each gamepad and its id
        int gamepadNum;
        SDL_GetGamepads(&gamepadNum);

        for (int i = 0; i < gamepadNum; ++i)
        {
            SDL_PollEvent(&e);
            std::cout << e.jdevice.which << ": " << SDL_GetGamepadNameForID(e.jdevice.which) << std::endl;
        }

        // poll user for the id to use
        std::cout << "Enter Joystick ID from List: ";
        std::cin >> gamepadID;

        // open joystick regardless if one is connected
        gamepad = SDL_OpenGamepad(gamepadID);
    }
}

bool InputHandler::isKeyPressed(const Key_Scancode key) const
{
    return !previousKeyboardState[key] && currentKeyboardState[key];
}

bool InputHandler::isKeyReleased(const Key_Scancode key) const
{
    return previousKeyboardState[key] && !currentKeyboardState[key];
}

bool InputHandler::isKeyHeld(const Key_Scancode key) const
{
    return previousKeyboardState[key] && currentKeyboardState[key];
}

bool InputHandler::isMouseButtonPressed(Mouse_Scancode button) const
{
    return previousMouseState != SDL_BUTTON_MASK(button) && currentMouseState == SDL_BUTTON_MASK(button);
}

bool InputHandler::isMouseButtonReleased(Mouse_Scancode button) const
{
    return previousMouseState == SDL_BUTTON_MASK(button) && currentMouseState != SDL_BUTTON_MASK(button);
}

bool InputHandler::isMouseButtonHeld(Mouse_Scancode button) const
{
    return previousMouseState == SDL_BUTTON_MASK(button) && currentMouseState == SDL_BUTTON_MASK(button);
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

bool InputHandler::isGamepadButtonPressed(ControllerButton button) const
{
    if (currentGamepadButtonState[button] && !previousGamepadButtonState[button])
    {
        // save the button state and return
        currentGamepadButtonState[button] = true;
        return true;
    }
    return false;
}

bool InputHandler::isGamepadButtonReleased(ControllerButton button) const
{
    if (!currentGamepadButtonState[button] && previousGamepadButtonState[button])
    {
        // save the button state and return
        currentGamepadButtonState[button] = false;
        return true;
    }
    return false;
}

bool InputHandler::isGamepadButtonHeld(ControllerButton button) const
{
    return currentGamepadButtonState[button] && previousGamepadButtonState[button];
}

float InputHandler::getAxis(ControllerAxis axis) const
{
    // make sure the provided axis is actually valid
    if (axis > gamepadAxisCount)
    {
        std::cout << "Joystick Axis out of Bounds! Returning -1.0\n";
        return -1.0f;
    }

    float axisValue = SDL_GetGamepadAxis(gamepad, static_cast<SDL_GamepadAxis>(axis));

    float joystickMinimum = -32767;
    float joystickMaximum = 32767;
    float deadzoneMinimum = joystickMinimum * deadzonePercentage;
    float deadzoneMaximum = joystickMaximum * deadzonePercentage;

    if (axisValue <= (deadzoneMaximum) && axisValue >= (deadzoneMinimum))
    {
        return 0.0f;
    }

    axisValue /= joystickMaximum;

    return axisValue;
}

void InputHandler::updateInput(bool& isRunning)
{
    // reset mouse wheel every frame
    mouseWheel = 0.0f;

    // copy the current button state to the old state
    std::copy_n(currentGamepadButtonState, gamepadButtonCount, previousGamepadButtonState);

    // get the new button state for all valid options
    for (int i = 0; i < gamepadButtonCount; ++i)
    {
        currentGamepadButtonState[i] = SDL_GetGamepadButton(gamepad, static_cast<SDL_GamepadButton>(i));
    }

    // poll sdl events
    while (SDL_PollEvent(&e))
    {
        ImGui_ImplSDL3_ProcessEvent(&e);

        switch (e.type)
        {
            // set the program's while loop to begin shutdown if the x is clicked
            case SDL_EVENT_QUIT:
                isRunning = false;
                break;
            case SDL_EVENT_KEY_DOWN:
                lastInputDevice = Keyboard;
                break;
            case SDL_EVENT_MOUSE_BUTTON_DOWN:
                lastInputDevice = Mouse;
                break;
            case SDL_EVENT_MOUSE_MOTION:
                lastInputDevice = Mouse;
                break;
            case SDL_EVENT_GAMEPAD_AXIS_MOTION:
                lastInputDevice = TrueAxis;
                break;
            case SDL_EVENT_GAMEPAD_BUTTON_DOWN:
                lastInputDevice = Controller;
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
                lastInputDevice = Mouse;
                mouseWheel += e.wheel.y * mouseWheelScaler;
                break;
            default:
                break;
        }
    }

    // copy current state into the previous one then get new state
    std::copy_n(currentKeyboardState, 512, previousKeyboardState);
    std::copy_n(SDL_GetKeyboardState(nullptr), 512, currentKeyboardState);

    // copy current state into the previous one then get new state. no std::copy_n as it is a simple integer instead of a pointer to array[0] of some size
    previousMouseState = currentMouseState;
    currentMouseState = SDL_GetRelativeMouseState(&mouseX, &mouseY);
}
