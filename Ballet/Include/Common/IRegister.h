#ifndef _include_common_iregister_h
#define _include_common_iregister_h
#include "Include/Def/Inct.h"

namespace common
{
    _interface_def(IRegister)
    {
        def_function(register_object,   (const _guid&, const _guid&));
        def_function(register_service,  (const _guid&, const _guid&));
    };
}
#endif//_include_common_iregister_h