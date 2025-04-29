#include "gfx/gfx.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

void InitBackend(struct Engine* engine)
{
  glfwMakeContextCurrent(engine->window_handle);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    LogFatal(1, "could not initialize glad");
  }
  LogInfo("initialized glad");

  glEnable(GL_DEPTH_TEST);
}

void ClearBackground(float r, float g, float b)
{
  glClearColor(r, g, b, 1);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void AdjustViewport(Vec2f size)
{
  glViewport(0, 0, size.x, size.y);
}
