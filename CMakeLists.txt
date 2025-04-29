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
  -std=c99

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

include("bsecore.cmake")
include("bseopengl.cmake")
include("bse.cmake")
