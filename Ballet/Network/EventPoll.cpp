#include "EventPoll.h"

namespace network
{
    EventPoll::EventPoll()
    {
        this->efd_ = -1;
        this->should_exit_ = false;
        this->ready_list_protected_ = false;
    }

    EventPoll::~EventPoll()
    {
        if (0 < this->efd_)
        {
            ::close(this->efd_);
        }
    }

    ret EventPoll::init()
    {
        ::signal(SIGPIPE, SIG_IGN);
        if_return(this->efd_ > 0, ret_fail);
        this->efd_ = ::epoll_create(256);
        if_return(this->efd_ <= 0, ret_fail);
        IEventPoll* _poll = __get_i_entity<IEventPoll>();
        if_return(nullptr_t == _poll, ret_fail);
        this->thread_switch_.init(_poll);
        __get_instance<Signal>().init(_poll);
        return ret_success;
    }

    ret EventPoll::add_delay_release(IObject* _object)
    {
        if_return(nullptr_t == _object, ret_args_error);
        this->release_pool_.push_back(_object);
        return ret_success;
    }

    ret EventPoll::add_timer_once(int id, ITimerCallback* callback, int _time)
    {
        if_return(nullptr_t == callback, ret_args_error);
        return __bool_ret(this->time_heap_pool_.set_timer_out(id, callback, _time));
    }
    
    ret EventPoll::add_timer_interval(int id, ITimerCallback* callback, int _time)
    {
        if_return(nullptr_t == callback, ret_args_error);
        return __bool_ret(this->time_heap_pool_.set_timer_loop(id, callback, _time));
    }

    ret EventPoll::del_timer(int id, ITimerCallback* callback)
    {
        if_return(nullptr_t == callback, ret_args_error);
        return __bool_ret(this->time_heap_pool_.del_timer(id, callback));
    }

    ret EventPoll::set_event_listener(int id, int flag, IEventCallback* callback)
    {
        if_return(0 >= this->efd_, ret_fail);
        if_return(nullptr_t == callback || id <= 0, ret_args_error);
        if_return(!(flag & (event_read | event_write)), ret_args_error);

        _t_map_event_pool::iterator _it = this->event_pool_.find(id);
        if (this->event_pool_.end() == _it)
        {
            _event_callback _callback = {EPOLLET, -1, callback};
            _callback._flag |= ((flag & event_read) ? EPOLLIN : 0);
            _callback._flag |= ((flag & event_write) ? EPOLLOUT : 0);
            if_return(EPOLLET == _callback._flag, ret_fail);

            struct epoll_event _ev; 
            memset(&_ev, 0, sizeof(epoll_event));
            _ev.data.fd = id;
            _ev.events = _callback._flag;
            if_return(0 != ::epoll_ctl(this->efd_, EPOLL_CTL_ADD, id, &_ev), ret_fail);
            this->event_pool_[id] = _callback;
        }
        else
        {
            struct epoll_event _ev;
            memset(&_ev, 0, sizeof(epoll_event));
            _ev.data.fd = id;
            _ev.events = _it->second._flag;

            bool _change = false;
            if ((!(_ev.events & EPOLLIN)) && (flag & event_read))
            {
                _change = true;
                _ev.events |= EPOLLIN;
            }
            
            if ((!(_ev.events & EPOLLOUT)) && (flag & event_write))
            {
                _change = true;
                _ev.events |= EPOLLOUT;
            }

            if_return(!_change, ret_success);
            if_return(0 != ::epoll_ctl(this->efd_, EPOLL_CTL_MOD, id, &_ev), ret_fail);
            _it->second._flag = _ev.events;
        }
        return ret_success;
    }

