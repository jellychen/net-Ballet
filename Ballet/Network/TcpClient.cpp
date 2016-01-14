#include "TcpClient.h"

#define timer_tcp_connection_timeout            0
#define timer_tcp_heartbeat                     1
#define timer_tcp_check_connection              2

namespace network
{
    TcpClient::TcpClient()
        :event_callback_key_(this),timer_callback_key_(this)
    {
        this->fd_ = 0;
        this->max_read_size_ = 10*1024*1024;
        this->max_write_size_ = 10*1024*1024;
        this->use_ipv6_ = false;
        this->heartbeat_time_ = 0;
        this->set_heartbeat_ = false;
        this->set_time_out_ = false;
        this->time_out_ = 100000;
        this->last_read_time_ = 0;
        this->laset_write_time_ = 0;
        this->connection_status_ = status_none;
        this->want_read_buffer_size_ = -1;
        this->timer_callback_key_->hook_on_time(&__self::on_time);
        this->event_callback_key_->hook_notify_read(&__self::on_read);
        this->event_callback_key_->hook_notify_write(&__self::on_write);
    }

    TcpClient::~TcpClient()
    {
        if (this->fd_ > 0)
        {
            if (this->poll_)
            {
                this->poll_->del_event_listener(this->fd_, event_read|event_write);
            }
            ::close(this->fd_);
            this->fd_ = 0;
        }
    }

    bool TcpClient::internal_init()
    {
        this->client_self_ = __get_i_entity<ITcpClient>();
        this->connection_self_ = __get_i_entity<ITcpConnection>();
        return nullptr_t != client_self_ && nullptr_t != connection_self_;
    }

    ret TcpClient::init()
    {
        return ret_success;
    }

    ret TcpClient::connect(const char* _addr, int _port, int _timeout)
    {
        if_return(nullptr_t == _addr || 0 >= _port || !poll_, ret_args_error);
        if_return(connection_status_ != status_none && connection_status_ != status_closed, ret_fail);
        this->use_ipv6_ = false;

        struct sockaddr_in _addr_struct;
        memset(&_addr_struct, 0, sizeof(struct sockaddr_in));
        _addr_struct.sin_family = AF_INET;
        _addr_struct.sin_port = htons(_port);
        if_return(inet_aton(_addr, &_addr_struct.sin_addr) <= 0, ret_args_error);

        if (this->fd_ = (int)socket(AF_INET, SOCK_STREAM, IPPROTO_TCP), fd_ == -1)
        {
            perror("bind socket");
            this->fd_ = 0;
            return ret_fail;
        }

        int _opt_open_value = 1;
        setsockopt(this->fd_, SOL_SOCKET, SO_REUSEADDR, (const void*)&_opt_open_value, sizeof(int));
        setsockopt(this->fd_, SOL_SOCKET, SO_KEEPALIVE, (const void*)&_opt_open_value, sizeof(int));
        setsockopt(this->fd_, SOL_SOCKET, TCP_NODELAY, (const void*)&_opt_open_value, sizeof(int));
        fcntl(this->fd_, F_SETFL, fcntl(this->fd_, F_GETFL)|O_NONBLOCK);

        int _connect_ret = ::connect(this->fd_, (sockaddr*)&_addr_struct, sizeof(sockaddr));
        this->poll_->set_event_listener(this->fd_, event_read|event_write, this->event_callback_key_);

        if (0 == _connect_ret)
        {
            this->connection_status_ = status_establish;
            if (this->tcp_callback_)
            {
                this->tcp_callback_->on_connect(this->client_self_, true);
            }
        }
        else if(_connect_ret < 0 && EINPROGRESS == errno)
        {
            this->connection_status_ = status_connecting;    
            if (_timeout >= 0)
            {
                this->poll_->add_timer_once(timer_tcp_connection_timeout, this->timer_callback_key_, _timeout);
            }
        }
        return __bool_ret(0 <= _connect_ret);
    }

