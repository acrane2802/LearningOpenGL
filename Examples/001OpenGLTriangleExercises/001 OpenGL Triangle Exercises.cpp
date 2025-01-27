#include <iostream>
#include <sstream>
#include <fstream>
#include <glad/gl.h>
#include <SDL.h>

// constants for window size at the beginning of the program
#define WINDOW_HEIGHT 600
#define WINDOW_WIDTH 800

// useful functions that interface with SDL
void framebufferCallback(SDL_Window* window, int width, int height);

std::string readShader(std::ifstream& file);

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

    // open the shader file and read the data into a string for runtime compilation then convert into a c style string
    std::ifstream vertexShaderFile;
    vertexShaderFile.open("./assets/shaders/triangle_vertex_shader.glsl");
    std::string vertexShaderSource = readShader(vertexShaderFile);
    const char* vertexShaderData = vertexShaderSource.c_str();

    // all opengl objects need an id so we create one and assign it to a vertex object
    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);

    // here we attach the shader code from the file to the given id that was generated above and is attached to the GL_VERTEX_SHADER object. the "1" here tells us how many strings are being compiled.
    // Then we compile using the id
    glShaderSource(vertexShader, 1, &vertexShaderData, nullptr);
    glCompileShader(vertexShader);

    // this creates a variable used to determine the compilation success, a char array to store the specific error, then polls opengl for the shader status, storing it in the success variable
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

    // if the compilation fails, it retrieves the error, stores it in the array, then prints it out
    if(!success)
    {
        glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
        std::cout << "Vertex Shader failed to compile!\n" << infoLog << std::endl;
    }

    // repeating the same steps as before, reading in the shader data into a const char* c style string
    std::ifstream fragmentShaderFile;
    fragmentShaderFile.open("./assets/shaders/triangle_fragment_shader.glsl");
    std::string fragmentShaderSource = readShader(fragmentShaderFile);
    const char* fragmentShaderData = fragmentShaderSource.c_str();

    // assigning the shader an id and then creating it
    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

    // linking the shader data and the shader id then compiling
    glShaderSource(fragmentShader, 1, &fragmentShaderData, nullptr);
    glCompileShader(fragmentShader);

    // once again, get status of the shader compilation for fragment
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);

    // verify if the compilation is successful. if it isn't, print out an error
    if(!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog);
        std::cout << "Fragment Shader failed to compile!\n" << infoLog << std::endl;
    }

    // open the shader file and read the data into a string for runtime compilation then convert into a c style string
    std::ifstream vertexShaderFile2;
    vertexShaderFile2.open("./assets/shaders/triangle_exercises_vertex_shader.glsl");
    std::string vertexShaderSource2 = readShader(vertexShaderFile2);
    const char* vertexShaderData2 = vertexShaderSource2.c_str();

    // all opengl objects need an id so we create one and assign it to a vertex object
    unsigned int vertexShader2;
    vertexShader2 = glCreateShader(GL_VERTEX_SHADER);

    // here we attach the shader code from the file to the given id that was generated above and is attached to the GL_VERTEX_SHADER object. the "1" here tells us how many strings are being compiled.
    // Then we compile using the id
    glShaderSource(vertexShader2, 1, &vertexShaderData2, nullptr);
    glCompileShader(vertexShader2);

    // this creates a variable used to determine the compilation success, a char array to store the specific error, then polls opengl for the shader status, storing it in the success variable
    glGetShaderiv(vertexShader2, GL_COMPILE_STATUS, &success);

    // if the compilation fails, it retrieves the error, stores it in the array, then prints it out
    if(!success)
    {
        glGetShaderInfoLog(vertexShader2, 512, nullptr, infoLog);
        std::cout << "Vertex Shader 2 failed to compile!\n" << infoLog << std::endl;
    }

    // repeating the same steps as before, reading in the shader data into a const char* c style string
    std::ifstream fragmentShaderFile2;
    fragmentShaderFile2.open("./assets/shaders/triangle_exercises_fragment_shader.glsl");
    std::string fragmentShaderSource2 = readShader(fragmentShaderFile2);
    const char* fragmentShaderData2 = fragmentShaderSource2.c_str();

    // assigning the shader an id and then creating it
    unsigned int fragmentShader2;
    fragmentShader2 = glCreateShader(GL_FRAGMENT_SHADER);

    // linking the shader data and the shader id then compiling
    glShaderSource(fragmentShader2, 1, &fragmentShaderData2, nullptr);
    glCompileShader(fragmentShader2);

    // once again, get status of the shader compilation for fragment
    glGetShaderiv(fragmentShader2, GL_COMPILE_STATUS, &success);

    // verify if the compilation is successful. if it isn't, print out an error
    if(!success)
    {
        glGetShaderInfoLog(fragmentShader2, 512, nullptr, infoLog);
        std::cout << "Fragment Shader 2 failed to compile!\n" << infoLog << std::endl;
    }

    // create a shader program id and link it to a shader program object. a shader program can be shared across multiple objects or be recreated per object. recreating it is expensive so share as much as you can
    // the shader program links all the shaders of an object together and also links their individual inputs and outputs
    // this has been changed to an array to avoid excess variable names
    unsigned int shaderPrograms[2];
    shaderPrograms[0] = glCreateProgram();

    // here we set which program gets which shaders, then we link it to the context here
    glAttachShader(shaderPrograms[0], vertexShader);
    glAttachShader(shaderPrograms[0], fragmentShader);
    glLinkProgram(shaderPrograms[0]);

    // check the program status, then print an error if it fails
    glGetProgramiv(shaderPrograms[0], GL_LINK_STATUS, &success);

    if(!success)
    {
        glGetProgramInfoLog(shaderPrograms[0], 512, nullptr, infoLog);
        std::cout << "Shader Program failed to link!\n" << infoLog << std::endl;
    }


    // this code is copied to make a second shader program
    shaderPrograms[1] = glCreateProgram();

    // here we set which program gets which shaders, then we link it to the context here
    glAttachShader(shaderPrograms[1], vertexShader2);
    glAttachShader(shaderPrograms[1], fragmentShader2);
    glLinkProgram(shaderPrograms[1]);

    // check the program status, then print an error if it fails
    glGetProgramiv(shaderPrograms[1], GL_LINK_STATUS, &success);

    if(!success)
    {
        glGetProgramInfoLog(shaderPrograms[1], 512, nullptr, infoLog);
        std::cout << "Shader Program 2 failed to link!\n" << infoLog << std::endl;
    }

    // we can clear the shader objects we made as they have been copied into the shader program
    glDeleteShader(vertexShader);
    glDeleteShader(vertexShader2);
    glDeleteShader(fragmentShader);
    glDeleteShader(fragmentShader2);

    // array of vertices
    float vertices[] = {
        -0.75f, -0.5f, 0.0f,
         0.25f, -0.5f, 0.0f,
        -0.25f,  0.5f, 0.0f
    };

    // second set of vertices
    float vertices2[] = {
        -0.25f, -0.5f, 0.0f,
         0.75f, -0.5f, 0.0f,
         0.25f,  0.5f, 0.0f
    };

    // VAOs or Vertex Array Objects store a given VBO and their necessary Vertex Attributes. This makes it so you don't have to run the VBO and VA every frame and can simply call the VAO. Modern OpenGL *Requires* this to draw.
    // this has been changed to an array to avoid excess variable names
    unsigned int VAOs[2];
    glGenVertexArrays(1, &VAOs[0]);
    glBindVertexArray(VAOs[0]);

    // this is a vertex buffer object. it is generated using an int and an id. then it is bound to its buffer type
    // this has been changed to an array to avoid excess variable names
    unsigned int VBOs[2];
    glGenBuffers(1, &VBOs[0]);
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);

    // this function takes in the buffer type that is currently bound, the size of the data in bytes, the pointer to the first data of the array, and the way the data is used. GL_STATIC_DRAW is read once, use everywhere
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // this tells opengl how the vertices are laid out in memory. the first is the array we want to configure. this is the location variable set in the vertex shader
    // the second attribute tells us the size of the vertex vector. since a vertex here is defined by xyz, it is a vec3
    // the third argument defines the vertex coordinate datatype
    // the next specifies whether the data should be normalized (-1, 0, 1). since the data is in floats, we don't want it to be
    // the next argument is the stride. it defines how far in memory to move to find the next x coordinate. in this configuration, we move 3 floats. in a different declaration, this needs to be changed
    // and the final portion is a weird data offset. this has been set to nullptr here as a 0 literal but in other instances, a reinterpret_cast to void* would be *technically* correct but ugly. This is the OpenGL API problem
    // after the vertex behavior is defined, we enable the first group (location 0)

    // which vbo this uses is determined by whichever VBO is bound to the context, if a new vbo is laid out differently, we need to run this again
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);

    // this is another VAO and VBO to make a second triangle. this has to be done after the previous because only one can be bound at a time
    glGenVertexArrays(1, &VAOs[1]);
    glBindVertexArray(VAOs[1]);

    glGenBuffers(1, &VBOs[1]);
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices2), vertices2, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);

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
        glUseProgram(shaderPrograms[0]);
        glBindVertexArray(VAOs[0]);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        // render two VBOs, VAOs, and use separate shaders
        glUseProgram(shaderPrograms[1]);
        glBindVertexArray(VAOs[1]);
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

std::string readShader(std::ifstream& file)
{
    // open a string stream to begin reading in data, copy it to a buffer, then close the file.
    std::stringstream dataBuffer;
    dataBuffer << file.rdbuf();
    file.close();

    // return the dataBuffer in string form
    return dataBuffer.str();
}