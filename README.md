# Boomer Shooter
Currently this is just a 3D renderer, but in the future I want to make it into an actual game.

## Building
#### Dependencies
- `7z` CLI tool
- LuaJIT shared library
- GLFW shared library
#### Configs
- `debug`: symbols, `-O2` optimizations, and on Linux, will enable asan.
- `release`: no symbols, `-O2` optimizations.
### Compiling
Compile LuaJIT, GLFW, and place them in the root directory of this project.
```bash
$ make config=debug
```
## Project Structure
The project structure is very simple:
- `core`: this is the engine
- `core/gfx`: for any rendering code
- `core/wrap`: wraps the engine api into Lua
- `core/math`: linear algebra
- `core/gfx/opengl`: for any opengl-specific code
- `core/had`: contains scripts and resources that the core engine needs
- `game`: where the game code goes
- `lib`: third-party libraries
- `res`: resources, like shaders, textures, audio, etc
