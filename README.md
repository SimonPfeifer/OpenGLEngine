# OpenGLEngine
This is a toy renderer written in OpenGL.

## Requirement
This project currently only works on Windows!

```
make
cmake
gcc
```

My way of installing the dependencies on Windows is via [msys2](https://www.msys2.org/). I use MINGW64 and install the dependencies with the package manager `pacman`.

```
pacman -S mingw-w64-x86_64-toolchain
pacman -S mingw-w64-x86_64-cmake
```

## Building
To dowload the project along with its submodules:
```
git clone --recursive https://github.com/SimonPfeifer/OpenGLEngine.git
```

Then enter the directory and configure the submodules:
```
cd OpenGLEngine
git init
git submodule update --remote 
```

Now we can start building the application. This might take a little bit:
```
mkdir build
cd build
cmake ..
cmake --build .
```

## Running
The executable is located in:
```
./OpenGLEngine/build/bin/Application.exe
```

## Controls
Control the camera with your mouse.
```
W     : forward
A     : left
S     : back
D     : right
Space : up
Ctrl  : down
Q     : quit
Esc   : pause (to use the mouse)
