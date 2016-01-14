#ifndef _include_common_thread_h
#define _include_common_thread_h
#include "Include/Def/Inct.h"
#include "Include/Common/Inct.h"

namespace common
{
    _interface_inherit_def(IThread, IObject)
    {
        def_function(do_task, (Task*));
    };
    _interface_inherit_bind(IThread, IObject, common, A6EA73462E6B4E22, AB7D41AE2AFB5F6F)
}
#endif//_include_common_threadsolo_h