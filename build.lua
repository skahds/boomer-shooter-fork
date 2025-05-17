local os_name = GetOS()
local defines = {}
local ldflags = {}

if ctx.config == "debug" then
  table.insert(defines, "bse_debug")
  if os_name == "Linux" then
    table.insert(ldflags, "-fsanitize=address")
  end
else
  table.insert(defines, "bse_release")
end

if os_name == "Linux" then
  table.insert(defines, "bse_linux")
  table.insert(defines, "bse_posix")
elseif os_name == "Windows" then
  table.insert(defines, "bse_windows")
else
  local err_msg = "Unsupported OS."
  if os_name == "OSX" then
    err_msg = err_msg .. " MacOS might be supported in the future."
  end
  LMakeError(err_msg)
  os.exit(1)
end

AddTarget("chime", "exe", {
  src = {
    "main.c",
    "core/include.c",
    "core/include.h",
    "core/log.c",
    "core/log.h",
    "core/engine.c",
    "core/engine.h",
    "core/key.h",
    "core/image.c",
    "core/image.h",
    "core/prng.c",
    "core/prng.h",
    "core/timer.c",
    "core/timer.h",
    "core/gfx/buffer_object.h",
    "core/gfx/framebuffer.h",
    "core/gfx/gfx.h",
    "core/gfx/gfx_types.c",
    "core/gfx/gfx_types.h",
    "core/gfx/mesh.c",
    "core/gfx/mesh.h",
    "core/gfx/shader.c",
    "core/gfx/shader.h",
    "core/gfx/texture.c",
    "core/gfx/texture.h",
    "core/gfx/vertex_array.c",
    "core/gfx/vertex_array.h",
    "core/math/defs.h",
    "core/math/mat4.c",
    "core/math/mat4.h",
    "core/math/transform.c",
    "core/math/transform.h",
    "core/math/vec2f.h",
    "core/math/vec2i.h",
    "core/math/vec3f.h",
    "core/math/vec3i.h",
    "core/math/vec4f.h",
    "core/math/vec4i.h",
    "core/wrap/wrap.c",
    "core/wrap/wrap.h",
    "core/wrap/wrap_enums.c",
    "core/wrap/wrap_engine.c",
    "core/wrap/wrap_log.c",
    "core/wrap/wrap_mat4.c",
    "core/wrap/wrap_mesh.c",
    "core/wrap/wrap_vertex_format.c",
    "core/wrap/wrap_vertex_format.h",
    "core/wrap/wrap_shader.c",
    "core/wrap/wrap_texture.c",
    "core/wrap/wrap_prng.c",
    "opengl/gl_buffer_object.c",
    "opengl/gl_buffer_object.h",
    "opengl/gl_framebuffer.c",
    "opengl/gl_framebuffer.h",
    "opengl/gl_gfx.c",
    "opengl/gl_shader.c",
    "opengl/gl_shader.h",
    "opengl/gl_texture.c",
    "opengl/gl_type_conv.c",
    "opengl/gl_type_conv.h",
    "opengl/gl_vertex_array.c",
    "opengl/gl_vertex_array.h",
  },
  include = {
    "core",
    "lib/luajit/src",
    "lib/glfw/include",
    "lib/glad/include",
    "lib/miniz/include",
    "lib/stb/include",
  },
  define = defines,
  ldflags = ldflags,
  libs = {"glfw", "glad", "stb", "miniz", "luajit", "m"},
})

AddTarget("glad", "staticlib", {
  src = {"lib/glad/src/glad.c"},
  include = {"lib/glad/include"},
})

AddTarget("stb", "staticlib", {
  src = {"lib/stb/stb.c"},
  include = {"lib/stb/include"},
})

AddTarget("miniz", "staticlib", {
  src = {"lib/miniz/miniz.c"},
  include = {"lib/miniz/include"},
})

AddTarget("glfw", "command", {
  commands = {
    "cmake -S lib/glfw -B bin/glfw",
    "cmake --build bin/glfw",
  },
  clean_commands = {
    "cmake --build bin/glfw --target clean",
  },
  output_type = "staticlib",
  output_binary = "bin/glfw/src/libglfw3.a",
})

AddTarget("luajit", "command", {
  commands = {
    "make -C lib/luajit",
  },
  clean_commands = {
    "make -C lib/luajit clean",
  },
  output_type = "staticlib",
  output_binary = "lib/luajit/src/libluajit.a",
})
