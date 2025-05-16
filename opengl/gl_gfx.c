#include "gfx/gfx.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

static void MessageCallback(
  GLenum source,
  GLenum type,
  GLuint id,
  GLenum severity,
  GLsizei length,
  const GLchar* message,
  const void* userParam)
{
  switch (severity) {
    case GL_DEBUG_SEVERITY_NOTIFICATION:
      LogDebug("[" TEXT_DARK_GRAY "OpenGL" TEXT_NORMAL "] %s", message);
      return;
    default:
      LogWarning("[" TEXT_DARK_GRAY "OpenGL" TEXT_NORMAL "] %s", message);
      return;
  }
}

void InitBackend(struct Engine* engine)
{
  glfwMakeContextCurrent(engine->window_handle);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    LogFatal(1, "could not initialize glad");
  }
  LogInfo("initialized glad");

#ifdef bse_debug
  glEnable(GL_DEBUG_OUTPUT);
  glDebugMessageCallback(MessageCallback, 0);
#endif

  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);
  // glEnable(GL_CULL_FACE);
  // glCullFace(GL_BACK);
  // glFrontFace(GL_CW);
}

void ClearBackground(float r, float g, float b)
{
  glClearColor(r, g, b, 1);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void AdjustViewport(vec2f_t size)
{
  glViewport(0, 0, size.x, size.y);
}

void SetDepthTest(bool test)
{
  if (test) glEnable(GL_DEPTH_TEST);
  else glDisable(GL_DEPTH_TEST);
}
