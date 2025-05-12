# Boomer Shooter

Currently this is just a 3D renderer, but in the future I want to make it into an actual game.

## Building
Make sure to clone the repo with submodules, and ensure Lua 5.1 or newer is installed.
### On Linux
```bash
$ sh build.sh debug
$ ./bin/debug/bs
```
The `debug` can be replaced with `release`, and that will enable optimizations,
if you want optimizations, but don't want to give up symbols, you can use `releasesym`.
### Windows
Currently not tested, and probably won't work. Will eventually work. Maybe.
