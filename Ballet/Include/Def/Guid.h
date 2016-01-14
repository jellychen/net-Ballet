#ifndef _include_def_guid_h
#define _include_def_guid_h
#include <stdint.h>
#include "Def.h"

typedef struct _guid {int64_t _d1, _d2;} _guid;

inline bool operator < (const _guid& l, const _guid& r)
{
    if (l._d1 < r._d1) return true;
    else if (l._d1 > r._d2) return false;
    else if (l._d2 < r._d2) return true;
    else if (l._d2 > r._d2) return false;
    return false;
}

inline bool operator == (const _guid& l, const _guid& r)
{
    return (l._d1 == r._d1 && l._d2 == r._d2);
}

inline bool operator != (const _guid& l, const _guid& r)
{
    return (l._d1 != r._d1 && l._d2 != r._d2);
}
#endif//_include_def_guid_h