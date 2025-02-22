# HexChess

HexChess is a small chess game on a hexagonal board based on [CGPGrey's video](https://www.youtube.com/watch?v=bgR3yESAEVE).

## Getting started

The game is written in C++ using Microsoft's Visual Studio 2022. While it probably works on other platforms and can be compiled with other compilers, since I haven't tested it, I cannot guarantee it will work seamlessly.

To build it, first obtain the code, either by downloading the sources or by cloning the repo with your git client. I recommend to do it recursively so you then don't have to worry about the submodules.

```
git clone --recursive https://github.com/szkristof32/HexChess
```

### Dependencies

Before you could build it, you have to make sure all the dependencies are present. Most of them are submodules, so they should be downloaded automatically, but better be safe than sorry. Here you can find a list of all the dependencies:

- [GLFW](https://github.com/GLFW/GLFW) (submodule)
- [glm](https://github.com/g-truc/glm) (submodule)
- [Vulkan SDK](https://vulkan.lunarg.com/sdk/home) (you have to download and install it yourself, make sure to add the ```VULKAN_SDK``` environment variable)

Once all depenedencies are installed correctly run ```Bootstrap.bat``` (or ```Bootstrap.sh``` if you are on Linux), to generate the project files. I'm using [Premake](https://premake.github.io) for this.

To build it, simply open the Visual Studio solution, and hit Build. If you don't want to use the default compiler, you have to run premake yourself. You can find a more detailed documentation on their website.

The default compilers are:

- Windows: MSVC (Visual Studio 2022)
- Linux: Make + GCC
