#include <iostream>
#include <sstream>
#include <fstream>
#include <cmath>
#include <glad/gl.h>
#include <SDL.h>

#include "Shader.h"

// constants for window size at the beginning of the program
#define WINDOW_HEIGHT 600
#define WINDOW_WIDTH 800

// useful functions that interface with SDL
void framebufferCallback(SDL_Window* window, int width, int height);

// arguments in main are required so SDL_main doesn't cause compilation issues
int main(int argc, char* args[])
{
    // beginning variables for both naming the window and the only variable that should be changed if the program needs to be shut down.
    const std::string title = "001 OpenGL Triangle";
    bool isRunning = false;

    // set up SDL to begin its video subsystems and set the opengl attributes to avoid this program running on unsupported hardware
    SDL_Init(SDL_INIT_VIDEO);

    SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, 3 );
    SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, 3 );
    SDL_GL_SetAttribute( SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE );

    // create the window pointer, beginning around the middle of the screen with the dimension constants and the opengl flag
    SDL_Window* window = SDL_CreateWindow(title.c_str(), 800, 600, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_OPENGL);

    // this could be commented out at a later date. it is here to test the function framebuffer_callback
    SDL_SetWindowResizable(window, SDL_TRUE);

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
    SDL_Event e;

    // call a custom shader class to load the shaders and assemble the program
    Shader shader("./assets/shaders/shaders_attributes_vertex_shader.glsl", "./assets/shaders/shaders_attributes_fragment_shader.glsl");

    // array of vertices with the color data following after it
    float vertices[] = {
        -0.5f, -0.5f, 0.0f,      1.0f, 0.0f, 0.0f,
         0.5f, -0.5f, 0.0f,      0.0f, 1.0f, 0.0f,
         0.0f,  0.5f, 0.0f,      0.0f, 0.0f, 1.0f
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
    // the next argument is the stride. it defines how far in memory to move to find the next x coordinates. since we have color data in the vertices we need to move ahead 6 now instead of 3
    // and the final portion is a weird data offset. this has been set to nullptr here as a 0 literal but in other instances, a reinterpret_cast to void* would be *technically* correct but ugly. This is the OpenGL API problem
    // after the vertex behavior is defined, we enable the first group (location 0)

    // which vbo this uses is determined by whichever VBO is bound to the context, if a new vbo is laid out differently, we need to run this again
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);

    // this is the same as above except we begin at the next set of 3 (location 1) and stride 6 like above. cannot static cast the offset necessary so a reinterpret cast is needed
    // the reinterpret_cast<void*> here is hideous and really dangerous. Be careful about undefined behavior
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), reinterpret_cast<void*>(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // while(running) loop is for all rendering and OpenGL code. while(poll) is specifically for window events and input.
    while(isRunning)
    {
        while(SDL_PollEvent(&e))
        {
            switch(e.type)
            {
                case SDL_QUIT:
                    isRunning = false;
                    break;
                case SDL_WINDOWEVENT:
                    if(e.window.event == SDL_WINDOWEVENT_RESIZED)
                    {
                        framebufferCallback(window, SDL_GetWindowSurface(window)->w,  SDL_GetWindowSurface(window)->h);
                    }
                    break;
                case SDL_KEYDOWN:
                    switch(e.key.keysym.sym)
                    {
                        case SDLK_ESCAPE:
                            isRunning = false;
                            break;
                        default:
                            break;
                    }
                    break;
                default:
                    break;
            }
        }
        // here are the openGL commands
        // draw the background color then clear the screen every frame
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // use the shader program, bind the VAO with references to the VBO and vertex attributes, then run the draw command, with arguments to define the starting index and the number of vertices
        shader.use();
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        // swap the SDL front and back buffers
        SDL_GL_SwapWindow(window);
    }

    SDL_DestroyWindow(window);
    SDL_GL_DeleteContext(window);
    SDL_Quit();
    return 0;
}

// this resizes the viewport so opengl can adapt dynamically
void framebufferCallback(SDL_Window* window, int width, int height)
{
    glViewport(0, 0, width, height);
}