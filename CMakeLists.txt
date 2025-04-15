cmake_minimum_required(VERSION 4.0)

project(bse)

set(CMAKE_MODULE_PATH "${bse_SOURCE_DIR}/modules" ${CMAKE_MODULE_PATH})

# TODO: Add MSVC support

if(LINUX)
  add_compile_definitions(bse_linux)
elseif(WIN32)
  add_compile_definitions(bse_windows)
# maybe support apple someday? (fat chance)
# elseif(APPLE)
#   add_compile_definitions(bse_apple)
else()
  message(FATAL_ERROR "platform not supported")
endif()

add_compile_options(
  -O2

  -Wall
  -Wextra

  -Wno-unused-parameter
  -Wno-unused-value
)

if(CONFIG STREQUAL "RELEASE")
  add_compile_options(-g)
  add_compile_definitions(bse_release)
elseif(CONFIG STREQUAL "DEBUG")
  add_compile_options(-g)
  add_compile_definitions(bse_debug)

  if(LINUX)
    add_compile_options(-fsanitize=address -fsanitize=undefined)
    add_link_options(-fsanitize=address -fsanitize=undefined)
  endif()
else()
  message(FATAL_ERROR
    "invalid config. add either -DCONFIG=RELEASE or -DCONFIG=DEBUG")
endif()

add_subdirectory("lib/glad")
add_subdirectory("lib/glfw")
include("lib/luajit.cmake")

set(
  BSE_SRC

  # engine
  engine/main.c
  engine/include.h
  engine/mem.h
  engine/context.c engine/context.h
  engine/log.c engine/log.h engine/exit_codes.h
  engine/keys.h
  engine/luawrap.c engine/luawrap.h
  engine/renderer.c engine/renderer.h
  engine/texture.c engine/texture.h
  engine/shader.c engine/shader.h
  engine/material.c engine/material.h
  engine/mesh.c engine/mesh.h
  engine/prng.c engine/prng.h

  engine/math/defs.h
  engine/math/mat4.c engine/math/mat4.h
  engine/math/transform.c engine/math/transform.h
  engine/math/vec2f.h
  engine/math/vec2i.h
  engine/math/vec3f.h
  engine/math/vec3i.h
  engine/math/vec4f.h
  engine/math/vec4i.h

  engine/lua/wrap_log.c
  engine/lua/wrap_context.c
  engine/lua/wrap_input.c
  engine/lua/wrap_prng.c
  engine/lua/wrap_mesh.c
  engine/lua/wrap_material.c
  engine/lua/wrap_texture.c
)
add_executable(bse ${BSE_SRC})
target_include_directories(
  bse PRIVATE
  engine
  lib/glad/include
  lib/glfw/include
  lib/luajit/src
)
target_link_libraries(
  bse PRIVATE
  m
  glad
  glfw
  libluajit
)
add_dependencies(bse luajit)
