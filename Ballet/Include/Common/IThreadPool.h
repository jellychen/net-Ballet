#ifndef _include_common_threadpool_h
#define _include_common_threadpool_h
#include "Include/Def/Inct.h"
#include "Include/Common/Inct.h"

namespace common
{
    _interface_inherit_def(IThreadPool, IObject)
    {
        def_function(init,      (int));
        def_function(do_task,   (Task*));
    };
    _interface_inherit_bind(IThreadPool, IObject, common, 4B84DF877EBE4520, 8D26D8230C947538)
}
#endif//_include_common_threadpool_h