    ret TcpClient::connect_ipv6(const char* _addr, int _port, int _timeout)
    {
        if_return(nullptr_t == _addr || 0 >= _port || !poll_, ret_args_error);
        if_return(connection_status_ != status_none && connection_status_ != status_closed, ret_fail);
        this->use_ipv6_ = true;

        struct sockaddr_in6 _addr_struct;
        memset(&_addr_struct, 0, sizeof(struct sockaddr_in6));
        _addr_struct.sin6_family = AF_INET6;
        _addr_struct.sin6_port = htons(_port);
        if_return(inet_pton(AF_INET6, _addr, &_addr_struct.sin6_addr) <= 0, ret_args_error);

        if (this->fd_ = (int)socket(PF_INET6, SOCK_STREAM, IPPROTO_TCP), fd_ == -1)
        {
            perror("bind socket");
            this->fd_ = 0;
            return ret_fail;
        }

        int _opt_open_value = 1;
        setsockopt(this->fd_, SOL_SOCKET, SO_REUSEADDR, (const void*)&_opt_open_value, sizeof(int));
        setsockopt(this->fd_, SOL_SOCKET, SO_KEEPALIVE, (const void*)&_opt_open_value, sizeof(int));
        setsockopt(this->fd_, IPPROTO_TCP, TCP_NODELAY, (const void*)&_opt_open_value, sizeof(int));
        fcntl(this->fd_, F_SETFL, fcntl(this->fd_, F_GETFL)|O_NONBLOCK);

        int _connect_ret = ::connect(this->fd_, (sockaddr*)&_addr_struct, sizeof(sockaddr));
        this->poll_->set_event_listener(this->fd_, event_read|event_write, this->event_callback_key_);

        if (0 == _connect_ret)
        {
            this->connection_status_ = status_establish;
            if (this->tcp_callback_)
            {
                this->tcp_callback_->on_connect(this->client_self_, true);
            }
        }
        else if(_connect_ret < 0 && EINPROGRESS == errno)
        {
            this->connection_status_ = status_connecting;
            if (_timeout >= 0)
            {
                this->poll_->add_timer_once(timer_tcp_connection_timeout, this->timer_callback_key_, _timeout);
            }
        }
        return __bool_ret(0 <= _connect_ret);
    }

    ret TcpClient::cancel_connect()
    {
        if_return(this->connection_status_ != status_connecting, ret_fail);
        if (this->poll_)
        {
            this->poll_->del_timer(timer_tcp_connection_timeout, this->timer_callback_key_);
            this->poll_->del_event_listener(this->fd_, event_read|event_write);
        }

        ::close(this->fd_);
        this->fd_ = 0;
        this->connection_status_ = status_none;
        return ret_success;
    }

    ret TcpClient::follow_fd(int _fd, ITcpServer* _server)
    {
        return ret_no_support;
    }

    ret TcpClient::close(bool _right_now)
    {
        if (this->connection_status_ == status_connecting)
        {
            return this->cancel_connect();
        }
        else if (this->connection_status_ & (status_establish | status_closing))
        {
            if (_right_now || 0 >= this->write_buffer_.get_size())
            {
                this->on_set_close(this->fd_, this->poll_, true);
            }
            else
            {
                this->connection_status_ = status_closing;
            }
            return ret_success;
        }
        return ret_fail;
    }
    
    ret TcpClient::use_ipv6(bool* _use_ipv6)
    {
        if_return(nullptr_t == _use_ipv6, ret_args_error);
        *_use_ipv6 = this->use_ipv6_;
        return ret_success;
    }

    ret TcpClient::get_status(int* _status)
    {
        if_return(nullptr_t == _status, ret_args_error);
        *_status = this->connection_status_;
        return ret_success;
    }

    ret TcpClient::set_protocol(ITcpProtocol* _protocol)
    {
        this->protocol_ = _protocol;
        if (this->protocol_)
        {
            this->protocol_->callback(this->tcp_callback_);
        }
        return ret_success;
    }

    ret TcpClient::get_protocol(ITcpProtocol** _protocol)
    {
        if_return(nullptr_t == _protocol, ret_args_error);
        *_protocol = nullptr_t;

        if (this->protocol_)
        {
            return this->protocol_->convert(_type_id(ITcpProtocol), (void**)_protocol);
        }
        return ret_success;
    }

    ret TcpClient::set_max_read_buffer(int _max)
    {
        this->max_read_size_ = _max;
        return ret_success;
    }

    ret TcpClient::set_max_write_buffer(int _max)
    {
        this->max_write_size_ = _max;
        return ret_success;
    }
    
