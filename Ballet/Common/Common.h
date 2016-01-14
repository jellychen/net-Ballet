#ifndef common_common_h
#define common_common_h
#define __attribute_export
#include "Include/Def/Inct.h"

namespace common
{
    __api ret init          ();
    __api ret uninit        ();
    __api ret load_module   (const char* _m);
    __api ret load_config   (const char* _c);
    __api ret let_object    (const _guid& _d, void** ppOut);
    __api ret let_service   (const _guid& _d, void** ppOut);
}
#endif//common_common_h