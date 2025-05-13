#include "timer.h"

#include <GLFW/glfw3.h>

struct Timer TimerCreate()
{
  struct Timer t;
  t.tick_rate = 30;
  t.accum = 0;
  t.fps = 0;
  t.tps = 0;
  t.prev_time = 0;
  t.last_fps = 0;
  t.frames_rendered = 0;
  t.ticks_ticked = 0;
  return t;
}

bool TimerShouldTick(const struct Timer* t)
{
  return t->accum > 1.0 / t->tick_rate;
}

void TimerNewTick(struct Timer* t)
{
  t->accum -= 1.0 / t->tick_rate;
  if (t->accum < 0) t->accum = 0;
  t->ticks_ticked++;
}

void TimerStep(struct Timer* t)
{
  double current_time = glfwGetTime();
  double dt = current_time - t->prev_time;
  t->prev_time = current_time;

  t->accum += dt;

  double since_fps_update = current_time - t->last_fps;
  if (since_fps_update > 1) {
    t->fps = (double)t->frames_rendered / since_fps_update;
    t->tps = (double)t->ticks_ticked / since_fps_update;
    t->last_fps = current_time;

    t->frames_rendered = 0;
    t->ticks_ticked = 0;
  }
}

void TimerDoneRendering(struct Timer* t)
{
  t->frames_rendered++;
}
