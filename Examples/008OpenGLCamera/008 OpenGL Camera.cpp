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
    const std::string title = "008 OpenGL Camera";
    bool isRunning = false;

    // set up SDL to begin its video subsystems and set the opengl attributes to avoid this program running on unsupported hardware
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK);

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
    Shader shader("assets/shaders/coordinates_vertex_shader.glsl", "assets/shaders/coordinates_fragment_shader.glsl");

    // make sure the images are loaded in the right orientation
    stbi_set_flip_vertically_on_load(true);

    // this reads in a texture using stb_image and stores the data in these variables, with the last data being the desired channels
    int width, height, nrChannels;
    unsigned char* imageData = stbi_load("assets/textures/tile_01_diffuse.jpg", &width, &height, &nrChannels, 0);

    // here we generate a texture and bind it to the context
    unsigned int texture1;
    glGenTextures(1, &texture1);
    glBindTexture(GL_TEXTURE_2D, texture1);

    // this sets the texture parameters. s and t are equivalent to x and y
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // this loads the image data and into an opengl-readable texture object after checking the data is valid
    // the first argument defines what bound data we're editing (GL_TEXTURE_1D and 3D unaffected here)
    // the next argument defines tells opengl the mipmap level to use
    // the third argument defines what colorspace to store the texture in
    // the fourth and fifth argument tell opengl the width and height respectively of our image
    // the next should ALWAYS be 0 due to legacy code
    // the next two specify the colorspace of the loaded image and the datatype of the image
    // the final points to the data itself
    // then the mipmaps are generated
    if (imageData)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, imageData);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else
    {
        std::cout << "Failed to load texture!" << std::endl;
    }

    // here the data is flushed as it is loaded now
    stbi_image_free(imageData);

    imageData = stbi_load("assets/textures/awesomeface.png", &width, &height, &nrChannels, 0);

    // to use multiple textures, you to have to load it, generate it, bind it, set the texture parameters, generate the texture in opengl, and generate the mipmaps for each one.
    unsigned int texture2;
    glGenTextures(1, &texture2);
    glBindTexture(GL_TEXTURE_2D, texture2);

    // this sets the texture parameters. s and t are equivalent to x and y
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // copied from above
    if (imageData)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else
    {
        std::cout << "Failed to load texture 2!" << std::endl;
    }

    // free the second bit of data
    stbi_image_free(imageData);

    float vertices[] = {
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };

    glm::vec3 cubePositions[] = {
        glm::vec3( 0.0f,  0.0f,  0.0f),
        glm::vec3( 2.0f,  5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f),
        glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3( 2.4f, -0.4f, -3.5f),
        glm::vec3(-1.7f,  3.0f, -7.5f),
        glm::vec3( 1.3f, -2.0f, -2.5f),
        glm::vec3( 1.5f,  2.0f, -2.5f),
        glm::vec3( 1.5f,  0.2f, -1.5f),
        glm::vec3(-1.3f,  1.0f, -1.5f)
    };


    // VAOs or Vertex Array Objects store a given VBO and their necessary Vertex Attributes. This makes it so you don't have to run the VBO and VA every frame and can simply call the VAO. Modern OpenGL *Requires* this to draw.
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // this is a vertex buffer object. it is generated using an int and an id. then it is bound to its buffer type
    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

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
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);

    // since this is the tex coords which has only 2 data points, we set it to read location 1 in the first argument and the second defines how many
    // we stride 5 as there are 5 numbers until we hit the next set
    // we also change the byte offset since there are 3 floats until the first coord
    // we bind it to the VAO at another increment
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), reinterpret_cast<void*>(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // since we are editing the texture uniforms, we need to use the shader program before we can do that
    shader.use();

    // we set the uniforms to their corresponding GL_TEXTURE using our class
    shader.setInt("textureColor1", 0);
    shader.setInt("textureColor2", 1);

    // here we assign the shader uniform locations in memory here
    const GLint modelLocation = glGetUniformLocation(shader.ID, "modelMatrix");
    const GLint projectionLocation = glGetUniformLocation(shader.ID, "projectionMatrix");
    const GLint viewLocation = glGetUniformLocation(shader.ID, "viewMatrix");

    // enable depth buffer to avoid z-fighting
    glEnable(GL_DEPTH_TEST);

    InputHandler input(window, false);

    Camera camera;
    const float cameraSpeed = 7.5f;
    camera.setCameraSpeed(cameraSpeed);

    bool isMaximized = false;

    std::chrono::duration<double, std::milli> fixedUpdateTime = std::chrono::duration<double>(1.0 / UPDATE_TIME_IN_FPS);
    std::chrono::time_point previousTime = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> timeSinceLastFrame(std::chrono::milliseconds(0));

    SDL_SetWindowRelativeMouseMode(window, true);

    // while(running) loop is for all rendering and OpenGL code. while(poll) is specifically for window events and input.
    while(isRunning)
    {
        auto deltaTimeInNS = std::chrono::high_resolution_clock::now() - previousTime;
        previousTime = std::chrono::high_resolution_clock::now();
        timeSinceLastFrame += std::chrono::duration_cast<std::chrono::milliseconds>(deltaTimeInNS);

        float deltaTime = static_cast<float>(deltaTimeInNS.count()) / 1000000000.0f;

        input.updateInput(isRunning);

        if (input.isKeyHeld(SDL_SCANCODE_W))
        {
            camera.setCameraMovement(CAMERA_FORWARD);
        }
        if (input.isKeyHeld(SDL_SCANCODE_S))
        {
            camera.setCameraMovement(CAMERA_BACKWARD);
        }
        if (input.isKeyHeld(SDL_SCANCODE_A))
        {
            camera.setCameraMovement(CAMERA_LEFT);
        }
        if (input.isKeyHeld(SDL_SCANCODE_D))
        {
            camera.setCameraMovement(CAMERA_RIGHT);
        }

        camera.lockAxis(CAMERA_AXIS_Z, 0.0f, true);

        camera.setMouseX(input.getMouseX());
        camera.setMouseY(input.getMouseY());

        camera.update(deltaTime);

        camera.setFieldOfView(camera.getFieldOfView() - input.getMouseScrollWheel());

        if (input.isKeyPressed(SDL_SCANCODE_ESCAPE))
        {
            std::cout << "Escape is Pressed!\nShutting Down...\n";
            isRunning = false;
        }

        if (input.isKeyPressed(SDL_SCANCODE_F11))
        {
            if (isMaximized)
            {
                SDL_MaximizeWindow(window);
            }
            else
            {
                SDL_RestoreWindow(window);
            }

            isMaximized = !isMaximized;
        }

        while (timeSinceLastFrame >= fixedUpdateTime)
        {
            // fixed update
            timeSinceLastFrame -= fixedUpdateTime;
            //std::cout << "Fixed Update" << std::endl;
        }

        // here are the openGL commands
        // draw the background color then clear the screen every frame
        // we now also clear the depth buffer
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // we also bind each texture and then get the next unit and repeat. there are 16 texture units usually
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);

        // bind the VAO with references to the VBO, EBO, and vertex attributes
        glBindVertexArray(VAO);

        // use the shader program
        shader.use();

        // this defines a fov in the first argument, the viewport's width / height, and the near and far plane distance from the camera
        glm::mat4 projectionMatrix = glm::perspective(glm::radians(camera.getFieldOfView()), static_cast<float>(SDL_GetWindowSurface(window)->w) / static_cast<float>(SDL_GetWindowSurface(window)->h), 0.1f, 100.0f);

        // refer to the model matrix on how this works
        glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(camera.getViewMatrix()));

        // refer to the model matrix on how this works
        glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

        for (int i = 0; i < 10; ++i)
        {
            // here we create the translation matrix and translate it appropriately
            // we are forced to reset the matrix every frame
            glm::vec3 translation(cubePositions[i]);
            auto translationMatrix = glm::mat4(1.0f);
            translationMatrix = glm::translate(translationMatrix, translation);

            // rotation has to occur after translation, otherwise the rotation point is not adequately translated shifting the origin
            // we use a quaternion to avoid gimbal lock and also normalize that quaternion to remove vertex stretching
            float rotation = glm::radians((static_cast<float>(SDL_GetTicks()) / 1000.0f) * (static_cast<float>(i + 1) * 25.0f));
            glm::vec3 rotationAxis = glm::vec3(0.5f, 1.0f, 0.0f);
            glm::quat rotationQuaternion;
            rotationQuaternion = glm::normalize(glm::angleAxis(rotation, rotationAxis));
            glm::mat4 rotationMatrix = glm::mat4_cast(rotationQuaternion);

            // here we create a scale matrix. it must first be made with an identity so when we scale it using the scale variable, it doesn't end up 0
            glm::vec3 scale(1.0f, 1.0f, 1.0f);
            auto scaleMatrix = glm::mat4(1.0f);
            scaleMatrix = glm::scale(scaleMatrix, scale);

            // here we create the final matrix to apply all our actions
            glm::mat4 modelMatrix = translationMatrix * rotationMatrix * scaleMatrix;

            // we pass the transform location in shader memory, how many matrices to send, whether to transpose (swap columns and rows), and the actual matrix. glm may not store it in
            // an opengl compatible way so we use glm::value_ptr
            glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(modelMatrix));

            glDrawArrays(GL_TRIANGLES, 0, 36);
        }
        // we draw all the vertices here
        //glDrawArrays(GL_TRIANGLES, 0, 36);

        // swap the SDL front and back buffers
        SDL_GL_SwapWindow(window);
    }

    // here we clear all the data we are using
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    SDL_DestroyWindow(window);
    SDL_GL_DestroyContext(glContext);
    SDL_Quit();
    return 0;
}