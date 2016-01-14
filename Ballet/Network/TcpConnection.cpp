#include "TcpConnection.h"

namespace network
{
	TcpConnection::TcpConnection()
		:timer_callback_key_(this),event_callback_key_(this)
	{
		this->fd_ = 0;
		this->connection_status_ = status_none;
		this->want_read_buffer_size_ = -1;
		this->last_read_time_ = 0;
		this->delay_delete_ = true;
		this->set_time_out_ = false;
		this->wait_for_write_ = false;
		this->timer_callback_key_->hook_on_time(&TcpConnection::on_time);
		this->event_callback_key_->hook_notify_read(&TcpConnection::on_read);
		this->event_callback_key_->hook_notify_write(&TcpConnection::on_write);
	}

	TcpConnection::~TcpConnection()
	{
		if (this->fd_ > 0)
		{
			if (this->poll_)
			{
				int _flag = event_read;
				if (this->wait_for_write_)
				{
					_flag |= event_write;
				}
				this->poll_->del_event_listener(this->fd_, _flag);
			}
			::close(this->fd_);
			this->fd_ = 0;
		}
	}

	bool TcpConnection::internal_init()
	{
		this->connection_self_ = __get_i_entity<ITcpConnection>();
		return nullptr_t != connection_self_;
	}

	ret TcpConnection::init()
	{
		return ret_success;
	}

