#include <bgfx/bgfx.h>
#include <bgfx/embedded_shader.h>
#include <bx/bx.h>
#include <bx/math.h> 
#include <SDL2/SDL.h>
#include <SDL_syswm.h>

// Cube Shaders
#include <cube/vs_cube.sc.glsl.bin.h>
#include <cube/vs_cube.sc.essl.bin.h>
#include <cube/vs_cube.sc.spv.bin.h>
#include <cube/fs_cube.sc.glsl.bin.h>
#include <cube/fs_cube.sc.essl.bin.h>
#include <cube/fs_cube.sc.spv.bin.h>
#if defined(_WIN32)
#include <cube/vs_cube.sc.dx9.bin.h>
#include <cube/vs_cube.sc.dx11.bin.h>
#include <cube/fs_cube.sc.dx9.bin.h>
#include <cube/fs_cube.sc.dx11.bin.h>
#endif //  defined(_WIN32)
#if __APPLE__
#include <cube/vs_cube.sc.mtl.bin.h>
#include <cube/fs_cube.sc.mtl.bin.h>
#endif // __APPLE__

const int WIDTH = 800;
const int HEIGHT = 600;

struct PosColorVertex
{
    float x;
    float y;
    float z;
    uint32_t abgr;
};

static PosColorVertex cubeVertices[] =
{
    {-1.0f,  1.0f,  1.0f, 0xff000000 },
    { 1.0f,  1.0f,  1.0f, 0xff0000ff },
    {-1.0f, -1.0f,  1.0f, 0xff00ff00 },
    { 1.0f, -1.0f,  1.0f, 0xff00ffff },
    {-1.0f,  1.0f, -1.0f, 0xffff0000 },
    { 1.0f,  1.0f, -1.0f, 0xffff00ff },
    {-1.0f, -1.0f, -1.0f, 0xffffff00 },
    { 1.0f, -1.0f, -1.0f, 0xffffffff },
};

static const uint16_t cubeTriList[] =
{
    0, 1, 2,
    1, 3, 2,
    4, 6, 5,
    5, 6, 7,
    0, 2, 4,
    4, 2, 6,
    1, 5, 3,
    5, 7, 3,
    0, 4, 1,
    4, 5, 1,
    2, 3, 6,
    6, 3, 7,
};

static const bgfx::EmbeddedShader s_embeddedShaders[] = 
{
    BGFX_EMBEDDED_SHADER(vs_cube),
    BGFX_EMBEDDED_SHADER(fs_cube),

    BGFX_EMBEDDED_SHADER_END()
};

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

    // Set vertex and index buffer
    bgfx::VertexLayout pcvDecl;
    pcvDecl.begin()
        .add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
        .add(bgfx::Attrib::Color0, 4, bgfx::AttribType::Uint8, true)
    .end();
    bgfx::VertexBufferHandle vbh = bgfx::createVertexBuffer(bgfx::makeRef(cubeVertices, sizeof(cubeVertices)), pcvDecl);
    bgfx::IndexBufferHandle ibh = bgfx::createIndexBuffer(bgfx::makeRef(cubeTriList, sizeof(cubeTriList)));

    // Create cube program
    bgfx::RendererType::Enum rendererType = bgfx::getRendererType();
    bgfx::ShaderHandle vsh_cube = bgfx::createEmbeddedShader(s_embeddedShaders, rendererType, "vs_cube");
    bgfx::ShaderHandle fsh_cube = bgfx::createEmbeddedShader(s_embeddedShaders, rendererType, "fs_cube");
    bgfx::ProgramHandle cubeProgram = bgfx::createProgram(vsh_cube, fsh_cube, true);

    int counter = 0;
    bool running = true;
    while (running) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) { running = false; }
        }

        bgfx::setViewRect(0, 0, 0, WIDTH, HEIGHT);

        bgfx::touch(0);

        bgfx::setState(BGFX_STATE_DEFAULT);

        // Submit primitive for rendering to view 0.
        const bx::Vec3 at = {0.0f, 0.0f,  0.0f};
        const bx::Vec3 eye = {0.0f, 0.0f, -5.0f};
        float view[16];
        bx::mtxLookAt(view, eye, at);
        float proj[16];
        bx::mtxProj(proj, 60.0f, float(WIDTH) / float(HEIGHT), 0.1f, 100.0f, bgfx::getCaps()->homogeneousDepth);
        bgfx::setViewTransform(0, view, proj);

        bgfx::setVertexBuffer(0, vbh);
        bgfx::setIndexBuffer(ibh);

        // transform
        bgfx::setViewTransform(0, view, proj);
        float mtx[16];
        bx::mtxRotateXY(mtx, counter * 0.01f, counter * 0.01f);
        bgfx::setTransform(mtx);

        bgfx::submit(0, cubeProgram);

        bgfx::frame(); 

        counter++;
    }

    bgfx::destroy(vbh);
    bgfx::destroy(ibh);
    bgfx::destroy(vsh_cube);
    bgfx::destroy(fsh_cube);
    bgfx::destroy(cubeProgram);
    bgfx::shutdown();

    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
