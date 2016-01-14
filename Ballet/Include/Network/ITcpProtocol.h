#ifndef _include_network_tcpprotocol_h
#define _include_network_tcpprotocol_h
#include "Include/Def/Inct.h"

namespace network
{
	enum tcp_protocol_status
	{
		tcp_protocol_success      = 0x0,
		tcp_protocol_error        = 0x1,
		tcp_protocol_not_enough	  = 0x2,
	};

	_interface ITcpCallback;
	_interface ITcpConnection;
	_interface_inherit_def(ITcpProtocol, IObject)
    {
    	def_function(input,		(const byte*, int, int*, int*));
		def_function(encode,	(const byte*, int, ITcpConnection*));
		def_function(decode,	(const byte*, int, ITcpConnection*));
        def_function(callback,  (ITcpCallback*));
    };
    _interface_inherit_bind(ITcpProtocol, IObject, network, A3CD8CECA8924F57, 9321563B23626A3C)
}
#endif//_include_network_tcpprotocol_h