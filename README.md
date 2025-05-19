# Boomer Shooter
Currently this is just a 3D renderer, but in the future I want to make it into an actual game.

## Building
#### Dependencies
- `7z` CLI tool
- LuaJIT shared library
- GLFW shared library
#### Configs
- `debug` - symbols, `-O2` optimizations, and on Linux, will enable asan.
- `release` - no symbols, `-O2` optimizations.
### On Linux
Needs the `zip` command line tool.
Compile LuaJIT, GLFW, and place them in the root directory of this project.
```bash
$ make config=debug
```
### Windows
Other than that, the same as Linux.

## Project Structure
The project structure is very simple:
- `core`: this is the engine
- `core/gfx`: for any rendering code
- `core/wrap`: wraps the engine api into Lua
- `core/math`: linear algebra
- `core/gfx/opengl`: for any opengl-specific code
- `game`: where the game code goes
- `lib`: third-party libraries
- `res`: resources, like shaders, textures, audio, etc
