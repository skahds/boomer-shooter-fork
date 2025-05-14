#ifndef __engine_key__
#define __engine_key__

#include "include.h"

enum Key
{
#define KEY_DEF(name, val) KEY_##name = (val),
#include "key_def.h"
#undef KEY_DEF
};

#endif