    ret EventPoll::del_event_listener(int id, int flag)
    {
        if_return(0 >= id, ret_args_error);
        if_return(0 >= this->efd_, ret_fail);
        if_return(!(flag & (event_read | event_write)), ret_args_error);

        _t_map_event_pool::iterator _it = this->event_pool_.find(id);
        if_return(this->event_pool_.end() == _it, ret_fail);
        struct epoll_event _ev;
        memset(&_ev, 0, sizeof(epoll_event));
        _ev.data.fd = id; _ev.events = _it->second._flag;

        uint32_t _change = 0;
        if ((_ev.events & EPOLLIN) && (flag & event_read))
        {
            _change |= event_read;
            _ev.events &= ~EPOLLIN;
        }

        if ((_ev.events & EPOLLOUT) && (flag & event_write))
        {
            _change |= event_write;
            _ev.events &= ~EPOLLOUT;
        }
        if_return(0 == _change, ret_success);

        if (EPOLLET == _ev.events || 0 == _ev.events)
        {
            if_return(0 != ::epoll_ctl(this->efd_, EPOLL_CTL_DEL, id, &_ev), ret_fail);
            this->del_ready_list(_it->second._index, id, _change);
            this->event_pool_.erase(_it);
        }
        else
        {
            if_return(0 != ::epoll_ctl(this->efd_, EPOLL_CTL_MOD, id, &_ev), ret_fail);
            this->del_ready_list(_it->second._index, id, _change);
            _it->second._flag = _ev.events;
        }
        return ret_success;
    }

    ret EventPoll::set_signal_listener(int id, ISignalCallback* callback)
    {
        if_return(id <= 0 || nullptr_t == callback, ret_fail);
        return __bool_ret(__get_instance<Signal>().set_signal(id, callback));
    }

    ret EventPoll::del_signal_listener(int id)
    {
        if_return(id <= 0, ret_fail);
        return __bool_ret(__get_instance<Signal>().del_signal(id));
    }

    ret EventPoll::event_loop_entry(TaskQueueEntry** _entry)
    {
        if_return(nullptr_t == _entry, ret_args_error);
        *_entry = (TaskQueueEntry*)(&this->thread_switch_);
        return ret_success;
    }

    ret EventPoll::event_loop()
    {
        if_return(0 >= this->efd_, ret_fail);
        this->should_exit_ = false;
        IEventPoll* _poll = __get_i_entity<IEventPoll>();
        if_return(nullptr_t == _poll, ret_fail);
        
        const static int _max_read_fd = 10240;
        struct epoll_event _events[_max_read_fd];
        while (false == this->should_exit_)
        {
            this->release_pool_.clear();
            
            if (this->event_ready_list_.size() > 0)
            {
                this->exc_ready_list(_poll);
            }

            int64_t _wait_time = 0;
            if (this->event_ready_list_.size() <= 0)
            {
                this->time_heap_pool_.lastest_timeout(_wait_time);
            }
            this->time_heap_pool_.tick(_poll);

            int _nfds = ::epoll_wait(this->efd_, _events, _max_read_fd, (int)_wait_time);
            if (-1 == _nfds && errno == EINTR)
            {
                errno = 0;
                continue;
            }

            for (int i = 0; i < _nfds; ++i)
            {
                _t_map_event_pool::iterator _it = this->event_pool_.find(_events[i].data.fd);
                if_continue(this->event_pool_.end() == _it);
                _event_callback& callback = _it->second;

                if (_events[i].events & EPOLLIN)
                {
                    callback._index = (int)this->add_ready_list(callback._index,
                                _events[i].data.fd, event_read, _it, callback._callback);
                }

                if (_events[i].events & EPOLLOUT)
                {

                    callback._index = (int)this->add_ready_list(callback._index,
                                _events[i].data.fd, event_write, _it, callback._callback);
                }
            }
        }
        return ret_success;
    }

    ret EventPoll::event_loop_once(int _time)
    {
        if_return(0 >= this->efd_, ret_fail);
        this->should_exit_ = false;
        IEventPoll* _poll = __get_i_entity<IEventPoll>();
        if_return(nullptr_t == _poll, ret_fail);
        
        this->release_pool_.clear();
        if (this->event_ready_list_.size() > 0)
        {
            this->exc_ready_list(_poll);
        }

        int64_t _wait_time = 0;
        if (this->event_ready_list_.size() <= 0)
        {
            this->time_heap_pool_.lastest_timeout(_wait_time);
        }
        this->time_heap_pool_.tick(_poll);
        
        if (_time > 0 && _time < _wait_time)
        {
            _wait_time = _time;
        }

        const static int _max_read_fd = 10240;
        struct epoll_event _events[_max_read_fd];
        int _nfds = ::epoll_wait(this->efd_, _events, _max_read_fd, (int)_wait_time);
        this->time_heap_pool_.tick(_poll);
        for (int i = 0; i < _nfds; ++i)
        {
            _t_map_event_pool::iterator _it = this->event_pool_.find(_events[i].data.fd);
             if_continue(this->event_pool_.end() == _it);
            _event_callback& callback = _it->second;

            if (_events[i].events & EPOLLIN)
            {
                callback._index = (int)this->add_ready_list(callback._index,
                        _events[i].data.fd, event_read, _it, callback._callback);
            }

            if (_events[i].events & EPOLLOUT)
            {
                callback._index = (int)this->add_ready_list(callback._index,
                        _events[i].data.fd, event_write, _it, callback._callback);
            }
        }
        return ret_success;
    }

