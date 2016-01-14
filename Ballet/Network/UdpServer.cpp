#include "UdpServer.h"

namespace network
{
    UdpServer::UdpServer():event_callback_key_(this)
    {
        this->fd_ = -1;
        this->use_ipv6_ = false;
        this->wait_for_read_ = false;
        this->peer_set_ = true;
        this->event_callback_key_->hook_notify_read(&UdpServer::on_read);
    }

    UdpServer::~UdpServer()
    {
        if (this->fd_ > 0)
        {
            if (this->poll_)
            {
                this->poll_->del_event_listener(this->fd_, event_read);
            }
            ::close(this->fd_);
        }
    }

    bool UdpServer::internal_init()
    {
        this->udp_base_ = __get_i_entity<IUdpBase>();
        return nullptr_t != this->udp_base_;
    }

    ret UdpServer::init()
    {
        return ret_success;
    }

    ret UdpServer::use_ipv6(bool* _ipv6)
    {
        if_return(nullptr_t == _ipv6, ret_args_error);
        *_ipv6 = this->use_ipv6_;
        return ret_success;
    }
    
    ret UdpServer::listen(const char* _addr, int _port, int _backlog)
    {
        if_return(0 != this->fd_, ret_fail);
        this->use_ipv6_ = false;

        if (this->fd_ = (int)socket(AF_INET, SOCK_DGRAM, 0), fd_ <= 0)
        {
            perror("create socket");
            this->fd_ = 0;
            return ret_fail;
        }

        memset(&addr_struct_.addr_ipv4_struct_, 0, sizeof(struct sockaddr_in));
        addr_struct_.addr_ipv4_struct_.sin_family = AF_INET;
        addr_struct_.addr_ipv4_struct_.sin_port = htons(_port);

        if (nullptr_t == _addr) 
        {
            addr_struct_.addr_ipv4_struct_.sin_addr.s_addr = htonl(INADDR_ANY);
        }
        else
        {
            if_return(inet_aton(_addr, &addr_struct_.addr_ipv4_struct_.sin_addr) <= 0, ret_args_error);
        }

        int _opt_open_value = 1;
        setsockopt(this->fd_, SOL_SOCKET, SO_REUSEADDR, (const void*)&_opt_open_value, sizeof(int));
        setsockopt(this->fd_, SOL_SOCKET, SO_KEEPALIVE, (const void*)&_opt_open_value, sizeof(int));
        fcntl(this->fd_, F_SETFL, fcntl(this->fd_, F_GETFL)|O_NONBLOCK);

        if (::bind(this->fd_, (sockaddr*)(&addr_struct_.addr_ipv4_struct_), sizeof(sockaddr)) == -1)
        {
            perror("bind error");
            ::close(this->fd_); this->fd_ = 0;
            return ret_fail;
        }
        return ret_success;
    }
    
    ret UdpServer::listen_ipv6(const char* _addr, int _port, int _backlog)
    {
        if_return(0 != this->fd_, ret_fail);
        this->use_ipv6_ = true;

        if (this->fd_ = (int)socket(PF_INET6, SOCK_DGRAM, 0), fd_ <= 0)
        {
            perror("create socket");
            this->fd_ = 0;
            return ret_fail;
        }

        memset(&addr_struct_.addr_ipv6_struct_, 0, sizeof(struct sockaddr_in6));
        addr_struct_.addr_ipv6_struct_.sin6_family = AF_INET6;
        addr_struct_.addr_ipv6_struct_.sin6_port = htons(_port);

        if (nullptr_t == _addr) 
        {
            addr_struct_.addr_ipv6_struct_.sin6_addr = in6addr_any;// IN6ADDR_ANY;
        }
        else
        {
            if_return(inet_pton(AF_INET6, _addr, &addr_struct_.addr_ipv6_struct_.sin6_addr) <= 0, ret_args_error);
        }

        int _opt_open_value = 1;
        setsockopt(this->fd_, SOL_SOCKET, SO_REUSEADDR, (const void*)&_opt_open_value, sizeof(int));
        setsockopt(this->fd_, SOL_SOCKET, SO_KEEPALIVE, (const void*)&_opt_open_value, sizeof(int));
        fcntl(this->fd_, F_SETFL, fcntl(this->fd_, F_GETFL)|O_NONBLOCK);

        if (::bind(this->fd_, (sockaddr*)(&addr_struct_.addr_ipv6_struct_), sizeof(sockaddr_in6)) == -1)
        {
            perror("bind error");
            ::close(this->fd_); this->fd_ = 0;
            return ret_fail;
        }
        return ret_success;
    }
    
