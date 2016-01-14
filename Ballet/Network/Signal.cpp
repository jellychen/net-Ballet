#include "Signal.h"

namespace network
{
    void __sigroutine(int _signal)
    {
        __get_instance<Signal>().dispath(_signal);
    }

    Signal::Signal():_callback_key(this)
    {
        this->_fd[0] = 0;
        this->_fd[1] = 0;
        this->_callback_key->hook_notify_read(&Signal::on_read);
    }

    Signal::~Signal()
    {
        if (this->_fd[0] > 0)
        {
            ::close(this->_fd[0]);
            this->_fd[0] = 0;
        }

        if (this->_fd[1] > 0)
        {
            ::close(this->_fd[1]);
            this->_fd[1] = 0;
        }
    }

    bool Signal::init(IEventPoll* poll)
    {
        if_return(poll == nullptr_t, false);
        if_return(::pipe(_fd) < 0, false);
        fcntl(this->_fd[0], F_SETFL, fcntl(this->_fd[0], F_GETFL)|O_NONBLOCK);
        return __success(poll->set_event_listener(this->_fd[0], event_read, _callback_key));
    }

    bool Signal::set_signal(int _signal, ISignalCallback* callback)
    {
        if_return(callback == nullptr_t, ret_args_error);
        signal(_signal, __sigroutine);
        this->_pool[_signal] = callback;
        return true;
    }

    bool Signal::del_signal(int _signal)
    {
        signal(_signal, SIG_IGN);
        _t_map_signal_pool::iterator _it = this->_pool.find((int)(_signal));
        if (this->_pool.end() != _it)
        {
            this->_pool.erase(_it);
        }
        return true;
    }

    bool Signal::dispath(int _signal)
    {
        if_return(0 >= this->_fd[1], false);
        uint8_t _data = (uint8_t)_signal;
        return sizeof(uint8_t) == ::write(this->_fd[1], (void*)&_data, sizeof(uint8_t));
    }

    ret Signal::on_read(int id, IEventPoll* poll)
    {
        if_return(0 >= id, ret_args_error);

        uint8_t _buffer[1024] = {0};
        int _read_size = ::read(id, _buffer, 1024);
        if (_read_size > 0)
        {
            for (int i = 0; i < _read_size; ++i)
            {
                int _signal = (int)(_buffer[i]);
                _t_map_signal_pool::iterator _it = this->_pool.find(_signal);
                if (this->_pool.end() != _it)
                {
                    _it->second->on_signal((int)(_signal), poll);
                }
            }
            return callback_continue;
        }
        return callback_complete;
    }
}