	ret TcpConnection::close(bool _right_now)
	{
		if_return(this->connection_status_ & (status_establish | status_closing), ret_fail);
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

	ret TcpConnection::follow_fd(int _fd, ITcpServer* _server)
	{
		if_return(_fd <= 0 || nullptr_t == _server, ret_args_error);

		this->fd_ = _fd;
		this->tcp_server_ = _server;

		if_return(!tcp_server_, ret_fail);
		int _opt_open_value = 1;
		setsockopt(this->fd_, IPPROTO_TCP, SO_REUSEADDR, (const void*)&_opt_open_value, sizeof(int));
		setsockopt(this->fd_, IPPROTO_TCP, SO_KEEPALIVE, (const void*)&_opt_open_value, sizeof(int));
		setsockopt(this->fd_, IPPROTO_TCP, TCP_NODELAY, (const void*)&_opt_open_value, sizeof(int));
		fcntl(this->fd_, F_SETFL, fcntl(this->fd_, F_GETFL)|O_NONBLOCK);
		this->connection_status_ = status_establish;

		__sp<ITcpCallback> _callback;
		if (__success(this->tcp_server_->get_callback(&_callback)) && _callback)
		{
			_callback->on_connect(this->connection_self_, true);
		}
		return ret_success;
	}

	ret TcpConnection::use_ipv6(bool* _ipv6)
	{
		if_return(nullptr_t == _ipv6, ret_args_error);
		if_return(!tcp_server_, ret_fail);
		return this->tcp_server_->use_ipv6(_ipv6);
	}

	ret TcpConnection::get_status(int* _status)
	{
		if_return(nullptr_t == _status, ret_args_error);
		*_status = this->connection_status_;
		return ret_success;
	}

	ret TcpConnection::send_buffer(const byte* _buffer, int _size)
	{
		if_return(nullptr_t == _buffer || 0 >= _size, ret_fail);
		if_return(status_establish != this->connection_status_ || !tcp_server_, ret_fail);

		__sp<ITcpProtocol> _protocol;
		if_return(!__success(this->tcp_server_->get_protocol(&_protocol)) || !_protocol, ret_fail);
		return _protocol->encode(_buffer, _size, this->connection_self_);
	}
		
	ret TcpConnection::send_raw_buffer(const byte* _buffer, int _size)
	{
		if_return(nullptr_t == _buffer || 0 >= _size, ret_fail);
		if_return(status_establish != this->connection_status_ || !tcp_server_, ret_fail);
		int _max_write_size = 0;
		if_return(!__success(this->tcp_server_->get_max_write_buffer(&_max_write_size)), ret_fail);
		if_return(_size + (int)this->write_buffer_.get_size() > _max_write_size, error_write_exceed);

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
					if (false == this->wait_for_write_ && this->poll_)
					{
						this->wait_for_write_ = __success(this->poll_
							->set_event_listener(fd_, event_write, event_callback_key_));
					}

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
	
	ret TcpConnection::set_delay_delete(bool _delay)
	{
		this->delay_delete_ = _delay;
		return ret_success;
	}
	
	ret TcpConnection::get_delay_delete(bool* _delay)
	{
		if_return(nullptr_t == _delay, ret_args_error);
		*_delay = this->delay_delete_;
		return ret_success;
	}

	ret TcpConnection::get_local_info(std::string* _addr, int* _port)
	{
		if_return(nullptr_t == _addr && nullptr_t == _port, ret_args_error);
		if_return(0 >= this->fd_ || !this->tcp_server_, ret_fail);
		
		bool _use_ipv6 = false;
		this->tcp_server_->use_ipv6(&_use_ipv6);
		if (_use_ipv6)
		{
			struct sockaddr_in6 _sockaddr;
			memset(&_sockaddr, 0, sizeof(struct sockaddr_in6));
			socklen_t _addr_len = sizeof(struct sockaddr_in6);
			if_return(0 != getsockname(this->fd_, (sockaddr*)&_sockaddr, &_addr_len), ret_fail);

			if (nullptr_t != _port)
			{
				*_port = (int)ntohs(_sockaddr.sin6_port);
			}

			if (nullptr_t != _addr)
			{
				char _ip_string[40] = {0}; uint8_t* _ia = _sockaddr.sin6_addr.s6_addr;
            	sprintf(_ip_string,
	                "%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x",
	                _ia[0], _ia[1], _ia[2], _ia[3], _ia[4], _ia[5], _ia[6], _ia[7], _ia[8],
	                _ia[9], _ia[10], _ia[11], _ia[12], _ia[13], _ia[14], _ia[15]);
            	*_addr = _ip_string;
			}
		}
		else
		{
			struct sockaddr_in _sockaddr;
			memset(&_sockaddr, 0, sizeof(sockaddr_in));
			socklen_t _addr_len = sizeof(sockaddr_in);
			if_return(0 != getsockname(this->fd_, (sockaddr*)&_sockaddr, &_addr_len), ret_fail);

			if (nullptr_t != _port)
			{
				*_port = (int)ntohs(_sockaddr.sin_port);
			}

			if (nullptr_t != _addr)
			{
				*_addr = inet_ntoa(_sockaddr.sin_addr);
			}
		}
		return ret_success;
	}

    ret TcpConnection::get_peer_info(std::string* _addr, int* _port)
    {
    	if_return(nullptr_t == _addr && nullptr_t == _port, ret_args_error);
    	if_return(status_establish != this->connection_status_ 
    		&& status_closing != this->connection_status_, ret_fail);

    	bool _use_ipv6 = false;
		this->tcp_server_->use_ipv6(&_use_ipv6);
		if (_use_ipv6)
		{
			struct sockaddr_in6 _sockaddr;
			memset(&_sockaddr, 0, sizeof(struct sockaddr_in6));
			socklen_t _addr_len = sizeof(struct sockaddr_in6);
			if_return(0 != ::getpeername(this->fd_, (sockaddr*)&_sockaddr, &_addr_len), ret_fail);

			if (nullptr_t != _port)
			{
				*_port = (int)ntohs(_sockaddr.sin6_port);
			}

			if (nullptr_t != _addr)
			{
				char _ip_string[40] = {0}; uint8_t* _ia = _sockaddr.sin6_addr.s6_addr;
            	sprintf(_ip_string,
	                "%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x",
	                _ia[0], _ia[1], _ia[2], _ia[3], _ia[4], _ia[5], _ia[6], _ia[7], _ia[8],
	                _ia[9], _ia[10], _ia[11], _ia[12], _ia[13], _ia[14], _ia[15]);
            	*_addr = _ip_string;
			}
		}
		else
		{
			struct sockaddr_in _sockaddr;
			memset(&_sockaddr, 0, sizeof(struct sockaddr_in));
			socklen_t _addr_len = sizeof(struct sockaddr_in);
			if_return(0 != ::getpeername(this->fd_, (sockaddr*)&_sockaddr, &_addr_len), ret_fail);

			if (nullptr_t != _port)
			{
				*_port = (int)ntohs(_sockaddr.sin_port);
			}

			if (nullptr_t != _addr)
			{
				*_addr = inet_ntoa(_sockaddr.sin_addr);
			}
		}
    	return ret_success;
    }
	
	ret TcpConnection::wait_for_event(IEventPoll* _poll)
	{
		if_return(nullptr_t == _poll || !tcp_server_, ret_args_error);
		this->poll_ = _poll;

		if (this->fd_ > 0 && this->poll_)
		{
			this->poll_->set_event_listener(fd_, event_read, event_callback_key_);
		}

		int _timeout = 0;
		if (__success(this->tcp_server_->get_timeout(&_timeout)) && _timeout > 0)
		{
			this->set_time_out_ = true;
			this->poll_->add_timer_once(0, timer_callback_key_, _timeout);
		}
		return ret_success;
	}

	ret TcpConnection::on_read(int fd, IEventPoll* _poll)
	{
		const static int _max_read_buffer = 10240;
		byte _buffer[_max_read_buffer] = {0};
		int _read_size = ::read(fd, _buffer, _max_read_buffer);

		if (0 == _read_size || (-1 == _read_size && EAGAIN != errno))
		{
			this->on_set_close(fd, _poll, false);
			return callback_close;
		}

		if (_read_size > 0)
		{
			if_return(!tcp_server_, ret_fail);
			int _max_read_size = 0;
			if_return(!__success(this->tcp_server_->get_max_read_buffer(&_max_read_size)), ret_fail);
			__sp<ITcpProtocol> _protocol;
			if_return(!__success(this->tcp_server_->get_protocol(&_protocol)) || !_protocol, ret_fail);
			__sp<ITcpCallback> _callback;
			if_return(!__success(this->tcp_server_->get_callback(&_callback)) || !_callback, ret_fail);

			this->last_read_time_ = this->get_current_time();
			if (_read_size + (int)this->read_buffer_.get_size() > _max_read_size)
			{
				this->on_set_close(fd, _poll, true);
				_callback->on_error(this->connection_self_, error_read_exceed);
				return callback_fail;
			}
			this->read_buffer_.append_buffer(_buffer, _read_size);

			while (true)
			{
				const byte* _buffer = nullptr_t;
				size_t _size = this->read_buffer_.get_buffer(&_buffer);
				if (-1 == this->want_read_buffer_size_)
				{
					int _want_size = -1; int _input_info = 0;
					_protocol->input(_buffer, (int)_size, &_want_size, &_input_info);
					
					if (tcp_protocol_error == _input_info)
					{
						this->on_set_close(fd, _poll, true);
						this->want_read_buffer_size_ = -1;
						_callback->on_error(this->connection_self_, error_input);
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
					if (!__success(_protocol->decode(_buffer, want_read_buffer_size_, connection_self_)))
					{
						this->on_set_close(fd, _poll, true);
						_callback->on_error(this->connection_self_, error_decode);
						return callback_close;
					}
					this->read_buffer_.consume_buffer(this->want_read_buffer_size_);
					this->want_read_buffer_size_ = -1;
					if_break(0 >= this->read_buffer_.get_size());
				}
			}
			return callback_continue;
		}
		return callback_complete;
	}

	ret TcpConnection::on_write(int fd, IEventPoll* _poll)
	{
#if 0
		if_return(0 >= this->fd_ || nullptr_t == this->poll_, ret_fail);
		if_return(0 == this->write_buffer_.get_size(), callback_complete);

		const byte* _buffer = nullptr_t; int _size = 0;
		this->write_buffer_.get_first_block(&_buffer, &_size);
		int _write_size = ::write(fd, _buffer, _size);
		if (_write_size > 0)
		{
			this->write_buffer_.consume_first_block_size(_write_size);
			if_return(_write_size < _size, callback_complete);
		}
		else if (0 == _write_size || (-1 == _write_size && errno != EAGAIN))
		{
			this->on_set_close(fd, _poll, false);
			return callback_close;
		}

		if (0 == this->write_buffer_.get_size())
		{
			if (status_closing == this->connection_status_)
			{
				this->on_set_close(fd, _poll, true);
				return callback_close;
			}
			else
			{
				return callback_complete;
			}
		}
		return callback_continue;
#endif//0

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
	
	ret TcpConnection::on_time(int id, IEventPoll* _poll)
	{
		if_return(!tcp_server_, ret_fail);

		int _timeout = 0;
		if_return(!__success(this->tcp_server_->get_timeout(&_timeout)), ret_fail);
		int64_t _interval_time = this->get_current_time() - this->last_read_time_;
		if (_interval_time >= _timeout)
		{
			return this->on_set_close(this->fd_, this->poll_, true);
		}
		else if (this->poll_)
		{
			this->poll_->add_timer_once(0, timer_callback_key_, _timeout - _interval_time);
		}
		return ret_fail;
	}
	
	ret TcpConnection::on_set_close(int fd, IEventPoll* _poll, bool _accord)
	{
		this->connection_status_ = status_closed;

		if (this->poll_ && this->set_time_out_)
		{
			this->set_time_out_ = false;
			this->poll_->del_timer(0, this->timer_callback_key_);
		}

		__sp<ITcpCallback> _callback;
		if (this->tcp_server_ && __success(this->tcp_server_->get_callback(&_callback)) && _callback)
		{
			_callback->on_close(this->connection_self_, _accord);
		}

		if (this->poll_ && this->delay_delete_)
		{
			this->poll_->add_delay_release(__get_i_entity<IObject>());
		}

		if (this->tcp_server_)
		{
			this->tcp_server_->erase(this->fd_);
		}

		if (this->fd_ > 0)
		{
			if (this->poll_)
			{
				int _flag = event_read;
				if (this->wait_for_write_)
				{
					_flag |= event_write;
				}
				this->poll_->del_event_listener(this->fd_, _flag);
			}
			::close(this->fd_);
			this->fd_ = 0;
		}
		return ret_success;
	}
}