    ret TcpClient::get_max_read_buffer(int* _max)
    {
        if_return(nullptr_t == _max, ret_args_error);
        *_max = this->max_read_size_;
        return ret_success;
    }
    
    ret TcpClient::get_max_write_buffer(int* _max)
    {
        if_return(nullptr_t == _max, ret_args_error);
        *_max = this->max_write_size_;
        return ret_success;
    }
    
    ret TcpClient::set_timeout(int _timeout)
    {
        if (this->poll_ && this->set_time_out_)
        {
            this->set_time_out_ = false;
            this->poll_->del_timer(timer_tcp_check_connection, this->timer_callback_key_);
        }
        
        if_return(_timeout < 0, ret_success);

        if (this->connection_status_ == status_connecting)
        {
            this->time_out_ = _timeout;
            if_return(nullptr_t == this->poll_, ret_fail);

            this->set_time_out_ = true;
            this->poll_->add_timer_once(timer_tcp_check_connection, this->timer_callback_key_, _timeout);
        }
        else
        {
            this->time_out_ = _timeout;
        }
        return ret_success;
    }
    
    ret TcpClient::get_timeout(int* _timeout)
    {
        if_return(nullptr_t == _timeout, ret_args_error);
        *_timeout = this->time_out_ > 0;
        return ret_success;
    }
    
    ret TcpClient::set_callback(ITcpCallback* _callback)
    {
        this->tcp_callback_ = _callback;
        if (this->protocol_)
        {
            this->protocol_->callback(this->tcp_callback_);
        }
        return ret_success;
    }
    
    ret TcpClient::get_callback(ITcpCallback** _callback)
    {
        if_return(nullptr_t == _callback, ret_args_error);
        *_callback = nullptr_t;
        if (nullptr_t != this->tcp_callback_)
        {
            return this->tcp_callback_->convert(_type_id(ITcpCallback), (void**)_callback);
        }
        return ret_success;
    }

    ret TcpClient::set_heartbeat(bool _heartbeat, int _time)
    {
        if (this->poll_ && this->set_heartbeat_)
        {
            this->set_heartbeat_ = false;
            this->heartbeat_time_ = 0;
            this->poll_->del_timer(timer_tcp_heartbeat, this->timer_callback_key_);
        }

        if_return(_time < 0, ret_success);

        if (this->connection_status_ == status_connecting)
        {
            this->heartbeat_time_ = _time;
            if_return(nullptr_t == this->poll_, ret_fail);

            this->set_heartbeat_ = true;
            
            int _wait_time = _time;

            if (this->laset_write_time_  > 0)
            {
                _wait_time = this->get_current_time() - this->laset_write_time_;
            }

            if (_wait_time < 0) 
            {
                _wait_time = 0;
            }
            else if (_wait_time > _time)
            {
                _wait_time = _time;
            }
            this->poll_->add_timer_once(timer_tcp_heartbeat, this->timer_callback_key_, _wait_time);
        }
        else
        {
            this->heartbeat_time_ = _time;
        }
        return ret_success;
    }

    ret TcpClient::get_heartbeat(bool* _heartbeat)
    {
        if_return(nullptr_t == _heartbeat, ret_args_error);
        *_heartbeat = this->heartbeat_time_ > 0;
        return ret_success;
    }
    
    ret TcpClient::wait_for_event(IEventPoll* _poll)
    {
        if_return(nullptr_t == _poll, ret_args_error);
        this->poll_ = _poll;
        return ret_success;
    }

    ret TcpClient::send_buffer(const byte* _buffer, int _size)
    {
        if_return(nullptr_t == _buffer || 0 >= _size, ret_fail);
        if_return(status_establish != this->connection_status_ || !protocol_, ret_fail);
        return this->protocol_->encode(_buffer, _size, this->connection_self_);
    }

