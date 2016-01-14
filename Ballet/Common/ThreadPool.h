#ifndef common_threadpool_h
#define common_threadpool_h
#include "Include/Def/Inct.h"
#include "Include/Common/Factory.h"
#include "Include/Common/IThreadPool.h"
#include "ThreadPoolTaskQueue.h"
#include "Module.h"

namespace common
{
    _implement_bind(ThreadPool, 3CAFC583F6324F46, A7BF1F12D296A44D)
    _support IThreadPool
    {
        _implement_support_begin()
        _implement_support(IThreadPool)
        _implement_support_end()

    public:
        ThreadPool();
        virtual ~ThreadPool();

    public:
        static void* __thread_function(void*);

    public:
        ret init    (int _num);
        ret do_task (Task* _task);

    private:
        ThreadPoolTaskQueue* task_queue_;
    };
    _implement_register_ref_object(IThreadPool, ThreadPool, __rtns, __module_register)
}
#endif//common_threadpool_h