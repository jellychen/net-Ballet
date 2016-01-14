#ifndef common_thread_h
#define common_thread_h
#include "Include/Def/Inct.h"
#include "Include/Common/Factory.h"
#include "Include/Common/IThread.h"
#include "ThreadTaskQueue.h"
#include "Module.h"

namespace common
{
    _implement_bind(Thread, 9F44CA9C7B7040f0, 932CF64CDB8760BA)
    _support IThread
    {
        _implement_support_begin()
        _implement_support(IThread)
        _implement_support_end()

    public:
        Thread();
        virtual ~Thread();

    public:
        static void* __thread_function(void*);

    public:
        ret do_task(Task* _task);

    private:
        bool                thread_;
        ThreadTaskQueue*    task_queue_;
    };
    _implement_register_ref_object(IThread, Thread, __rtns, __module_register)
}
#endif//common_threadsolo_h