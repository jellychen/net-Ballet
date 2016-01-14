#include "HttpConnection.h"

namespace network
{
    // 当解析到URL时的回调
    int g_on_url_cb(http_parser *parser, const char* _buffer, size_t len)
    {
        if_return(parser == nullptr_t, 0);
        HttpConnection* _this = (HttpConnection*)parser->data;
        return _this->on_url_cb(parser, _buffer, len);
    }

    // 当解析到头部字段名称的回调
    int g_on_header_field_cb(http_parser *parser, const char* _buffer, size_t len)
    {
        if_return(parser == nullptr_t, 0);
        HttpConnection* _this = (HttpConnection*)parser->data;
        return _this->on_header_field_cb(parser, _buffer, len);
    }

    // 当解析到头部字段值的回调
    int g_on_header_value_cb(http_parser *parser, const char* _buffer, size_t len)
    {
        if_return(parser == nullptr_t, 0);
        HttpConnection* _this = (HttpConnection*)parser->data;
        return _this->on_header_value_cb(parser, _buffer, len);
    }

    // 当解析到HTTP消息主体的回调
    int g_on_body_cb(http_parser *parser, const char* _buffer, size_t len)
    {
        if_return(parser == nullptr_t, 0);
        HttpConnection* _this = (HttpConnection*)parser->data;
        return _this->on_body_cb(parser, _buffer, len);
    }

    // 通知回调：说明开始解析HTTP消息
    int g_on_message_begin(http_parser *parser)
    {
        if_return(parser == nullptr_t, 0);
        HttpConnection* _this = (HttpConnection*)parser->data;
        return _this->on_message_begin(parser);
    }

    // 通知回调：说明消息解析完毕
    int g_on_message_complete(http_parser *parser)
    {
        if_return(parser == nullptr_t, 0);
        HttpConnection* _this = (HttpConnection*)parser->data;
        return _this->on_message_complete(parser);
    }

    // 通知回调：说明HTTP报文头部解析完毕
    int g_on_headers_complete(http_parser *parser)
    {
        if_return(parser == nullptr_t, 0);
        HttpConnection* _this = (HttpConnection*)parser->data;
        return _this->on_headers_complete(parser);
    }

    HttpConnection::HttpConnection()
        :timer_callback_key_(this),event_callback_key_(this)
    {
        this->fd_ = 0;
        this->connection_status_ = status_none;
        this->last_read_time_ = 0;
        this->delay_delete_ = true;
        this->set_time_out_ = false;
        this->wait_for_write_ = false;

        this->request_header_field_keying_ = true;
        this->request_body_size_ = 0;
        this->request_is_keep_alive_ = false;
        http_parser_init(&request_parser_, HTTP_REQUEST);
        request_parser_.data = (void*)(this);
        memset((&request_setting_), 0, sizeof(http_parser_settings));
        request_setting_.on_message_begin = g_on_message_begin;
        request_setting_.on_headers_complete = g_on_headers_complete;
        request_setting_.on_message_complete = g_on_message_complete;
        request_setting_.on_header_field = g_on_header_field_cb;
        request_setting_.on_header_value = g_on_header_value_cb;
        request_setting_.on_url = g_on_url_cb;
        request_setting_.on_body = g_on_body_cb;

        this->respond_keep_alive_ = false;
        this->respond_chunked_ = false;
        this->respond_http_head_buffer_ = "";

        this->timer_callback_key_->hook_on_time(&HttpConnection::on_time);
        this->event_callback_key_->hook_notify_read(&HttpConnection::on_read);
        this->event_callback_key_->hook_notify_write(&HttpConnection::on_write);
    }

    HttpConnection::~HttpConnection()
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
    
    bool HttpConnection::internal_init()
    {
        this->connection_self_ = __get_i_entity<IHttpConnection>();
        return nullptr_t != connection_self_;
    }

    ret HttpConnection::init()
    {
        return ret_success;
    }

    ret HttpConnection::close(bool _right_now)
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
    
