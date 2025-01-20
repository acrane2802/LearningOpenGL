#include <iostream>
#include <glad/glad.h>
#include <SDL.h>

// constants for window size at the beginning of the program
#define WINDOW_HEIGHT 600
#define WINDOW_WIDTH 800

// useful functions that interface with SDL
void framebuffer_callback(SDL_Window* window, int width, int height);
void render(int deltaTime);

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
    SDL_GLContext gl_context = SDL_GL_CreateContext(window);

    // make sure all OpenGL extensions can be accessed, otherwise, close
    if(!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress))
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
                case SDL_QUIT:
                    isRunning = false;
                    break;
                case SDL_WINDOWEVENT:
                    if(e.window.event == SDL_WINDOWEVENT_RESIZED)
                    {
                        framebuffer_callback(window, SDL_GetWindowSurface(window)->w,  SDL_GetWindowSurface(window)->h);
                    }
                    break;
                case SDL_KEYDOWN:
                    switch(e.key.keysym.sym)
                    {
                        case SDLK_ESCAPE:
                            isRunning = false;
                            return 0;
                        break;
                        default:
                            break;
                    }
                    break;
                default:
                    break;
            }
        }
        // here are the openGL commands, deltaTime is currently unused but will be needed later
        render(0);
        SDL_GL_SwapWindow(window);
    }

    SDL_Quit();
    return 0;
}

// this resizes the viewport so opengl can adapt dynamically
void framebuffer_callback(SDL_Window* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

// here is where all opengl code goes that needs to be looped
void render(int deltaTime)
{
    // draw the background color then clear the screen every frame
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}