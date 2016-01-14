#include "Thread.h"
#include <queue>

namespace common
{
    Thread::Thread()
    {
        this->thread_ = false;
        this->task_queue_ = nullptr_t;
    }

    Thread::~Thread()
    {
        if (nullptr_t != task_queue_)
        {
            if (this->thread_)
            {
                task_queue_->exit_ = true;
            }
            else
            {
                delete task_queue_;
            }
        }
    }

    void* Thread::__thread_function(void* _arg)
    {
        if_return(nullptr_t == _arg, 0);
        ThreadTaskQueue* _task_queue = (ThreadTaskQueue*)(_arg);
        
        while (true)
        {
            _task_queue->do_task(true);
            bool _should_exit = false;
            _task_queue->should_exit(&_should_exit);
            if_continue(false == _should_exit);
            _task_queue->do_task(false);
            break;
        }

        delete _task_queue;
        return 0;
    }

    ret Thread::do_task(Task* _task)
    {
        if_return(nullptr_t == _task, ret_args_error);
        if (nullptr_t == this->task_queue_)
        {
            this->task_queue_ = new(std::nothrow) ThreadTaskQueue();
            if (nullptr_t == this->task_queue_) _task->force_delete();
            if_return(nullptr_t == this->task_queue_, ret_fail);
        }

        if (false == this->thread_)
        {
            pthread_t _t;
            pthread_attr_t _attr;
            ::pthread_attr_init(&_attr);
            ::pthread_attr_setdetachstate(&_attr, PTHREAD_CREATE_DETACHED);
            int _ret = ::pthread_create(&_t, &_attr, Thread::__thread_function, (void*)task_queue_);
            if (0 != _ret) _task->force_delete();
            ::pthread_attr_destroy(&_attr);
            if_return(0 != _ret, ret_fail);
            this->thread_ = true;
        }
        return this->task_queue_->commit(_task);
    }
}