    ret HttpConnection::follow_fd(int _fd, ITcpServer* _server)
    {
        if_return(_fd <= 0 || nullptr_t == _server, ret_args_error);

        this->fd_ = _fd;
        __sp<IHttpServer> _http_server;
        _server->convert(_type_id(IHttpServer), (void**)&_http_server);
        if_return(!_http_server, ret_fail);
        this->http_server_ = _http_server;
        if_return(!this->http_server_, ret_fail);

        int _opt_open_value = 1;
        setsockopt(this->fd_, IPPROTO_TCP, SO_REUSEADDR, (const void*)&_opt_open_value, sizeof(int));
        setsockopt(this->fd_, IPPROTO_TCP, SO_KEEPALIVE, (const void*)&_opt_open_value, sizeof(int));
        setsockopt(this->fd_, IPPROTO_TCP, TCP_NODELAY, (const void*)&_opt_open_value, sizeof(int));
        fcntl(this->fd_, F_SETFL, fcntl(this->fd_, F_GETFL)|O_NONBLOCK);
        this->connection_status_ = status_establish;

        __sp<IHttpCallback> _callback;
        if (__success(this->http_server_->get_http_callback(&_callback)) && _callback)
        {
            _callback->on_http_connect(this->connection_self_, true);
        }
        return ret_success;
    }

    ret HttpConnection::follow_http_fd(int _fd, IHttpServer* _server)
    {
        if_return(_fd <= 0 || nullptr_t == _server, ret_args_error);

        this->fd_ = _fd;
        this->http_server_ = _server;
        if_return(!http_server_, ret_fail);

        int _opt_open_value = 1;
        setsockopt(this->fd_, IPPROTO_TCP, SO_REUSEADDR, (const void*)&_opt_open_value, sizeof(int));
        setsockopt(this->fd_, IPPROTO_TCP, SO_KEEPALIVE, (const void*)&_opt_open_value, sizeof(int));
        setsockopt(this->fd_, IPPROTO_TCP, TCP_NODELAY, (const void*)&_opt_open_value, sizeof(int));
        fcntl(this->fd_, F_SETFL, fcntl(this->fd_, F_GETFL)|O_NONBLOCK);
        this->connection_status_ = status_establish;

        __sp<IHttpCallback> _callback;
        if (__success(this->http_server_->get_http_callback(&_callback)) && _callback)
        {
            _callback->on_http_connect(this->connection_self_, true);
        }
        return ret_success;
    }
    
    ret HttpConnection::use_ipv6(bool* _ipv6)
    {
        if_return(nullptr_t == _ipv6, ret_args_error);
        if_return(!http_server_, ret_fail);
        return this->http_server_->use_ipv6(_ipv6);
    }
    
    ret HttpConnection::get_status(int* _status)
    {
        if_return(nullptr_t == _status, ret_args_error);
        *_status = this->connection_status_;
        return ret_success;
    }
    
    ret HttpConnection::send_buffer(const byte*, int)
    {
        return ret_fail;
    }
    
