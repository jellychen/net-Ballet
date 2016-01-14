#ifndef network_httpServer_h
#define network_httpServer_h
#include "Include/Def/Inct.h"
#include "Include/Common/Factory.h"
#include "Include/Network/IEventPoll.h"
#include "Include/Network/IHttpConnection.h"
#include "Include/Network/IHttpServer.h"
#include "HttpConnection.h"
#include "Module.h"
#include <map>

namespace network
{
    typedef std::map<int, __sp<IHttpConnection> > _t_map_http_connection_pool;

    _implement_bind(HttpServer, 212D448275AF4B5E, A127AF16A5FDBE41)
    _support IHttpServer
    {
        _implement_def(HttpServer)
        _implement_support_begin()
        _implement_support(IHttpServer)
        _implement_support_end()

    public:
        HttpServer();
        virtual ~HttpServer();

    public:
        ret init                ();
        ret listen              (const char*, int, int);
        ret listen_ipv6         (const char*, int, int);
        ret erase               (int);
        ret close               ();
        ret use_ipv6            (bool*);
        ret set_protocol        (ITcpProtocol*);
        ret get_protocol        (ITcpProtocol**);
        ret set_max_read_buffer (int);
        ret set_max_write_buffer(int);
        ret get_max_read_buffer (int*);
        ret get_max_write_buffer(int*);
        ret set_timeout         (int);
        ret get_timeout         (int*);
        ret set_callback        (ITcpCallback*);
        ret get_callback        (ITcpCallback**);
        ret set_http_callback   (IHttpCallback*);
        ret get_http_callback   (IHttpCallback**);
        ret wait_for_event      (IEventPoll*);

    private:
        ret on_accept           (int _fd, IEventPoll* poll);

    protected:
        int                             fd_;
        int                             max_read_size_;
        int                             max_write_size_;
        int                             time_out_;
        bool                            use_ipv6_;
        bool                            listen_accept_;
        __sp<IEventPoll>                poll_;
        __sp<IHttpCallback>             http_callback_;
        _t_map_http_connection_pool     pool_;
        CEventCallbackPtr<HttpServer>   event_callback_key_;
    };
    _implement_register_ref_object(IHttpServer, HttpServer, __rtns, __module_register)
}
#endif//network_httpServer_h