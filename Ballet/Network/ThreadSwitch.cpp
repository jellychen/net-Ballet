#include "ThreadSwitch.h"

namespace network
{
    ThreadSwitch::ThreadSwitch():callback_key_(this)
    {
        this->event_fd_ = -1;
        this->callback_key_->hook_notify_read(&ThreadSwitch::on_read);
    }

    ThreadSwitch::~ThreadSwitch()
    {
        if (this->event_fd_ > 0)
        {
            ::close(this->event_fd_);
        }
    }

    ret ThreadSwitch::init(IEventPoll* poll)
    {
        if_return(nullptr_t == poll, ret_args_error);
        this->event_fd_ = ::eventfd(0, EFD_NONBLOCK);
        if_return(-1 == this->event_fd_, ret_fail);
        return __success(poll->set_event_listener(this->event_fd_, event_read, callback_key_));
    }

    ret ThreadSwitch::commit(const Task* _task)
    {
        if_return(nullptr_t == _task, ret_args_error);
        ThreadTaskQueue::commit(_task);
        uint64_t _u = 1;
        ::write(this->event_fd_, &_u, sizeof(uint64_t));
        return ret_success;
    }

    ret ThreadSwitch::on_read(int id, IEventPoll* poll)
    {
        uint64_t _u;
        ::read(this->event_fd_, &_u, sizeof(uint64_t));
        ThreadTaskQueue::do_task();
        return callback_complete;
    }
}