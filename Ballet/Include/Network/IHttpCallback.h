#ifndef _include_network_httpcallback_h
#define _include_network_httpcallback_h
#include "Include/Def/Inct.h"

namespace network
{
    enum http_connection_error
    {
        http_error_none         = 0x0,
        http_error_decode       = 0x1,
        http_error_encode       = 0x2,
        http_error_exceed       = 0x3,
    };

    _interface IHttpConnection;
    _interface_inherit_def(IHttpCallback, IObject)
    {
        def_function(on_http_connect,   (IHttpConnection* _conn, bool _success));
        def_function(on_http_begin,     (IHttpConnection* _conn));
        def_function(on_http_header,    (IHttpConnection* _conn));
        def_function(on_http_body,      (IHttpConnection* _conn, const byte*, int));
        def_function(on_http_complete,  (IHttpConnection* _conn));
        def_function(on_http_error,     (IHttpConnection* _conn, int _error));
        def_function(on_http_close,     (IHttpConnection* _conn, bool _accord));
    };
    _interface_inherit_bind(IHttpCallback, IObject, network, 77B357A227914F11, AC767A3DE6CF01B8)

    __callback_sink_begin(CHttpCallback)
    __callback_function(ret, on_http_connect,   (IHttpConnection* _conn, bool _success), (_conn, _success))
    __callback_function(ret, on_http_begin,     (IHttpConnection* _conn), (_conn))
    __callback_function(ret, on_http_header,    (IHttpConnection* _conn), (_conn))
    __callback_function(ret, on_http_body,      (IHttpConnection* _conn, const byte* _buf, int _len), (_conn, _buf, _len))
    __callback_function(ret, on_http_complete,  (IHttpConnection* _conn), (_conn))
    __callback_function(ret, on_http_error,     (IHttpConnection* _conn, int _error), (_conn, _error))
    __callback_function(ret, on_http_close,     (IHttpConnection* _conn, bool _accord), (_conn, _accord))
    __callback_sink_end()
    __callback_sink_ptr(CHttpCallback, IHttpCallback)
}
#endif//_include_network_tcpcallback_h