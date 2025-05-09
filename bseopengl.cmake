set(
  OPENGL_BACKEND_SRC

  engine/gfx/opengl/gl_gfx.c
  engine/gfx/opengl/gl_buffer_object.c engine/gfx/opengl/gl_buffer_object.h
  engine/gfx/opengl/gl_type_conv.c engine/gfx/opengl/gl_type_conv.h
  engine/gfx/opengl/gl_vertex_array.c engine/gfx/opengl/gl_vertex_array.h
  engine/gfx/opengl/gl_shader.c
  engine/gfx/opengl/gl_texture.c
  engine/gfx/opengl/gl_framebuffer.c engine/gfx/opengl/gl_framebuffer.h
)

add_library(bseopengl STATIC ${OPENGL_BACKEND_SRC})
target_link_libraries(
  bseopengl PRIVATE
  m
  glad
  bsecore
)
target_include_directories(
  bseopengl PRIVATE
  engine
  lib/glad/include
  lib/glfw/include
)
