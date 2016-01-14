#include "ThreadPoolTaskQueue.h"

namespace common
{
    ThreadPoolTaskQueue::ThreadPoolTaskQueue()
    {
        this->exit_ = false;
        ::pthread_cond_init(&task_ready_, nullptr_t);
        ::pthread_mutex_init(&task_locker_, nullptr_t);
    }

    ThreadPoolTaskQueue::~ThreadPoolTaskQueue()
    {
        ::pthread_cond_destroy(&task_ready_);
        ::pthread_mutex_destroy(&task_locker_);
    }

    ret ThreadPoolTaskQueue::add_ref()
    {
        this->ref_.increment_and_get();
        return ret_success;
    }

    ret ThreadPoolTaskQueue::del_ref()
    {
        int32_t _ref = this->ref_.decrement_and_get();
        if (0 == _ref) delete this;
        return ret_success;
    }

    ret ThreadPoolTaskQueue::do_task(int* _left_task)
    {
        Task* _task = nullptr_t;
        ::pthread_mutex_lock(&task_locker_);
        if (this->task_queue_.size() <= 0 && !exit_)
        {
            ::pthread_cond_wait(&task_ready_, &task_locker_);
        }
        
        if (this->task_queue_.size() > 0)
        {
            _task = this->task_queue_.front();
            this->task_queue_.pop();
        }

        if (nullptr_t != _left_task)
        {
            *_left_task = (int)this->task_queue_.size();
        }
        ::pthread_mutex_unlock(&task_locker_);
        
        if (nullptr_t != _task) _task->run();
        return ret_success;
    }

    ret ThreadPoolTaskQueue::commit(const Task* _task)
    {
        if_return(nullptr_t == _task, ret_args_error);
        ::pthread_mutex_lock(&task_locker_);
        this->task_queue_.push((Task*)_task);
        ::pthread_cond_signal(&task_ready_);
        ::pthread_mutex_unlock(&task_locker_);
        return ret_success;
    }

    ret ThreadPoolTaskQueue::wakeup_all()
    {
        ::pthread_cond_broadcast(&task_ready_);
        return ret_success;
    }
}