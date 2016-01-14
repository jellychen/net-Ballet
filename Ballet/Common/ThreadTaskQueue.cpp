#include "ThreadTaskQueue.h"

namespace common
{
    ThreadTaskQueue::ThreadTaskQueue()
    {
        ::pthread_cond_init(&task_ready_, nullptr_t);
        ::pthread_mutex_init(&task_locker_, nullptr_t);
        this->task_input_ = &this->task_queue_a_;
        this->task_output_ = &this->task_queue_b_;
    }

    ThreadTaskQueue::~ThreadTaskQueue()
    {
        ::pthread_cond_destroy(&task_ready_);
        ::pthread_mutex_destroy(&task_locker_);
    }

    ret ThreadTaskQueue::do_task(bool _lock_wait)
    {
        if (true == _lock_wait)
        {
            ::pthread_mutex_lock(&task_locker_);
            if (this->task_input_->size() <= 0)
            {
                ::pthread_cond_wait(&task_ready_, &task_locker_);
            }
        }

        queue_task_t* _temp = this->task_input_;
        this->task_input_ = this->task_output_;
        this->task_output_ = _temp;

        if (true == _lock_wait)
        {
            ::pthread_mutex_unlock(&task_locker_);
        }

        while (this->task_output_->size() > 0)
        {
            Task* _task = this->task_output_->front();
            this->task_output_->pop();
            if_continue(nullptr_t == _task);
            _task->run();
        }
        return ret_success;
    }

    ret ThreadTaskQueue::commit(const Task* _task)
    {
        if_return(nullptr_t == _task, ret_args_error);
        ::pthread_mutex_lock(&task_locker_);
        this->task_input_->push((Task*)_task);
        ::pthread_cond_signal(&task_ready_);
        ::pthread_mutex_unlock(&task_locker_);
        return ret_success;
    }

    ret ThreadTaskQueue::should_exit(bool* _exit)
    {
        if_return(nullptr_t == _exit, ret_args_error);
        *_exit = this->exit_;
        return ret_success;
    }
}