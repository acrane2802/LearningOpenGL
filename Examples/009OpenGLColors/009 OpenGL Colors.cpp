#include <iostream>
#include <sstream>
#include <fstream>
#include <glad/gl.h>
#include <SDL3/SDL.h>
#define STB_IMAGE_IMPLEMENTATION
#include <chrono>
#include <stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"
#include "InputHandler.h"
#include "Camera.h"

// constants for window size at the beginning of the program
#define WINDOW_HEIGHT 600
#define WINDOW_WIDTH 800

#define UPDATE_TIME_IN_FPS 60.0f

// useful functions that interface with SDL
void framebufferCallback(int width, int height);

// arguments in main are required so SDL_main doesn't cause compilation issues
int main(int argc, char* args[])
{
    // beginning variables for both naming the window and the only variable that should be changed if the program needs to be shut down.
    const std::string title = "009 OpenGL Colors";
    bool isRunning = false;

    // set up SDL to begin its video subsystems and set the opengl attributes to avoid this program running on unsupported hardware
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMEPAD);

    SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, 3 );
    SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, 3 );
    SDL_GL_SetAttribute( SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE );

    // create the window pointer, beginning around the middle of the screen with the dimension constants and the opengl flag
    SDL_Window* window = SDL_CreateWindow(title.c_str(), WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_OPENGL);

    // this could be commented out at a later date. it is here to test the function framebuffer_callback
    SDL_SetWindowResizable(window, true);

    // make sure window exists
    if(window == nullptr)
    {
        std::cout << "Failed to create window" << std::endl;
        SDL_Quit();
        return EXIT_FAILURE;
    }

    // hand the opengl context over to SDL
    SDL_GLContext glContext = SDL_GL_CreateContext(window);
    SDL_GL_MakeCurrent(window, glContext);

    // make sure all OpenGL extensions can be accessed, otherwise, close
    // this dangerous cast is to stay within C++ standards, but should be changed if undefined behavior occurs
    int version = gladLoadGL(reinterpret_cast<GLADloadfunc>(SDL_GL_GetProcAddress));
    if(version == 0)
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return EXIT_FAILURE;
    }

    // set viewport to the window and set the running variable to true
    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
    isRunning = true;

    // loads the shader and compiles a program
    Shader cubeShader("assets/shaders/colors_vertex_shader.glsl", "assets/shaders/colors_fragment_shader.glsl");
    Shader lightShader("assets/shaders/colors_light_vertex_shader.glsl", "assets/shaders/colors_light_fragment_shader.glsl");

    float vertices[] = {
        -0.5f, -0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f,  0.5f, -0.5f,
         0.5f,  0.5f, -0.5f,
        -0.5f,  0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,

        -0.5f, -0.5f,  0.5f,
         0.5f, -0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,
        -0.5f, -0.5f,  0.5f,

        -0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,

         0.5f,  0.5f,  0.5f,
         0.5f,  0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f, -0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,

        -0.5f, -0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f, -0.5f,  0.5f,
         0.5f, -0.5f,  0.5f,
        -0.5f, -0.5f,  0.5f,
        -0.5f, -0.5f, -0.5f,

        -0.5f,  0.5f, -0.5f,
         0.5f,  0.5f, -0.5f,
         0.5f,  0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f, -0.5f
    };

    glm::vec3 lightPosition(2.0f, 1.5f, -1.5f);

    // VAOs or Vertex Array Objects store a given VBO and their necessary Vertex Attributes. This makes it so you don't have to run the VBO and VA every frame and can simply call the VAO. Modern OpenGL *Requires* this to draw.
    unsigned int cubeVAO;
    glGenVertexArrays(1, &cubeVAO);
    glBindVertexArray(cubeVAO);

    // this is a vertex buffer object. it is generated using an int and an id. then it is bound to its buffer type
    unsigned int cubeVBO;
    glGenBuffers(1, &cubeVBO);
    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);

    // this function takes in the buffer type that is currently bound, the size of the data in bytes, the pointer to the first data of the array, and the way the data is used. GL_STATIC_DRAW is read once, use everywhere
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // this tells opengl how the vertices are laid out in memory. the first is the array we want to configure. this is the location variable set in the vertex shader
    // the second attribute tells us the size of the vertex vector. since a vertex here is defined by xyz, it is a vec3
    // the third argument defines the vertex coordinate datatype
    // the next specifies whether the data should be normalized (-1, 0, 1). since the data is in floats, we don't want it to be
    // the next argument is the stride. it defines how far in memory to move to find the next x coordinate. in this configuration, we move 5 floats due to the vertex array. in a different declaration, this needs to be changed
    // and the final portion is a weird data offset. this has been set to nullptr here as a 0 literal but in other instances, a reinterpret_cast to void* would be *technically* correct but ugly. This is the OpenGL API problem
    // after the vertex behavior is defined, we enable the first group (location 0)

    // which vbo this uses is determined by whichever VBO is bound to the context, if a new vbo is laid out differently, we need to run this again
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);

    unsigned int lightVAO;
    glGenVertexArrays(1, &lightVAO);
    glBindVertexArray(lightVAO);

    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);

    // enable depth buffer to avoid z-fighting
    glEnable(GL_DEPTH_TEST);

    // create our input handler object using the custom class
    InputHandler input(window);
    // input.askUserIfUsingGamepad();

    // create our camera and set necessary data
    Camera camera;
    constexpr float cameraSpeed = 7.5f;
    camera.setCameraSpeed(cameraSpeed);

    float xAxis;
    float yAxis;
    float zAxis;
    float LookAtX;
    float LookAtY;

    // create a bool to toggle size
    bool isMaximized = false;

    // get all the data necessary to have a fixed update loop and a non-fixed update loop
    std::chrono::duration<double, std::milli> fixedUpdateTime = std::chrono::duration<double>(1.0 / UPDATE_TIME_IN_FPS);
    std::chrono::time_point previousTime = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> timeSinceLastFrame(0);
    double deltaTime = 0.0;

    // tell SDL to capture our mouse and report motion even at the window edges
    SDL_SetWindowRelativeMouseMode(window, true);

    camera.setMouseSensitivity(300.0f);

    // while(running) loop is for all rendering and OpenGL code
    while(isRunning)
    {
        // get the time in nanoseconds and reset the initial time counter, then get the time since last frame in milliseconds
        std::chrono::duration<double> deltaTimeCast = (std::chrono::high_resolution_clock::now() - previousTime);
        previousTime = std::chrono::high_resolution_clock::now();
        timeSinceLastFrame += std::chrono::duration_cast<std::chrono::milliseconds>(deltaTimeCast);

        deltaTime = deltaTimeCast.count();

        // continuously run our input handler
        input.updateInput(isRunning);

        if (input.usingGamepad())
        {
            xAxis = input.getAxis(CONTROLLER_AXIS_LEFTX);
            yAxis = input.getAxis(CONTROLLER_AXIS_LEFTY) * -1.0f;
            zAxis = input.getAxis(CONTROLLER_AXIS_LEFT_TRIGGER) - input.getAxis(CONTROLLER_AXIS_RIGHT_TRIGGER);
            LookAtX = input.getAxis(CONTROLLER_AXIS_RIGHTX);
            LookAtY = input.getAxis(CONTROLLER_AXIS_RIGHTY) * -1.0f;
        } else
        {
            xAxis = static_cast<float>(input.isKeyHeld(KEY_D) - input.isKeyHeld(KEY_A));
            yAxis = static_cast<float>(input.isKeyHeld(KEY_W) - input.isKeyHeld(KEY_S));
            zAxis = static_cast<float>(input.isKeyHeld(KEY_SPACE) - input.isKeyHeld(KEY_LCTRL));
            LookAtX = input.getMouseX();
            LookAtY = input.getMouseY();
        }

        camera.setTransformation(CAMERA_AXIS_X, xAxis);
        camera.setTransformation(CAMERA_AXIS_Y, yAxis);
        camera.setTransformation(CAMERA_AXIS_Z, zAxis);
        camera.setTransformation(CAMERA_LOOK_AXIS_X, LookAtX);
        camera.setTransformation(CAMERA_LOOK_AXIS_Y, LookAtY);

        if (input.usingGamepad())
        {
            if (input.isGamepadButtonPressed(CONTROLLER_BUTTON_SOUTH))
            {
                std::cout << "South Button Pressed!" << '\n';
            }
        }

        if (input.getLastUsedDevice() == 1 || input.getLastUsedDevice() == 2)
        {
            input.setUsingGamepad(false);
        }
        if (input.getLastUsedDevice() == 0 || input.getLastUsedDevice() == 3)
        {
            input.setUsingGamepad(true);
        }

        camera.setFieldOfView(camera.getFieldOfView() - input.getMouseScrollWheel());

        camera.update(deltaTime);

        // check if escape is pressed. if it is close
        if (input.isKeyPressed(KEY_ESCAPE) || input.isGamepadButtonPressed(CONTROLLER_BUTTON_START))
        {
            std::cout << "Escape is Pressed!\nShutting Down...\n";
            isRunning = false;
        }

        // swap between maximized and original dimensions using the bool defined earlier
        if (input.isGamepadButtonPressed(CONTROLLER_BUTTON_BACK) || ((input.isKeyHeld(KEY_LALT) && input.isKeyPressed(KEY_RETURN)) || (input.isKeyHeld(KEY_RETURN) && input.isKeyPressed(KEY_LALT))))
        {
            isMaximized = !isMaximized;
            if (isMaximized)
            {
                SDL_MaximizeWindow(window);
            }
            else
            {
                SDL_RestoreWindow(window);
            }
        }

        // here is our fixed update loop. if the time since last frame took longer than the fixed update rate, then we subtract the fixed update time until it is satisfactory
        while (timeSinceLastFrame >= fixedUpdateTime)
        {
            // fixed update
            timeSinceLastFrame -= fixedUpdateTime;
        }

        // here are the openGL commands
        // draw the background color then clear the screen every frame
        // we now also clear the depth buffer
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // use the shader program
        cubeShader.use();
        cubeShader.setVec3("objectColor", 1.0f, 0.5f, 0.31f);
        cubeShader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);

        // this defines a fov in the first argument, the viewport's width / height, and the near and far plane distance from the camera
        glm::mat4 projectionMatrix = glm::perspective(glm::radians(camera.getFieldOfView()), static_cast<float>(SDL_GetWindowSurface(window)->w) / static_cast<float>(SDL_GetWindowSurface(window)->h), 0.1f, 100.0f);

        // refer to the model matrix on how this works
        cubeShader.setMat4("viewMatrix", camera.getViewMatrix());
        cubeShader.setMat4("projectionMatrix", projectionMatrix);

        // here we create the translation matrix and translate it appropriately
        // we are forced to reset the matrix every frame
        glm::vec3 cubeTranslation(0.5f, 0.0f, 0.0f);
        auto cubeTranslationMatrix = glm::mat4(1.0f);
        cubeTranslationMatrix = glm::translate(cubeTranslationMatrix, cubeTranslation);

        // rotation has to occur after translation, otherwise the rotation point is not adequately translated shifting the origin
        // we use a quaternion to avoid gimbal lock and also normalize that quaternion to remove vertex stretching
        float cubeRotation = glm::radians(45.0f);
        glm::vec3 cubeRotationAxis = glm::vec3(0.33f, 1.0f, 0.0f);
        glm::quat cubeRotationQuaternion;
        cubeRotationQuaternion = glm::normalize(glm::angleAxis(cubeRotation, cubeRotationAxis));
        glm::mat4 cubeRotationMatrix = glm::mat4_cast(cubeRotationQuaternion);

        // here we create a scale matrix. it must first be made with an identity so when we scale it using the scale variable, it doesn't end up 0
        glm::vec3 cubeScale(1.0f, 1.0f, 1.0f);
        auto cubeScaleMatrix = glm::mat4(1.0f);
        cubeScaleMatrix = glm::scale(cubeScaleMatrix, cubeScale);

        // here we create the final matrix to apply all our actions
        glm::mat4 cubeModelMatrix = cubeTranslationMatrix * cubeRotationMatrix * cubeScaleMatrix;

        // we pass the transform location in shader memory, how many matrices to send, whether to transpose (swap columns and rows), and the actual matrix. glm may not store it in
        // an opengl compatible way so we use glm::value_ptr
        cubeShader.setMat4("modelMatrix", cubeModelMatrix);

        // bind the VAO with references to the VBO, EBO, and vertex attributes
        glBindVertexArray(cubeVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        lightShader.use();
        lightShader.setMat4("viewMatrix", camera.getViewMatrix());
        lightShader.setMat4("projectionMatrix", projectionMatrix);

        glm::vec3 lightTranslation = lightPosition;
        auto lightTranslationMatrix = glm::mat4(1.0f);
        lightTranslationMatrix = glm::translate(lightTranslationMatrix, lightTranslation);

        // rotation has to occur after translation, otherwise the rotation point is not adequately translated shifting the origin
        // we use a quaternion to avoid gimbal lock and also normalize that quaternion to remove vertex stretching
        float lightRotation = glm::radians(0.0f);
        glm::vec3 lightRotationAxis = glm::vec3(0.0f, 0.0f, 0.0f);
        glm::quat lightRotationQuaternion;
        lightRotationQuaternion = glm::normalize(glm::angleAxis(lightRotation, lightRotationAxis));
        glm::mat4 lightRotationMatrix = glm::mat4_cast(lightRotationQuaternion);

        // here we create a scale matrix. it must first be made with an identity so when we scale it using the scale variable, it doesn't end up 0
        glm::vec3 lightScale(0.5f, 0.5f, 0.5f);
        auto lightScaleMatrix = glm::mat4(1.0f);
        lightScaleMatrix = glm::scale(lightScaleMatrix, lightScale);

        // here we create the final matrix to apply all our actions
        glm::mat4 lightModelMatrix = lightTranslationMatrix * lightRotationMatrix * lightScaleMatrix;

        lightShader.setMat4("modelMatrix", lightModelMatrix);

        glBindVertexArray(lightVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // disable vsync
        // swap the SDL front and back buffers
        SDL_GL_SetSwapInterval(0);
        SDL_GL_SwapWindow(window);
    }

    // here we clear all the data we are using
    glDeleteVertexArrays(1, &cubeVAO);
    glDeleteVertexArrays(1, &lightVAO);
    glDeleteBuffers(1, &cubeVBO);

    // we destroy all sdl resources
    SDL_DestroyWindow(window);
    SDL_GL_DestroyContext(glContext);
    SDL_Quit();

    return 0;
}