    ret EventPoll::exit_event_loop()
    {
        this->should_exit_ = true;
        return ret_success;
    }

    ret EventPoll::exc_ready_list(IEventPoll* poll)
    {
        if_return(nullptr_t == poll, ret_args_error);

        this->ready_list_protected_ = true;
        int _len = (int)this->event_ready_list_.size() -1;
        for (int i = 0; i <= _len;)
        {
            bool _closed = false;
            _event_ready_item& _item = this->event_ready_list_[i];

            if (_item._read && false == _closed)
            {
                ret _ret = _item._callback->notify_read(_item._id, poll);
                if (_ret == callback_complete || _ret == callback_again)
                {
                    _item._read = 0;
                }
                else if (_ret == callback_close)
                {
                    _closed = true;
                }
            }

            if (_item._write && false == _closed)
            {
                ret _ret = _item._callback->notify_write(_item._id, poll);
                if (_ret == callback_complete || _ret == callback_again)
                {
                    _item._write = 0;
                }
                else if (_ret == callback_close)
                {
                    _closed = true;
                }
            }

            if (_closed || (0 == _item._write && 0 == _item._read))
            {
                if (i < _len)
                {
                    this->event_ready_list_[i]._iter->second._index = -1;
                    this->event_ready_list_[_len]._iter->second._index = i;
                    this->event_ready_list_[i] = this->event_ready_list_[_len];
                }
                else
                {
                    this->event_ready_list_[_len]._iter->second._index = -1;
                }

                this->event_ready_list_[_len]._callback = nullptr_t;
                this->event_ready_list_.resize(_len);
                _len--;
            }
            else
            {
                ++i;
            }
        }
        this->ready_list_protected_ = false;
        return ret_success;
    }

    ret EventPoll::add_ready_list(int index, int id, int ready, _t_map_event_iterator _iter, IEventCallback* callback)
    {
        if_return(id <= 0 || nullptr_t == callback || this->event_pool_.end() == _iter, ret_args_error);
        if (index < 0 || id != this->event_ready_list_[index]._id)
        {
            _event_ready_item _item = {id, _iter, 0, 0, callback};
            _item._read = (ready & event_read)? 1: 0;
            _item._write = (ready & event_write)? 1: 0;
            this->event_ready_list_.push_back(_item);
            index = (int)this->event_ready_list_.size() -1;
        }
        else
        {
            _event_ready_item& _item = this->event_ready_list_[index];
            _item._iter = _iter; _item._callback = callback;
            _item._read |= ((ready & event_read)? 1: 0);
            _item._write |= ((ready & event_write)? 1: 0);
        }
        return index;
    }

    ret EventPoll::del_ready_list(int index, int id, int ready)
    {
        if_return(index < 0 || index >= (int)this->event_ready_list_.size(), ret_args_error);

        _event_ready_item& _item = this->event_ready_list_[index];
        if_return(id != _item._id, ret_fail);
        
        _item._read = (ready & event_read)? 0: 1;
        _item._write = (ready & event_write)? 0: 1;
        if (0 == _item._write && 0 == _item._read && !this->ready_list_protected_)
        {
            int _len = (int)this->event_ready_list_.size() -1;
            if (index < _len)
            {
                this->event_ready_list_[index]._iter->second._index = -1;
                this->event_ready_list_[_len]._iter->second._index = index;
                this->event_ready_list_[index] = this->event_ready_list_[_len];
            }
            else
            {
                this->event_ready_list_[_len]._iter->second._index = -1;
            }

            this->event_ready_list_[_len]._callback = nullptr_t;
            this->event_ready_list_.resize(_len);
        }
        return ret_success;
    }
}