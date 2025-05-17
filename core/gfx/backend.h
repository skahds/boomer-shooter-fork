#ifndef __engine_gfx_backend__
#define __engine_gfx_backend__

#include "include.h"

#include "gfx_types.h"
#include "math/vec2f.h"
#include "math/vec3f.h"
#include "math/vec4f.h"
#include "math/vec2i.h"
#include "math/vec3i.h"
#include "math/vec4i.h"
#include "math/mat4.h"

struct Engine;
struct BufferObject;
struct VertexArray;
struct VertexFormat;
struct Image;
struct Texture;
struct Shader;
struct Framebuffer;

typedef void (*ClearBackgroundFn)(float r, float g, float b);
typedef void (*AdjustViewportFn)(vec2f_t size);
typedef void (*SetDepthTestFn)(bool do_test);

typedef struct BufferObject* (*BufferObjectCreateFn)(
  enum BufferObjectType type
);
typedef void (*BufferObjectBindFn)(struct BufferObject* buf);
typedef void (*BufferObjectUnbindFn)(enum BufferObjectType type);
typedef void (*BufferObjectSetFn)(
  struct BufferObject* buf,
  void* data,
  size_t size,
  enum DrawMode mode
);
typedef void (*BufferObjectDestroyFn)(struct BufferObject* buf);

typedef struct VertexArray* (*VertexArrayCreateFn)(
  const struct VertexFormat* fmt);
typedef void (*VertexArrayDestroyFn)(struct VertexArray* varr);
typedef void (*VertexArrayBindFn)(struct VertexArray* varr);
typedef void (*VertexArrayDrawFn)(
  struct VertexArray* varr,
  size_t start,
  size_t count,
  enum IndexMode index_mode
);
typedef void (*VertexArrayDrawIndexedFn)(
  struct VertexArray* varr,
  struct BufferObject* ebo,
  size_t count,
  enum GfxDataType type,
  enum IndexMode index_mode
);

typedef struct Framebuffer* (*FramebufferCreateFn)(vec2i_t size, uint8_t flags);
typedef void (*FramebufferDestroyFn)(struct Framebuffer* fb);
typedef void (*FramebufferBindFn)(struct Framebuffer* fb);
typedef void (*FramebufferResizeFn)(struct Framebuffer* fb, vec2i_t size);
typedef void (*FramebufferDrawFn)(
  struct Framebuffer* fb,
  vec2i_t start,
  vec2i_t end
);

typedef struct Shader* (*ShaderLoadFromFilesFn)(
  const char* vert,
  const char* frag
);
typedef struct Shader* (*ShaderLoadFromSourceFn)(
  const char* vert,
  const char* frag
);
typedef void (*ShaderSendIntFn)(
  struct Shader* s,
  const char* name,
  int i
);
typedef void (*ShaderSendFloatFn)(
  struct Shader* s,
  const char* name,
  float f
);
typedef void (*ShaderSendVec2fFn)(
  struct Shader* s,
  const char* name,
  vec2f_t v
);
typedef void (*ShaderSendVec2iFn)(
  struct Shader* s,
  const char* name,
  vec2i_t v
);
typedef void (*ShaderSendVec3fFn)(
  struct Shader* s,
  const char* name, 
  vec3f_t v
);
typedef void (*ShaderSendVec3iFn)(
  struct Shader* s,
  const char* name,
  vec3i_t v
);
typedef void (*ShaderSendVec4fFn)(
  struct Shader* s,
  const char* name,
  vec4f_t v
);
typedef void (*ShaderSendVec4iFn)(
  struct Shader* s,
  const char* name,
  vec4i_t v
);
typedef void (*ShaderSendMat4Fn)(struct Shader* s, const char* name, mat4_t m);
typedef void (*ShaderBindFn)(struct Shader* s);
typedef void (*ShaderDestroyFn)(struct Shader* s);

typedef struct Texture (*TextureLoadFromImgFn)(struct Image* img);
typedef void (*TextureGenerateMipmapsFn)(struct Texture* tex);
typedef void (*TextureBindFn)(struct Texture* tex, uint8_t slot);
typedef void (*TextureSetFilterFn)(
  struct Texture* tex,
  enum TextureFilter min,
  enum TextureFilter mag
);
typedef void (*TextureSetWrapFn)(
  struct Texture* tex,
  enum TextureWrap x_wrap,
  enum TextureWrap y_wrap
);
typedef void (*TextureDestroyFn)(struct Texture* tex);

struct GraphicsBackend
{
  ClearBackgroundFn clear_background;
  AdjustViewportFn adjust_viewport;
  SetDepthTestFn set_depth_test;

  BufferObjectCreateFn buffer_object_create;
  BufferObjectDestroyFn buffer_object_destroy;
  BufferObjectBindFn buffer_object_bind;
  BufferObjectUnbindFn buffer_object_unbind;
  BufferObjectSetFn buffer_object_set;

  VertexArrayCreateFn vertex_array_create;
  VertexArrayDestroyFn vertex_array_destroy;
  VertexArrayBindFn vertex_array_bind;
  VertexArrayDrawFn vertex_array_draw;
  VertexArrayDrawIndexedFn vertex_array_draw_indexed;

  FramebufferCreateFn framebuffer_create;
  FramebufferDestroyFn framebuffer_destroy;
  FramebufferBindFn framebuffer_bind;
  FramebufferResizeFn framebuffer_resize;
  FramebufferDrawFn framebuffer_draw;

  ShaderLoadFromFilesFn shader_load_files;
  ShaderLoadFromSourceFn shader_load_src;
  ShaderDestroyFn shader_destroy;;
  ShaderSendIntFn shader_send_int;
  ShaderSendFloatFn shader_send_float;
  ShaderSendVec2fFn shader_send_vec2f;
  ShaderSendVec2iFn shader_send_vec2i;
  ShaderSendVec3fFn shader_send_vec3f;
  ShaderSendVec3iFn shader_send_vec3i;
  ShaderSendVec4fFn shader_send_vec4f;
  ShaderSendVec4iFn shader_send_vec4i;
  ShaderSendMat4Fn shader_send_mat4;
  ShaderBindFn shader_bind;

  TextureLoadFromImgFn texture_load_img;
  TextureDestroyFn texture_destroy;
  TextureGenerateMipmapsFn texture_gen_mipmaps;
  TextureBindFn texture_bind;
  TextureSetFilterFn texture_set_filter;
  TextureSetWrapFn texture_set_wrap;
};

#endif
