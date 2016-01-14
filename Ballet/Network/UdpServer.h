#ifndef network_udpserver_h
#define network_udpserver_h
#include "Include/Def/Inct.h"
#include "Include/Common/Factory.h"
#include "Include/Network/IEventPoll.h"
#include "Include/Network/IUdpServer.h"
#include "Module.h"

namespace network
{
    _implement_bind(UdpServer, AB2D2486E70943BE, A85DE2B1B36796C5)
    _support IUdpServer
    {
        _implement_def(UdpServer)
        _implement_support_begin()
        _implement_support(IUdpServer)
        _implement_support_end()
    
    public:
        UdpServer();
        virtual ~UdpServer();
        bool internal_init();

    public:
        ret init                    ();
        ret use_ipv6                (bool*);
        ret listen                  (const char*, int, int);
        ret listen_ipv6             (const char*, int, int);
        ret close                   ();
        ret send_buffer             (const byte*, int, const sockaddr*);
        ret send_raw_buffer         (const byte*, int, const sockaddr*);
        ret set_protocol_callback   (IUdpProtocol*, IUdpCallback*);
        ret get_protocol_callback   (IUdpProtocol**, IUdpCallback**);
        ret wait_for_event          (IEventPoll*);

    private:
        ret on_read                 (int, IEventPoll*);

    protected:
        int                         fd_;
        bool                        use_ipv6_;
        union struct_sockaddr_
        {
            sockaddr_in6            addr_ipv6_struct_;
            sockaddr_in             addr_ipv4_struct_;
        };
        struct_sockaddr_            addr_struct_;
        bool                        peer_set_;

        IUdpBase*                   udp_base_;
        bool                        wait_for_read_;
        __sp<IEventPoll>            poll_;
        __sp<IUdpProtocol>          protocol_;
        __sp<IUdpCallback>          udp_callback_;
        CEventCallbackPtr<__self>   event_callback_key_;
    };
    _implement_register_ref_object(IUdpServer, UdpServer, __rtns, __module_register)
}
#endif//network_udpserver_h