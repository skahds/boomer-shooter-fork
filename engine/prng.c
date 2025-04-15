// based on:
// https://github.com/love2d/love/blob/main/src/modules/math/RandomGenerator.cpp
// above link has links to sources

#include "prng.h"

#include <time.h>

static uint64_t wangHash64(uint64_t key)
{
	key = (~key) + (key << 21); // key = (key << 21) - key - 1;
	key = key ^ (key >> 24);
	key = (key + (key << 3)) + (key << 8); // key * 265
	key = key ^ (key >> 14);
	key = (key + (key << 2)) + (key << 4); // key * 21
	key = key ^ (key >> 28);
	key = key + (key << 31);
	return key;
}

Prng prngCreate(uint32_t seed)
{
  if (seed == 0) {
    seed = time(NULL);
  }
  return wangHash64(seed);
}

uint64_t prngNext(Prng* p)
{
  *p ^= (*p >> 12);
	*p ^= (*p << 25);
	*p ^= (*p >> 27);
  return *p * 2685821657736338717ULL;
}

int prngGetInt(Prng* p)
{
  union {
    uint64_t u64;
    int i;
  } n;
  n.u64 = prngNext(p);
  return n.i;
}

double prngGetDouble(Prng* p)
{
  union {
    uint64_t u64;
    double d;
  } n;
  n.u64 = ((uint64_t)0x3FF) << 52 | prngNext(p) >> 12;
  return n.d - 1.0;
}

int prngGetIntRange(Prng* p, int min, int max)
{
  uint32_t i = prngNext(p);
  return i % (max - min + 1) + min;
}

double prngGetDoubleRange(Prng* p, double min, double max)
{
  double d = prngGetDouble(p);
  return d * (max - min) + min;
}