    ret TcpClient::send_raw_buffer(const byte* _buffer, int _size)
    {
        if_return(nullptr_t == _buffer || 0 >= _size, ret_fail);
        if_return(status_establish != this->connection_status_, ret_fail);
        if_return(_size + (int)this->write_buffer_.get_size() > max_write_size_, error_write_exceed);

        if (this->write_buffer_.get_size() <= 0)
        {
            int _write_size = ::write(this->fd_, _buffer, _size);

            if (_write_size < _size)
            {
                if (0 == _write_size || (_write_size == -1 && errno != EAGAIN))
                {
                    return this->on_set_close(this->fd_, this->poll_, false);
                }
                else
                {
                    if (-1 == _write_size)
                    {
                        this->write_buffer_.append_buffer(_buffer, _size);
                    }
                    else
                    {
                        this->write_buffer_.append_buffer(_buffer + _write_size, _size - _write_size);
                    }
                }
            }
            else if (this->connection_status_ == status_closing)
            {
                this->on_set_close(this->fd_, this->poll_, true);
            }
        }
        else
        {
            this->write_buffer_.append_buffer(_buffer, _size);
        }
        return ret_success;
    }

    ret TcpClient::set_delay_delete(bool)
    {
        return ret_no_support;
    }

    ret TcpClient::get_delay_delete(bool*)
    {
        return ret_no_support;
    }

    ret TcpClient::get_local_info(std::string* _addr, int* _port)
    {
        return ret_success;
    }

    ret TcpClient::get_peer_info(std::string* _addr, int* _port)
    {
        return ret_success;
    }

    ret TcpClient::on_read(int fd, IEventPoll* _poll)
    {
        const static int _max_read_buffer = 10240;
        byte _buffer[_max_read_buffer] = {0};
        int _read_size = ::read(fd, _buffer, _max_read_buffer);

        if (0 == _read_size || (-1 == _read_size && EAGAIN != errno))
        {
            this->on_set_close(fd, _poll, false);
            return callback_close;
        }

        if_return(!tcp_callback_ || !protocol_, ret_fail);

        if (_read_size > 0)
        {
            this->last_read_time_ = this->get_current_time();
            if (_read_size + (int)this->read_buffer_.get_size() > max_read_size_)
            {
                this->on_set_close(fd, _poll, true);
                this->tcp_callback_->on_error(this->connection_self_, error_read_exceed);
                return callback_fail;
            }
        }
        this->read_buffer_.append_buffer(_buffer, _read_size);

        while (true)
        {
            const byte* _buffer = nullptr_t;
            size_t _size = this->read_buffer_.get_buffer(&_buffer);
            if (-1 == this->want_read_buffer_size_)
            {
                int _want_size = -1; int _input_info = 0;
                this->protocol_->input(_buffer, (int)_size, &_want_size, &_input_info);
                if (tcp_protocol_error == _input_info)
                {
                    this->on_set_close(fd, _poll, true);
                    this->want_read_buffer_size_ = -1;
                    this->tcp_callback_->on_error(this->connection_self_, error_input);
                    return callback_fail;
                }
                else if (tcp_protocol_success == _input_info)
                {
                    this->want_read_buffer_size_ = _want_size;
                }
                else if (tcp_protocol_not_enough == _input_info)
                {
                    this->want_read_buffer_size_ = -1;
                }

                if_break(this->want_read_buffer_size_ <= 0);
            }
            
            if (this->want_read_buffer_size_ > 0)
            {
                if_break((int)this->read_buffer_.get_size() < this->want_read_buffer_size_);
                if (!__success(this->protocol_->decode(_buffer, want_read_buffer_size_, connection_self_)))
                {
                    this->on_set_close(fd, _poll, true);
                    this->tcp_callback_->on_error(this->connection_self_, error_decode);
                    return callback_close;
                }
                
                this->read_buffer_.consume_buffer(this->want_read_buffer_size_);
                this->want_read_buffer_size_ = -1;
                if_break(0 >= this->read_buffer_.get_size());
            }
        }
        return callback_complete;
    }

