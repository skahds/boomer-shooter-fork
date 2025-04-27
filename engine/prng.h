#ifndef __engine_prng__
#define __engine_prng__

#include "include.h"

typedef uint64_t Prng;

// 0 for a auto generated seed
Prng PrngCreate(uint32_t seed);
uint64_t PrngNext(Prng* p);
int PrngGetInt(Prng* p);
double PrngGetDouble(Prng* p);

int PrngGetIntRange(Prng* p, int min, int max);
double PrngGetDoubleRange(Prng* p, double min, double max);

#endif
