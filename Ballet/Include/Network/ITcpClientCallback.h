#ifndef _include_network_tcpclientcallback_h
#define _include_network_tcpclientcallback_h
#include "Include/Def/Inct.h"

namespace network
{
    _interface ITcpClient;
    _interface_inherit_def(ITcpClientCallback, ICallback)
    {
        def_function(on_connect,    (ITcpClient* _client, bool _success));
        def_function(on_close,      (ITcpClient* _client, bool _accord));
        def_function(on_error,      (ITcpClient* _client, int _error));
        def_function(on_receive,    (ITcpClient* _client, const byte* _buffer, int _size));
        def_function(on_heartbeat,  (ITcpClient* _client));
    };
    _interface_inherit_bind(ITcpClientCallback, ICallback, network, A3C239D0D42F47CF, 90BA0EC86288AD61)

    __callback_sink_begin(CTcpClientCallback)
    __callback_function(ret, on_connect,    (ITcpClient* _client, bool _success), (_client, _success))
    __callback_function(ret, on_close,      (ITcpClient* _client, bool _accord), (_client, _accord))
    __callback_function(ret, on_error,      (ITcpClient* _client, int _error), (_client, _error))
    __callback_function(ret, on_receive,    (ITcpClient* _client, const byte* _buffer, int _size), (_client, _buffer, _size))
    __callback_function(ret, on_heartbeat,  (ITcpClient* _client), ())
    __callback_sink_end()
    __callback_sink_ptr(CTcpClientCallback, ITcpClientCallback)
}
#endif//_include_network_tcpclientcallback_h