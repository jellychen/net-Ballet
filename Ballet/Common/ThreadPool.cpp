#include "ThreadPool.h"

namespace common
{
    ThreadPool::ThreadPool()
    {
        this->task_queue_ = nullptr_t;
    }

    ThreadPool::~ThreadPool()
    {
        if (nullptr_t != this->task_queue_)
        {
            this->task_queue_->exit_ = true;
            this->task_queue_->wakeup_all();
        }
    }

    void* ThreadPool::__thread_function(void* _arg)
    {
        if_return(nullptr_t == _arg, 0);
        ThreadPoolTaskQueue* _task_queue = (ThreadPoolTaskQueue*) _arg;

        _task_queue->add_ref();
        while (true)
        {
            int _left_task = 0;
            _task_queue->do_task(&_left_task);
            if_break(0 == _left_task);

        }
        _task_queue->del_ref();
        _task_queue = nullptr_t;
        return 0;
    }

    ret ThreadPool::init(int _num)
    {
        if_return(0 >= _num, ret_args_error);
        this->task_queue_ = new(std::nothrow) ThreadPoolTaskQueue();
        if_return(nullptr_t == this->task_queue_, ret_fail);
        this->task_queue_ ->add_ref();

        for (int i = 0; i < _num; ++i)
        {
            pthread_t _t;
            pthread_attr_t _attr;
            ::pthread_attr_init(&_attr);
            ::pthread_attr_setdetachstate(&_attr, PTHREAD_CREATE_DETACHED);
            int _ret = ::pthread_create(&_t, &_attr, ThreadPool::__thread_function, (void*)task_queue_);
            ::pthread_attr_destroy(&_attr);
            if_continue(0 != _ret);
        }

        this->task_queue_ ->del_ref();
        return ret_success;
    }

    ret ThreadPool::do_task(Task* _task)
    {
        if_return(nullptr_t == _task, ret_args_error);
        if (nullptr_t == this->task_queue_)
        {
            _task->force_delete();
            return ret_fail;
        }
        return this->task_queue_->commit(_task);
    }
}