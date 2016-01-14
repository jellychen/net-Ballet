#include "ThreadTaskQueue.h"

namespace network
{
    ThreadTaskQueue::ThreadTaskQueue()
    {
        ::pthread_mutex_init(&task_locker_, nullptr_t);
        this->task_input_ = &this->task_queue_a_;
        this->task_output_ = &this->task_queue_b_;
    }

    ThreadTaskQueue::~ThreadTaskQueue()
    {
        ::pthread_mutex_destroy(&task_locker_);
    }

    ret ThreadTaskQueue::do_task()
    {
        ::pthread_mutex_lock(&task_locker_);
        queue_task_t* _temp = this->task_input_;
        this->task_input_ = this->task_output_;
        this->task_output_ = _temp;
        ::pthread_mutex_unlock(&task_locker_);

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
        ::pthread_mutex_unlock(&task_locker_);
        return ret_success;
    }
}