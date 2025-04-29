set(
  BSE_SRC

  engine/main.c
)

add_executable(bse ${BSE_SRC})
target_include_directories(
  bse PRIVATE
  engine
  lib/glfw/include
  lib/luajit/src
)
target_link_libraries(
  bse PRIVATE
  bsecore
)
add_dependencies(bse luajit)
