#include <iostream>
#include <glad/gl.h>
#include <SDL3/SDL.h>

// constants for window size at the beginning of the program
#define WINDOW_HEIGHT 600
#define WINDOW_WIDTH 800

// useful functions that interface with SDL
void framebufferCallback(SDL_Window* window, int width, int height);

// arguments in main are required so SDL_main doesn't cause compilation issues
int main(int argc, char* args[])
{
    // beginning variables for both naming the window and the only variable that should be changed if the program needs to be shut down.
    const std::string title = "000 OpenGL Window";
    bool isRunning = false;

    // set up SDL to begin its video subsystems and set the opengl attributes to avoid this program running on unsupported hardware
    SDL_Init(SDL_INIT_VIDEO);

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
    SDL_Event e;

    // while(running) loop is for all rendering and OpenGL code. while(poll) is specifically for window events and input.
    while(isRunning)
    {
        while(SDL_PollEvent(&e))
        {
            switch(e.type)
            {
                case SDL_EVENT_QUIT:
                    isRunning = false;
                    break;
                case SDL_EVENT_WINDOW_RESIZED:
                    framebufferCallback(window, SDL_GetWindowSurface(window)->w,  SDL_GetWindowSurface(window)->h);
                    break;
                case SDL_EVENT_KEY_DOWN:
                    switch(e.key.key)
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

        // swap the SDL front and back buffers
        SDL_GL_SwapWindow(window);
    }

    SDL_DestroyWindow(window);
    SDL_GL_DestroyContext(glContext);
    SDL_Quit();
    return 0;
}

// this resizes the viewport so opengl can adapt dynamically
void framebufferCallback(SDL_Window* window, int width, int height)
{
    glViewport(0, 0, width, height);
}