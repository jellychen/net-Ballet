#ifndef common_threadsolo_h
#define common_threadsolo_h
#include "Include/Def/Inct.h"
#include "Include/Common/Factory.h"
#include "Include/Common/IThreadSolo.h"
#include "Module.h"

namespace common
{
    _implement_bind(ThreadSolo, 7DF0E5C6FD204D29, B0868636B2A17DCF)
    _support IThreadSolo
    {
        _implement_support_begin()
        _implement_support(IThreadSolo)
        _implement_support_end()

    public:
        ThreadSolo();
        virtual ~ThreadSolo();

    public:
        static void* __thread_function(void*);

    public:
        ret do_task(Task* _task);
    };
    _implement_register_ref_object(IThreadSolo, ThreadSolo, __rtns, __module_register)
}
#endif//common_threadsolo_h