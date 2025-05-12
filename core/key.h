#ifndef __engine_key__
#define __engine_key__

enum Key
{
#define KEY_DEF(name, val) KEY_##name = (val),
#include "key_def.h"
#undef KEY_DEF
};

#endif