    ret HttpConnection::send_raw_buffer(const byte* _buffer, int _size)
    {
        if_return(nullptr_t == _buffer || 0 >= _size, ret_fail);
        if_return(status_establish != this->connection_status_ || !http_server_, ret_fail);
        int _max_write_size = 0;
        if_return(!__success(this->http_server_->get_max_write_buffer(&_max_write_size)), ret_fail);
        if_return(_size + (int)this->write_buffer_.get_size() > _max_write_size, error_write_exceed);

        if (this->write_buffer_.get_size() <= 0)
        {
            int _write_size = ::write(this->fd_, _buffer, _size);
            
            if (_write_size < _size)
            {
                if (0 == _write_size || (_write_size == -1 && errno != EAGAIN))
                {
                    this->on_set_close(this->fd_, this->poll_, false);
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
    
    ret HttpConnection::set_delay_delete(bool _delay)
    {
        this->delay_delete_ = _delay;
        return ret_success;
    }
    
    ret HttpConnection::get_delay_delete(bool* _delay)
    {
        if_return(nullptr_t == _delay, ret_args_error);
        *_delay = this->delay_delete_;
        return ret_success;
    }

    ret HttpConnection::get_local_info(std::string* _addr, int* _port)
    {
        return ret_success;
    }

    ret HttpConnection::get_peer_info(std::string* _addr, int* _port)
    {
        return ret_success;
    }
    
    ret HttpConnection::wait_for_event(IEventPoll* _poll)
    {
        if_return(nullptr_t == _poll || !http_server_, ret_args_error);
        this->poll_ = _poll;

        if (this->fd_ > 0 && this->poll_)
        {
            this->poll_->set_event_listener(fd_, event_read, event_callback_key_);
        }

        int _timeout = 0;
        if (__success(this->http_server_->get_timeout(&_timeout)) && _timeout > 0)
        {
            this->set_time_out_ = true;
            this->poll_->add_timer_once(0, timer_callback_key_, _timeout);
        }
        return ret_success;
    }
    
    ret HttpConnection::get_http_method(int* _method)
    {
        if_return(nullptr_t == _method, ret_args_error);
        *_method = (int)this->request_parser_.method;
        return ret_success;
    }
    
    ret HttpConnection::get_http_version(int* _major, int* _minor)
    {
        if (nullptr_t != _major)
        {
            *_major = this->request_parser_.http_major;
        }

        if (nullptr_t != _minor)
        {
            *_minor = this->request_parser_.http_minor;
        }
        return ret_success;
    }
    
    ret HttpConnection::get_http_url(std::string* _url)
    {
        if_return(nullptr_t == _url, ret_args_error);
        *_url = this->request_url_;
        return ret_success;
    }
    
    ret HttpConnection::get_http_header_field(std::string& _key, std::string* _value)
    {
        if_return(nullptr_t == _value, ret_args_error);
        _t_map_pool::iterator _it = this->request_header_field_.find(_key);
        if_return(this->request_header_field_.end() == _it, ret_fail);
        *_value = _it->second;
        return ret_success;
    }
    
    ret HttpConnection::http_respond_begin()
    {
        this->respond_http_head_buffer_ = "HTTP/";
        return ret_success;
    }
    
    ret HttpConnection::http_respond_version(int _major, int _minor)
    {
        char _version_str[20];
        sprintf(_version_str, "%d", _major);
        this->respond_http_head_buffer_ += _version_str;
        this->respond_http_head_buffer_ += '.';
        sprintf(_version_str, "%d", _minor);
        this->respond_http_head_buffer_ += _version_str;
        this->respond_http_head_buffer_ += ' ';
        return ret_success;
    }
    
    ret HttpConnection::http_respond_keep_alive()
    {
        this->respond_keep_alive_ = true;
        this->respond_http_head_buffer_ += "Connection: keep-alive\r\n";
        return ret_success;
    }
    
    ret HttpConnection::http_respond_chunked()
    {
        this->respond_chunked_ = true;
        this->respond_http_head_buffer_ += "Transfer-Encoding: chunked\r\n";
        return ret_success;
    }
    
    ret HttpConnection::http_respond_status(int _status, const char* _info)
    {
        char _status_str[20];
        sprintf(_status_str, "%d", _status);
        this->respond_http_head_buffer_ += _status_str;
        this->respond_http_head_buffer_ += ' ';
        this->respond_http_head_buffer_ += _info;
        this->respond_http_head_buffer_ += "\r\n";
        return ret_success;
    }
    
    ret HttpConnection::http_respond_content_type(const char* _type)
    {
        if (nullptr_t != _type)
        {
            this->respond_http_head_buffer_ += "Content-type: ";
            this->respond_http_head_buffer_ += _type;
            this->respond_http_head_buffer_ += "\r\n";
        }
        return ret_success;
    }
    
    ret HttpConnection::http_respond_content_length(int _size)
    {
        this->respond_http_head_buffer_ += "Content-Length: ";
        char _content_length_str[20];
        sprintf(_content_length_str, "%d", _size);
        this->respond_http_head_buffer_ += _content_length_str;
        this->respond_http_head_buffer_ += "\r\n";
        return ret_success;
    }
    
    ret HttpConnection::http_respond_header(const char* _key, const char* _value)
    {
        if (nullptr_t != _key && nullptr_t != _value)
        {
            this->respond_http_head_buffer_ += _key;
            this->respond_http_head_buffer_ += ": ";
            this->respond_http_head_buffer_ += _value;
            this->respond_http_head_buffer_ += "\r\n";
        }
        return ret_success;
    }
    
    ret HttpConnection::http_respond_header_complete()
    {
        this->respond_http_head_buffer_ += "\r\n";
        ret _ret = this->send_raw_buffer((const byte*)this
            ->respond_http_head_buffer_.c_str(), (int)this->respond_http_head_buffer_.size());
        this->respond_http_head_buffer_.clear();
        return _ret;
    }
    
    ret HttpConnection::http_respond_body(const byte* _buffer, int _size)
    {
        if_return(true == respond_chunked_ || _buffer == nullptr_t || 0 == _size, ret_fail);
        return this->send_raw_buffer(_buffer, _size);
    }
    
    ret HttpConnection::http_respond_chunk(const byte* _buffer, int _size)
    {
        if_return(false == respond_chunked_, ret_fail);

        std::string _chunk_buffer;
        char _chunk_length_str[20] = {0};
        sprintf(_chunk_length_str, "\r\n%d\r\n", _size);
        _chunk_buffer += _chunk_length_str;

        if (_buffer && _size > 0)
        {
            _chunk_buffer.append((const char*)_buffer, _size);
        }
        return this->send_raw_buffer((const byte*)(_chunk_buffer.c_str()), (int)_chunk_buffer.size());
    }

    ret HttpConnection::http_respond_chunk_complete()
    {
        if_return(false == respond_chunked_, ret_fail);
        const char* _chunk_complete_str = "\r\n0\r\n";
        return this->send_raw_buffer((const byte*)(_chunk_complete_str), 5);
    }
    
    ret HttpConnection::http_respond_complete(bool _close)
    {
        if (this->connection_status_ == status_establish)
        {
            if (_close == true)
            {
                this->on_set_close(this->fd_, this->poll_, true);
            }
            else if (false == this->respond_keep_alive_ || false == this->request_is_keep_alive_)
            {
                if (0 >= this->write_buffer_.get_size())
                {
                    this->on_set_close(this->fd_, this->poll_, true);
                }
                else
                {
                    this->connection_status_ = status_closing;
                }
            }
        }

        this->respond_chunked_ = false;
        this->respond_keep_alive_ = false;
        return ret_success;
    }

    int HttpConnection::input_buffer(const char* _buffer, int _size)
    {
        return http_parser_execute(&request_parser_, &request_setting_, _buffer, _size);
    }

    int HttpConnection::on_url_cb(http_parser* parser, const char* _buffer, size_t len)
    {
        if (_buffer != nullptr_t && len > 0)
        {
            this->request_url_.append(_buffer, len);
        }
        return 0;
    }
    
    int HttpConnection::on_header_field_cb(http_parser* parser, const char* _buffer, size_t len)
    {
        this->on_set_http_field();
        if (this->request_header_field_keying_)
        {
            this->request_header_field_key_.append(_buffer, len);
        }
        return 0;
    }
    
    int HttpConnection::on_header_value_cb(http_parser* parser, const char* _buffer, size_t len)
    {
        this->request_header_field_keying_ = false;
        this->request_header_field_value_.append(_buffer, len);
        return 0;
    }
    
    int HttpConnection::on_body_cb(http_parser* parser, const char* _buffer, size_t len)
    {
        __sp<IHttpCallback> _callback;
        if (this->http_server_ && __success(this->http_server_->get_http_callback(&_callback)) && _callback)
        {
            _callback->on_http_body(this->connection_self_, (const byte*)_buffer, len);
        }
        this->request_body_size_ += len;
        return 0;
    }
    
    int HttpConnection::on_message_begin(http_parser* parser)
    {
        __sp<IHttpCallback> _callback;
        if (this->http_server_ && __success(this->http_server_->get_http_callback(&_callback)) && _callback)
        {
            _callback->on_http_begin(this->connection_self_);
        }
        return 0;
    }
    
    int HttpConnection::on_message_complete(http_parser* parser)
    {
        __sp<IHttpCallback> _callback;
        if (this->http_server_ && __success(this->http_server_->get_http_callback(&_callback)) && _callback)
        {
            _callback->on_http_complete(this->connection_self_);
        }

        this->request_url_.clear();
        this->request_body_size_ = 0;
        this->request_is_keep_alive_ = false;
        this->request_header_field_keying_ = true;
        this->request_header_field_key_.clear();
        this->request_header_field_value_.clear();
        this->request_header_field_.clear();
        return 0;
    }
    
    int HttpConnection::on_headers_complete(http_parser* parser)
    {
        this->on_set_http_field();
        __sp<IHttpCallback> _callback;
        if (this->http_server_ && __success(this->http_server_->get_http_callback(&_callback)) && _callback)
        {
            _callback->on_http_header(this->connection_self_);
        }
        return 0;
    }
    
    int HttpConnection::on_set_http_field()
    {
        if (this->request_header_field_key_.length() > 0)
        {
            if (this->request_header_field_key_ == http_header_field::_connection 
                    && this->request_header_field_value_ == http_header_field::_keep_alive)
            {
                this->request_is_keep_alive_ = true;
            }
            request_header_field_[this->request_header_field_key_] = this->request_header_field_value_;
            this->request_header_field_keying_ = true;
            this->request_header_field_key_.clear();
            this->request_header_field_key_.clear();
        }
        return 0;
    }

    ret HttpConnection::on_read(int fd, IEventPoll* _poll)
    {
        const static int _max_read_buffer = 10240;

        byte _buffer[_max_read_buffer] = {0};
        int _read_size = read(fd, _buffer, _max_read_buffer);
        if_return(0 >= this->fd_ || nullptr_t == this->poll_, ret_fail);

        if (_read_size > 0)
        {
            if_return(!http_server_, ret_fail);
            __sp<IHttpCallback> _callback;
            this->http_server_->get_http_callback(&_callback);
            if_return(!_callback, ret_fail);
            if (_read_size != this->input_buffer((char*)_buffer, _read_size))
            {
                _callback->on_http_error(this->connection_self_, http_error_decode);
                this->on_set_close(this->fd_, this->poll_, false);
                return callback_fail;
            }

            int _max_read_size = 0;
            ret _ret = this->http_server_->get_max_read_buffer(&_max_read_size);
            if_return(!__success(_ret), ret_fail);
            if (this->request_body_size_ > _max_read_size)
            {
                _callback->on_http_error(this->connection_self_, http_error_exceed);
                this->on_set_close(this->fd_, this->poll_, true);
                return callback_fail;
            }
        }
        else if (0 == _read_size || (_read_size == -1 && errno != EAGAIN))
        {
            this->input_buffer(nullptr_t, 0);
            this->on_set_close(this->fd_, this->poll_, false);
            return callback_close;
        }

        this->last_read_time_ = this->get_current_time();
        if (_read_size > 0) return callback_continue;
        return callback_complete;
    }

    ret HttpConnection::on_write(int fd, IEventPoll* _poll)
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

    ret HttpConnection::on_time(int id, IEventPoll* _poll)
    {
        if_return(!http_server_, ret_fail);

        int _timeout = 0;
        if_return(!__success(this->http_server_->get_timeout(&_timeout)), ret_fail);
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

    ret HttpConnection::on_set_close(int fd, IEventPoll* _poll, bool _accord)
    {
        this->connection_status_ = status_closed;

        if (this->poll_ && this->set_time_out_)
        {
            this->set_time_out_ = false;
            this->poll_->del_timer(0, this->timer_callback_key_);
        }

        __sp<IHttpCallback> _callback;
        if (this->http_server_ && __success(this->http_server_->get_http_callback(&_callback)) && _callback)
        {
            _callback->on_http_close(this->connection_self_, _accord);
        }

        if (this->poll_ && this->delay_delete_)
        {
            this->poll_->add_delay_release(__get_i_entity<IObject>());
        }

        if (this->http_server_)
        {
            this->http_server_->erase(this->fd_);
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