    ret UdpServer::close()
    {
        if (this->fd_ > 0)
        {
            if (this->poll_)
            {
                this->poll_->del_event_listener(this->fd_, event_read);
            }
            ::close(this->fd_);
            this->fd_ = 0;
        }
        return ret_success;
    }
    
    ret UdpServer::send_buffer(const byte* _buffer, int _size, const sockaddr* _addr)
    {
        if_return(!this->protocol_, ret_fail);
        if_return(_buffer == nullptr_t || 0 >= _size, ret_args_error);
        return this->protocol_->encode(_buffer, _size, this->udp_base_, _addr);
    }
    
    ret UdpServer::send_raw_buffer(const byte* _buffer, int _size, const sockaddr* _addr)
    {
        if_return(nullptr_t == _buffer || 0 >= _size, ret_fail);
        if (false == this->use_ipv6_)
        {
            ::sendto(this->fd_, _buffer, _size, 0, (sockaddr*)(&addr_struct_.addr_ipv4_struct_), sizeof(sockaddr_in));
        }
        else
        {
            ::sendto(this->fd_, _buffer, _size, 0, (sockaddr*)(&addr_struct_.addr_ipv6_struct_), sizeof(sockaddr_in6));
        }
        return ret_success;
    }
    
    ret UdpServer::set_protocol_callback(IUdpProtocol* _protocol, IUdpCallback* _callback)
    {
        this->protocol_ = _protocol;
        this->udp_callback_ = _callback;
        return ret_success;
    }

    ret UdpServer::get_protocol_callback(IUdpProtocol** _protocol, IUdpCallback** _callback)
    {
        if (_protocol) _protocol = nullptr_t;
        if (_callback) _callback = nullptr_t;
        
        if (this->protocol_) 
        {
            this->protocol_->convert(_type_id(IUdpProtocol), (void**)_protocol);
        }

        if (this->udp_callback_)
        {
            this->udp_callback_->convert(_type_id(IUdpCallback), (void**)_callback);
        }
        return ret_success;
    }
    
    ret UdpServer::wait_for_event(IEventPoll* _poll)
    {
        if_return(nullptr_t == _poll, ret_args_error);

        this->poll_ = _poll;
        this->poll_->set_event_listener(this->fd_, event_read, event_callback_key_);
        return ret_success;
    }

    ret UdpServer::on_read(int fd, IEventPoll* _poll)
    {
        const static int _max_read_buffer = 10240;
        byte _buffer[_max_read_buffer] = {0};
        
        int _read_size = 0; socklen_t _addr_size = 0;
        if (false == this->use_ipv6_)
        {
            _read_size = ::recvfrom(this->fd_, _buffer, _max_read_buffer, 0, (sockaddr*)(&addr_struct_.addr_ipv4_struct_), &_addr_size);
        }
        else
        {
            _read_size = ::recvfrom(this->fd_, _buffer, _max_read_buffer, 0, (sockaddr*)(&addr_struct_.addr_ipv6_struct_), &_addr_size);
        }
        if_return(0 >= _read_size || !this->udp_callback_, ret_fail);

        this->udp_callback_->on_receive(this->udp_base_, _buffer, _read_size);
        return ret_success;
    }
}