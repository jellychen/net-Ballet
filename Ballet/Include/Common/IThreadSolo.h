#ifndef _include_common_threadsolo_h
#define _include_common_threadsolo_h
#include "Include/Def/Inct.h"
#include "Include/Common/Inct.h"

namespace common
{
    _interface_inherit_def(IThreadSolo, IObject)
    {
        def_function(do_task,   (Task*));
    };
    _interface_inherit_bind(IThreadSolo, IObject, common, 7DF0E5C6FD204D29, B0868636B2A17DCF)

    namespace thread
    {
        inline ret execute_task(Task* _task)
        {
            if_return(nullptr_t == _task, ret_fail);

            __sp<IThreadSolo> _thread_solo;
            let_object(_type_id(IThreadSolo), (void**)&_thread_solo);
            if_return(nullptr_t == _thread_solo, ret_fail);
            return _thread_solo->do_task(_task);
        }
    }
}
#endif//_include_common_threadsolo_h