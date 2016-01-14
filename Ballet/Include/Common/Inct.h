#ifndef _include_common_common_h
#define _include_common_common_h
#include "Include/Def/Inct.h"
#include "Include/Common/Factory.h"
#include "Include/Common/ISerialize.h"

namespace common
{
    __api ret init          ();
    __api ret uninit        ();
    __api ret load_module   (const char* _m);
    __api ret load_config   (const char* _c);
    __api ret let_object    (const _guid& _d, void** ppOut);
    __api ret let_service   (const _guid& _d, void** ppOut);

    template <typename t>   inline bool let_object(t** ppOut)
    {
        return __success(let_object(_type_id(t), (void**)ppOut));
    }

    template <typename t>   inline bool let_service(t** ppOut)
    {
        return true;
    }
}
#endif//_include_common_common_h