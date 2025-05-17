#ifndef __engine_gfx_opengl_gl_shader__
#define __engine_gfx_opengl_gl_shader__

#include "gfx/shader.h"

struct Shader
{
  uint32_t handle;
  struct ShaderTable uniforms;
  struct ShaderTable attrs;
};

struct Shader* gl_ShaderLoadFromFiles(const char* vert, const char* frag);
struct Shader* gl_ShaderLoadFromSource(const char* vert, const char* frag);
void gl_ShaderSendInt(struct Shader* s, const char* name, int i);
void gl_ShaderSendFloat(struct Shader* s, const char* name, float f);
void gl_ShaderSendVec2f(struct Shader* s, const char* name, vec2f_t v);
void gl_ShaderSendVec2i(struct Shader* s, const char* name, vec2i_t v);
void gl_ShaderSendVec3f(struct Shader* s, const char* name, vec3f_t v);
void gl_ShaderSendVec3i(struct Shader* s, const char* name, vec3i_t v);
void gl_ShaderSendVec4f(struct Shader* s, const char* name, vec4f_t v);
void gl_ShaderSendVec4i(struct Shader* s, const char* name, vec4i_t v);
void gl_ShaderSendMat4(struct Shader* s, const char* name, mat4_t m);
void gl_ShaderBind(struct Shader* s);
void gl_ShaderDestroy(struct Shader* s);

#endif
