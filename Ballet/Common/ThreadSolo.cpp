#include "ThreadSolo.h"

namespace common
{
    ThreadSolo::ThreadSolo()
    {
    }

    ThreadSolo::~ThreadSolo()
    {
    }

    void* ThreadSolo::__thread_function(void* _arg)
    {
        if_return(nullptr_t == _arg, 0);
        ((Task*)(_arg))->run();;
        return 0;
    }

    ret ThreadSolo::do_task(Task* _task)
    {
        if_return(nullptr_t == _task, ret_args_error);

        pthread_t _t;
        pthread_attr_t _attr;
        ::pthread_attr_init(&_attr);
        ::pthread_attr_setdetachstate(&_attr, PTHREAD_CREATE_DETACHED);
        int _ret = ::pthread_create(&_t, &_attr, ThreadSolo::__thread_function, (void*)_task);
        if (0 != _ret) _task->force_delete();
        ::pthread_attr_destroy(&_attr);
        return __bool_ret(0 == _ret);
    }
}