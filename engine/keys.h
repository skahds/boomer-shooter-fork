#ifndef __bse_keys__
#define __bse_keys__

typedef enum
{
#define KEY_DEF(name, val) Key_##name = (val),
#include "key_def.h"
#undef KEY_DEF
} Key;

#endif
