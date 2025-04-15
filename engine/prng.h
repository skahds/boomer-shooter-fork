#ifndef __bse_prng__
#define __bse_prng__

#include "include.h"

typedef uint64_t Prng;

// 0 for a auto generated seed
Prng prngCreate(uint32_t seed);
uint64_t prngNext(Prng* p);
int prngGetInt(Prng* p);
double prngGetDouble(Prng* p);
float prngGetFloat(Prng* p);

int prngGetIntRange(Prng* p, int min, int max);
double prngGetDoubleRange(Prng* p, double min, double max);

#endif
