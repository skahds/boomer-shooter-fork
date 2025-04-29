set(
  CORE_SRC

  # engine
  engine/include.c engine/include.h
  engine/log.c engine/log.h
  engine/engine.c engine/engine.h
  engine/key.h
  engine/image.c engine/image.h
  engine/prng.c engine/prng.h

  # math
  engine/math/mat4.c engine/math/mat4.h
  engine/math/transform.c engine/math/transform.h

  # graphics
  engine/gfx/buffer_object.h
  engine/gfx/vertex_array.c engine/gfx/vertex_array.h
  engine/gfx/gfx_types.c engine/gfx/gfx_types.h
  engine/gfx/shader.c engine/gfx/shader.h
  engine/gfx/mesh.c engine/gfx/mesh.h
  engine/gfx/texture.c engine/gfx/texture.h

  # lua wrapper
  engine/wrap/wrap.c engine/wrap/wrap.h
  engine/wrap/wrap_enums.c
  engine/wrap/wrap_engine.c
  engine/wrap/wrap_log.c
  engine/wrap/wrap_mat4.c
  engine/wrap/wrap_mesh.c
  engine/wrap/wrap_vertex_format.c engine/wrap/wrap_vertex_format.h
  engine/wrap/wrap_shader.c
  engine/wrap/wrap_texture.c
  engine/wrap/wrap_prng.c
)

add_library(bsecore STATIC ${CORE_SRC})
target_include_directories(
  bsecore PRIVATE
  engine
  lib/glfw/include
  lib/luajit/src
)
target_link_libraries(
  bsecore PRIVATE
  m
  glfw
  libluajit
  bseopengl
)

