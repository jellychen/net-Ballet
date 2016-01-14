#ifndef _include_network_httpconnection_h
#define _include_network_httpconnection_h
#include "Include/Def/Inct.h"
#include "Include/Network/ITcpConnection.h"
#include "Include/Network/IHttpCallback.h"
#include "Include/Network/IHttpServer.h"

namespace network
{
    enum http_query_function
    {
        http_get        = 0x0,
        http_post       = 0x1,
    };

    _interface_inherit_def(IHttpConnection, ITcpConnection)
    {
        def_function(follow_http_fd,                (int, IHttpServer*));
        def_function(get_http_method,               (int*));
        def_function(get_http_version,              (int*, int*));
        def_function(get_http_url,                  (std::string*));
        def_function(get_http_header_field,         (std::string&, std::string*));
        def_function(http_respond_begin,            ());
        def_function(http_respond_version,          (int, int));
        def_function(http_respond_keep_alive,       ());
        def_function(http_respond_chunked,          ());
        def_function(http_respond_status,           (int, const char*));
        def_function(http_respond_content_type,     (const char* _type));
        def_function(http_respond_content_length,   (int _size));
        def_function(http_respond_header,           (const char*, const char*));
        def_function(http_respond_header_complete,  ());
        def_function(http_respond_body,             (const byte*, int));
        def_function(http_respond_chunk,            (const byte*, int));
        def_function(http_respond_chunk_complete,   ());
        def_function(http_respond_complete,         (bool));
    };
    _interface_inherit_bind(IHttpConnection, ITcpConnection, network, 447C7695B46E46f8, 876D9B3A7A9B1CE4)
}
#endif//_include_network_httpconnection_h