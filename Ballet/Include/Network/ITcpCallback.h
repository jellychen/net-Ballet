#ifndef _include_network_tcpcallback_h
#define _include_network_tcpcallback_h
#include "Include/Def/Inct.h"

namespace network
{
	_interface ITcpConnection;
	_interface_inherit_def(ITcpCallback, ICallback)
    {
    	def_function(on_connect, 	(ITcpConnection* _conn, bool _success));
    	def_function(on_close, 		(ITcpConnection* _conn, bool _accord));
    	def_function(on_error, 		(ITcpConnection* _conn, int _error));
        def_function(on_receive,    (ITcpConnection* _conn, const byte* _buffer, int _size));
        def_function(on_heartbeat,  (ITcpConnection* _conn));
    };
    _interface_inherit_bind(ITcpCallback, ICallback, network, 67254DCC7D5E436B, ACD7D749E56B775C)

	__callback_sink_begin(CTcpCallback)
	__callback_function(ret, on_connect, 	(ITcpConnection* _conn, bool _success), (_conn, _success))
	__callback_function(ret, on_close, 		(ITcpConnection* _conn, bool _accord), (_conn, _accord))
	__callback_function(ret, on_error, 		(ITcpConnection* _conn, int _error), (_conn, _error))
    __callback_function(ret, on_receive,    (ITcpConnection* _conn, const byte* _buffer, int _size), (_conn, _buffer, _size))
    __callback_function(ret, on_heartbeat,  (ITcpConnection* _conn), (_conn))
	__callback_sink_end()
	__callback_sink_ptr(CTcpCallback, ITcpCallback)
}
#endif//_include_network_tcpcallback_h