#ifndef __engine_timer__
#define __engine_timer__

#include "include.h"

/// handles the fixed update loop and measures FPS and TPS
struct Timer
{
  double tick_rate;
  double accum;

  int fps;
  int tps;

  double prev_time;
  double last_fps;
  int frames_rendered;
  int ticks_ticked;
};

struct Timer TimerCreate();
bool TimerShouldTick(const struct Timer* t);
void TimerNewTick(struct Timer* t);
void TimerStep(struct Timer* t);
void TimerDoneRendering(struct Timer* t);

#endif
