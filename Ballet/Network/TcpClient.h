#ifndef network_tcpclient_h
#define network_tcpclient_h
#include "Include/Def/Inct.h"
#include "Include/Common/Factory.h"
#include "Include/Network/IEventPoll.h"
#include "Include/Network/ITcpClient.h"
#include "Current.h"
#include "IoBuffer.h"
#include "Module.h"

namespace network
{
    _implement_bind(TcpClient, 00AC242625294042, 80E239E40F44252E)
    _support ITcpClient, public Current
    {
        _implement_def(TcpClient)
        _implement_support_begin()
        _implement_support(ITcpClient)
        _implement_support_end()

    public:
        TcpClient();
        virtual ~TcpClient();
    
    public:
        bool internal_init();

    public:
        ret init                    ();
        ret connect                 (const char* _addr, int _port, int _timeout);
        ret connect_ipv6            (const char* _addr, int _port, int _timeout);
        ret cancel_connect          ();
        ret follow_fd               (int _fd, ITcpServer* _server);
        ret close                   (bool _right_now);
        ret use_ipv6                (bool* _use_ipv6);
        ret get_status              (int* _status);
        ret set_protocol            (ITcpProtocol* _protocol);
        ret get_protocol            (ITcpProtocol** _protocol);
        ret set_max_read_buffer     (int _max);
        ret set_max_write_buffer    (int _max);
        ret get_max_read_buffer     (int* _max);
        ret get_max_write_buffer    (int* _max);
        ret set_timeout             (int _timeout);
        ret get_timeout             (int* _timeout);
        ret set_callback            (ITcpCallback* _callback);
        ret get_callback            (ITcpCallback** _callback);
        ret set_heartbeat           (bool _heartbeat, int _time);
        ret get_heartbeat           (bool* _heartbeat);
        ret send_buffer             (const byte* _buffer, int _size);
        ret send_raw_buffer         (const byte* _buffer, int _size);
        ret set_delay_delete        (bool);
        ret get_delay_delete        (bool*);
        ret get_local_info          (std::string* _addr, int* _port);
        ret get_peer_info           (std::string* _addr, int* _port);
        ret wait_for_event          (IEventPoll* _poll);

    private:
        ret on_read                 (int fd, IEventPoll* _poll);
        ret on_write                (int fd, IEventPoll* _poll);
        ret on_time                 (int id, IEventPoll* _poll);
        ret on_set_close            (int fd, IEventPoll* _poll, bool _accord);

    protected:
        int                         fd_;
        int                         connection_status_;
        int                         max_read_size_;
        int                         max_write_size_;
        int                         want_read_buffer_size_;
        bool                        use_ipv6_;
        bool                        set_time_out_;
        int                         time_out_;
        bool                        set_heartbeat_;
        int                         heartbeat_time_;
        int64_t                     last_read_time_;
        int64_t                     laset_write_time_;
        NetBuffer                   write_buffer_;
        NetBufferArray              read_buffer_;
        ITcpClient*                 client_self_;
        ITcpConnection*             connection_self_;
        __sp<IEventPoll>            poll_;
        __sp<ITcpProtocol>          protocol_;
        __sp<ITcpCallback>          tcp_callback_;
        CEventCallbackPtr<__self>   event_callback_key_;
        CTimerCallbackPtr<__self>   timer_callback_key_;
    };
    _implement_register_ref_object(ITcpClient, TcpClient, __rtns, __module_register)
}
#endif//network_tcpclient_h