    ret TcpClient::on_write(int fd, IEventPoll* _poll)
    {
        if (this->connection_status_ == status_connecting)
        {
            int _error = 0;
            socklen_t len = sizeof(_error);
            if(::getsockopt(this->fd_, SOL_SOCKET, SO_ERROR, &_error, &len) != 0 || 0 != _error)
            {
                this->connection_status_ = status_none;

                if (this->poll_)
                {
                    this->poll_->del_event_listener(this->fd_, event_read|event_write);
                    this->poll_->del_timer(timer_tcp_connection_timeout, this->timer_callback_key_);
                }

                if (this->tcp_callback_)
                {
                    this->tcp_callback_->on_connect(this->connection_self_, false);
                }

                ::close(this->fd_);
                this->fd_ = 0;
            }
            else if (this->tcp_callback_)
            {
                this->connection_status_ = status_establish;
                this->tcp_callback_->on_connect(this->connection_self_, true);

                if (this->poll_)
                {
                    this->poll_->del_timer(timer_tcp_connection_timeout, this->timer_callback_key_);

                    if (this->time_out_ > 0)
                    {
                        this->set_time_out_ = true;
                        this->poll_->add_timer_once(timer_tcp_check_connection, this->timer_callback_key_, this->time_out_);
                    }

                    if (this->heartbeat_time_ > 0)
                    {
                        this->set_heartbeat_ = true;
                        this->poll_->add_timer_once(timer_tcp_heartbeat, this->timer_callback_key_, this->heartbeat_time_);
                    }
                }
            }
        }
        else if (this->connection_status_ == status_establish)
        {
            if_return(0 >= this->fd_ || nullptr_t == this->poll_, ret_fail);
            if_return(0 == this->write_buffer_.get_size(), callback_complete);

            const byte* _buffer = nullptr_t; int _size = 0;
            this->write_buffer_.get_first_block(&_buffer, &_size);
            int _write_size = ::write(fd, _buffer, _size);
            if (_write_size > 0)
            {
                this->write_buffer_.consume_first_block_size(_write_size);
                if (_write_size == _size)
                {
                    if (0 == this->write_buffer_.get_size())
                    {
                        if (status_closing == this->connection_status_)
                        {
                            this->on_set_close(fd, _poll, true);
                            return callback_close;
                        }
                        return callback_complete;
                    }
                    return callback_continue;
                }
                return callback_complete;
            }
            else if (0 == _write_size || (-1 == _write_size && errno != EAGAIN))
            {
                this->on_set_close(fd, _poll, false);
                return callback_close;
            }
            else if (-1 == _write_size)
            {
                return callback_again;
            }
        }
        return ret_fail;
    }

    ret TcpClient::on_time(int id, IEventPoll* _poll)
    {
        if (timer_tcp_connection_timeout == id)
        {
            if_return(this->connection_status_ != status_connecting, ret_fail);

            int _error = 0;
            socklen_t len = sizeof(_error);
            if (::getsockopt(this->fd_, SOL_SOCKET, SO_ERROR, &_error, &len) != 0 || 0 != _error)
            {
                this->connection_status_ = status_none;

                if (this->poll_)
                {
                    this->poll_->del_event_listener(this->fd_, event_read|event_write);
                }

                if (this->tcp_callback_)
                {
                    this->tcp_callback_->on_connect(this->connection_self_, false);
                }

                ::close(this->fd_);
                this->fd_ = 0;
            }
            else if (this->tcp_callback_)
            {
                this->connection_status_ = status_establish;
                this->tcp_callback_->on_connect(this->connection_self_, true);
            }
        }
        else if (timer_tcp_heartbeat == id)
        {
            if (this->tcp_callback_)
            {
                this->tcp_callback_->on_heartbeat(this->connection_self_);
            }
        }
        else if (timer_tcp_check_connection == id)
        {
            this->on_set_close(this->fd_, _poll, false);
        }
        return ret_success;
    }

    ret TcpClient::on_set_close(int fd, IEventPoll* _poll, bool _accord)
    {
        this->connection_status_ = status_closed;

        if (this->poll_ && this->set_time_out_)
        {
            this->set_time_out_ = false;
            this->time_out_ = 100000;
            this->poll_->del_timer(timer_tcp_check_connection, this->timer_callback_key_);
        }

        if (this->poll_ && this->set_heartbeat_)
        {
            this->set_heartbeat_ = false;
            this->heartbeat_time_ = 0;
            this->poll_->del_timer(timer_tcp_heartbeat, this->timer_callback_key_);
        }

        if (this->tcp_callback_)
        {
            this->tcp_callback_->on_close(this->connection_self_, _accord);
        }

        if (this->fd_ > 0)
        {
            if (this->poll_)
            {
                this->poll_->del_event_listener(this->fd_, event_read|event_write);
            }
            ::close(this->fd_);
            this->fd_ = 0;
        }
        return ret_success;
    }
}
