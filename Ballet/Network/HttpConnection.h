#ifndef network_httpconnection_h
#define network_httpconnection_h
#include "Include/Def/Inct.h"
#include "Include/Common/Factory.h"
#include "Include/Network/IEventPoll.h"
#include "Include/Network/IHttpConnection.h"
#include "Include/Network/IHttpCallback.h"
#include "Include/Network/IHttpServer.h"
#include "HttpParser.h"
#include "HttpDef.h"
#include "Current.h"
#include "IoBuffer.h"
#include "Module.h"
#include <map>

namespace network
{
    _implement_bind(HttpConnection, D49195221F7C4234, AA028660D242A768)
    _support IHttpConnection, public Current
    {
        _implement_def(HttpConnection)
        _implement_support_begin()
        _implement_support(IHttpConnection)
        _implement_support_end()

        typedef std::map<std::string, std::string> _t_map_pool;
    public:
        HttpConnection();
        virtual ~HttpConnection();
        bool internal_init();

    public:
        ret init                        ();
        ret close                       (bool);
        ret follow_fd                   (int, ITcpServer*);
        ret follow_http_fd              (int, IHttpServer*);
        ret use_ipv6                    (bool*);
        ret get_status                  (int*);
        ret send_buffer                 (const byte*, int);
        ret send_raw_buffer             (const byte*, int);
        ret set_delay_delete            (bool);
        ret get_delay_delete            (bool*);
        ret get_local_info              (std::string* _addr, int* _port);
        ret get_peer_info               (std::string* _addr, int* _port);
        ret wait_for_event              (IEventPoll*);
        ret get_http_method             (int*);
        ret get_http_version            (int*, int*);
        ret get_http_url                (std::string*);
        ret get_http_header_field       (std::string&, std::string*);
        ret http_respond_begin          ();
        ret http_respond_version        (int, int);
        ret http_respond_keep_alive     ();
        ret http_respond_chunked        ();
        ret http_respond_status         (int, const char*);
        ret http_respond_content_type   (const char* _type);
        ret http_respond_content_length (int _size);
        ret http_respond_header         (const char*, const char*);
        ret http_respond_header_complete();
        ret http_respond_body           (const byte*, int);
        ret http_respond_chunk          (const byte*, int);
        ret http_respond_chunk_complete ();
        ret http_respond_complete       (bool);

    public:
        int input_buffer                (const char* _buffer, int _size);
        int on_url_cb                   (http_parser* parser, const char* _buffer, size_t len);
        int on_header_field_cb          (http_parser* parser, const char* _buffer, size_t len);
        int on_header_value_cb          (http_parser* parser, const char* _buffer, size_t len);
        int on_body_cb                  (http_parser* parser, const char* _buffer, size_t len);
        int on_message_begin            (http_parser* parser);
        int on_message_complete         (http_parser* parser);
        int on_headers_complete         (http_parser* parser);
        int on_set_http_field           ();

    private:
        ret on_read                     (int fd, IEventPoll* _poll);
        ret on_write                    (int fd, IEventPoll* _poll);
        ret on_time                     (int id, IEventPoll* _poll);
        ret on_set_close                (int fd, IEventPoll* _poll, bool _accord);

    protected:
        int                             fd_;
        int                             connection_status_;
        int64_t                         last_read_time_;
        bool                            delay_delete_;
        bool                            set_time_out_;
        bool                            wait_for_write_;
        NetBuffer                       write_buffer_;

        http_parser                     request_parser_;
        http_parser_settings            request_setting_;
        std::string                     request_url_;
        _t_map_pool                     request_header_field_;
        int                             request_body_size_;
        bool                            request_header_field_keying_;
        bool                            request_is_keep_alive_;
        std::string                     request_header_field_key_;
        std::string                     request_header_field_value_;

        bool                            respond_keep_alive_;
        bool                            respond_chunked_;
        std::string                     respond_http_head_buffer_;

        IHttpConnection*                connection_self_;
        __sp<IEventPoll>                poll_;
        __wp<IHttpServer>               http_server_;
        CTimerCallbackPtr<__self>       timer_callback_key_;
        CEventCallbackPtr<__self>       event_callback_key_;
    };
    _implement_register_ref_object(IHttpConnection, HttpConnection, __rtns, __module_register)
}
#endif//network_httpconnection_h