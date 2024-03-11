#include <bgfx/bgfx.h>
#include <bx/bx.h>  
#include <SDL2/SDL.h>
#include <SDL_syswm.h>

const int WIDTH = 800;
const int HEIGHT = 600;

int main(int _argc, char** _argv) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
        return 1; 
    }

    SDL_Window* window = SDL_CreateWindow("bgfx + SDL2", 
                                  SDL_WINDOWPOS_UNDEFINED, 
                                  SDL_WINDOWPOS_UNDEFINED, 
                                  800, 600, 
                                  SDL_WINDOW_SHOWN);

    SDL_SysWMinfo wmi;
    SDL_VERSION(&wmi.version);
    if (!SDL_GetWindowWMInfo(window, &wmi))
    {
        return 1;
    }

    bgfx::PlatformData pd;
    pd.ndt = wmi.info.x11.display;
    pd.nwh = (void *)(uintptr_t)wmi.info.x11.window;

    bgfx::Init init;
    init.platformData = pd;
    init.type = bgfx::RendererType::OpenGL;
    init.resolution.width = WIDTH;
    init.resolution.height = HEIGHT;
    bgfx::init(init);

    bgfx::reset(WIDTH, HEIGHT, BGFX_RESET_VSYNC);

    bgfx::setDebug(BGFX_DEBUG_TEXT /*| BGFX_DEBUG_STATS*/);

    bgfx::setViewRect(0, 0, 0, uint16_t(WIDTH), uint16_t(HEIGHT));

    bgfx::setViewClear(0,
        BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH,
        0x443355FF, 1.0f, 0);

    bool running = true;
    while (running) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) { running = false; }
        }

        bgfx::setViewRect(0, 0, 0, WIDTH, HEIGHT);

        bgfx::touch(0);

        bgfx::setState(BGFX_STATE_DEFAULT);

        bgfx::frame(); 
    }

    bgfx::shutdown();

    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
