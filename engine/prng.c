// based on:
// https://github.com/love2d/love/blob/main/src/modules/math/RandomGenerator.cpp
// above link has links to sources

#include "prng.h"

#include <time.h>

static uint64_t WangHash64(uint64_t key)
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

Prng PrngCreate(uint32_t seed)
{
  if (seed == 0) {
    seed = time(NULL);
  }
  return WangHash64(seed);
}

uint64_t PrngNext(Prng* p)
{
  *p ^= (*p >> 12);
	*p ^= (*p << 25);
	*p ^= (*p >> 27);
  return *p * 2685821657736338717ULL;
}

int PrngGetInt(Prng* p)
{
  union {
    uint64_t uint64;
    int i;
  } n;
  n.uint64 = PrngNext(p);
  return n.i;
}

double PrngGetDouble(Prng* p)
{
  union {
    uint64_t uint64;
    double d;
  } n;
  n.uint64 = ((uint64_t)0x3FF) << 52 | PrngNext(p) >> 12;
  return n.d - 1.0;
}

int PrngGetIntRange(Prng* p, int min, int max)
{
  int i = PrngGetInt(p);
  return i % (max - min + 1) + min;
}

double PrngGetDoubleRange(Prng* p, double min, double max)
{
  double d = PrngGetDouble(p);
  return d * (max - min) + min;
}
