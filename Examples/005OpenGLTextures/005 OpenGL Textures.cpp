#include <iostream>
#include <sstream>
#include <fstream>
#include <glad/gl.h>
#include <SDL.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

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
    const std::string title = "002 OpenGL Rectangle";
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

    // loads the shader and compiles a program
    Shader shader("assets/shaders/textures_vertex_shader.glsl", "assets/shaders/textures_fragment_shader.glsl");

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

    // array of vertices, color, and texture coordinates
    float vertices[] = {
         0.5f,  0.5f, 0.0f,     1.0f, 0.0f, 0.0f,     1.0f, 1.0f,
         0.5f, -0.5f, 0.0f,     0.0f, 1.0f, 0.0f,     1.0f, 0.0f,
        -0.5f, -0.5f, 0.0f,     0.0f, 0.0f, 1.0f,     0.0f, 0.0f,
        -0.5f,  0.5f, 0.0f,     1.0f, 1.0f, 1.0f,     0.0f, 1.0f
    };

    // this is a set of indices to tell us when to draw which vertex
    unsigned int indices[] = {
        0, 1, 3,
        1, 2, 3
    };

    // here we generate an Element Buffer Object to make it so unique vertices are drawn but repeated vertices don't take up more resources than needed
    unsigned int EBO;
    glGenBuffers(1, &EBO);

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

    // refer to the VBO version of this below on how the function works
    // you have to bind the indices after the vertices, otherwise opengl will crash
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // this tells opengl how the vertices are laid out in memory. the first is the array we want to configure. this is the location variable set in the vertex shader
    // the second attribute tells us the size of the vertex vector. since a vertex here is defined by xyz, it is a vec3
    // the third argument defines the vertex coordinate datatype
    // the next specifies whether the data should be normalized (-1, 0, 1). since the data is in floats, we don't want it to be
    // the next argument is the stride. it defines how far in memory to move to find the next x coordinate. in this configuration, we move 8 floats due to the vertex array. in a different declaration, this needs to be changed
    // and the final portion is a weird data offset. this has been set to nullptr here as a 0 literal but in other instances, a reinterpret_cast to void* would be *technically* correct but ugly. This is the OpenGL API problem
    // after the vertex behavior is defined, we enable the first group (location 0)

    // which vbo this uses is determined by whichever VBO is bound to the context, if a new vbo is laid out differently, we need to run this again
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);

    // this is the same as above except we begin at the next set of 3 (location 1) and stride 8 like above. cannot static cast the offset necessary so a reinterpret cast is needed
    // the reinterpret_cast<void*> here is hideous and really dangerous. Be careful about undefined behavior
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), reinterpret_cast<void*>(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // since this is the tex coords which has only 2 data points, we set it to read 2 in the first argument and the second defines how many
    // we stride 8 as there are 8 numbers until we hit the next set
    // we also change the byte offset since there are 6 floats until the first coord
    // we bind it to the VAO at another increment
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), reinterpret_cast<void*>(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    // since we are editing the texture uniforms, we need to use the shader program before we can do that
    shader.use();

    // we set the uniforms to their corresponding GL_TEXTURE using our class
    shader.setInt("textureColor1", 0);
    shader.setInt("textureColor2", 1);

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

        // we also bind each texture and then get the next unit and repeat. there are 16 texture units usually
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);

        // use the shader program, bind the VAO with references to the VBO, EBO, and vertex attributes
        // then we run the draw command with the drawing mode, the number of elements to draw (6 indices so 6 vertices), then indices data type, and the offset which is 0 given our location begins at 0
        // the offset in glDrawElements is a void*. if this offset needs to be changed, it should be set to reinterpret_cast. be careful with that cast. it can cause undefined behavior
        shader.use();
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

        // swap the SDL front and back buffers
        SDL_GL_SwapWindow(window);
    }

    // here we clear all the data we are using
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

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
