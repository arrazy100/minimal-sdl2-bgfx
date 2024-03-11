# Minimal Example for SDL2 and BGFX

## Building

For the first time build, you need to clone and build bgfx using CMake, inside the project directory.

```bash
git clone https://github.com/bkaradzic/bgfx.cmake.git
cd bgfx.cmake
git submodule init
git submodule update
cmake -S. -Bcmake-build # $CMakeOptions
cmake --build cmake-build
```

Then you need to install SDL2, if you are using Ubuntu, you can run below command.

```bash
sudo apt-get install libsdl2-dev
```

After that, you can start to build the project.

```bash
cd build
cmake ..
cmake --build . --config Debug
```

## Running

To running the project after finished building, simply go to build folder and run the executable.

```bash
./sdl_bgfx_minimal
```