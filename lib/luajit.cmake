# this is so stupid, i hate cmake
# maybe i'll switch to premake, or even make my own build system

add_custom_command(
  OUTPUT lib/luajit/src/libluajit.a
  COMMAND make
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/lib/luajit
)

add_custom_target(
  luajit ALL
  DEPENDS lib/luajit/src/libluajit.a
)

add_library(libluajit STATIC IMPORTED GLOBAL)
add_dependencies(libluajit luajit)

set_target_properties(
  libluajit
  PROPERTIES
  IMPORTED_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/lib/luajit/src/libluajit.a
  INTERFACE_INCLUDE_DIRECTORIES ${CMAKE_CURRENT_SOURCE_DIR}/lib/luajit/src
)
