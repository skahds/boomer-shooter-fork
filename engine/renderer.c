#include "renderer.h"

#include <glad/glad.h>
#include "context.h"

static void openglErrorCallback(
  GLenum source, 
  GLenum type, 
  unsigned int id, 
  GLenum severity, 
  GLsizei length, 
  const char *message, 
  const void *userParam)
{
  switch (severity) {
    case GL_DEBUG_SEVERITY_HIGH: 
      logError("[opengl] [high severity] %s", message);
      break;
    case GL_DEBUG_SEVERITY_MEDIUM: 
      logWarning("[opengl] [medium severity] %s", message);
      break;
    case GL_DEBUG_SEVERITY_LOW: 
      logWarning("[opengl] [low severity] %s", message);
      break;
    case GL_DEBUG_SEVERITY_NOTIFICATION: 
      logDebugInfo("[opengl] %s", message);
      break;
  }
}

static void updateStateProjection(Renderer* r, RendererState* state)
{
  float window_width = (float)ctxGetWindowWidth(r->ctx);
  float window_height = (float)ctxGetWindowHeight(r->ctx);

  switch (state->kind) {
    case RStateKind_2d:
      mat4Ortho(state->projection, 0, window_width, window_height, 0, 0.1, 100);
      break;
    case RStateKind_3d: {
      float aspect = window_width / window_height;
      mat4Perspective(state->projection, 80, aspect, 0.1, 100);
      break;
    }
  }
}

static void framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
  Context* ctx = glfwGetWindowUserPointer(window);
  ctx->screen_size.x = width / 3;
  ctx->screen_size.y = height / 3;
  glViewport(0, 0, width, height);

  RendererState* state = ctx->renderer.state;
  while (state != NULL) {
    updateStateProjection(&ctx->renderer, state);
    state = state->previous;
  }
}

void rendererInit(Renderer* r, Context* ctx)
{
#ifdef bse_debug
  glEnable(GL_DEBUG_OUTPUT);
  glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
  glDebugMessageCallback(openglErrorCallback, NULL);
  glDebugMessageControl(
    GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, NULL, GL_TRUE);
#endif

  glfwSetFramebufferSizeCallback(ctx->window_handle, framebufferSizeCallback);

  glEnable(GL_DEPTH_TEST);

  r->ctx = ctx;
  r->state = NULL;
  rendererPushNewState(r, RStateKind_3d);
  r->default_shader = shaderLoadFromFiles("vdefault.glsl", "fdefault.glsl");
  r->default_texture = textureSingleColor(colorRgb(1, 1, 1));
}

void rendererDestroy(Renderer* r)
{
  while (r->state != NULL) {
    rendererPopState(r);
  }
  shaderDestroy(&r->default_shader);
  textureDestroy(r->default_texture);
}

void rendererPushNewState(Renderer* r, RStateKind kind)
{
  RendererState* state = create(RendererState);
  state->previous = r->state;
  state->kind = kind;

  mat4Identity(state->view);

  updateStateProjection(r, state);

  r->state = state;
}

void rendererPopState(Renderer* r)
{
  RendererState* state = r->state;
  r->state = state->previous;
  destroy(state);
}

void rendererSetViewPosition(Renderer* r, Vec3f pos)
{
  // RendererState* state = r->state;
  // glm_mat4_identity(state->view);
  // glm_translate(state->view, pos);
}
