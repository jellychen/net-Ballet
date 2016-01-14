#ifndef _include_network_tcpserver_h
#define _include_network_tcpserver_h
#include "Include/Def/Inct.h"
#include "Include/Network/IEventPoll.h"
#include "Include/Network/ITcpProtocol.h"
#include "Include/Network/ITcpCallback.h"

namespace network
{
    _interface_inherit_def(ITcpServer, IObject)
    {
    	def_function(init, 					());
		def_function(listen,				(const char*, int, int));
		def_function(listen_ipv6,			(const char*, int, int));
		def_function(erase,					(int));
		def_function(close,					());
		def_function(use_ipv6,				(bool*));
		def_function(set_protocol,			(ITcpProtocol*));
		def_function(get_protocol,			(ITcpProtocol**));
		def_function(set_max_read_buffer,	(int));
		def_function(set_max_write_buffer,	(int));
		def_function(get_max_read_buffer,	(int*));
		def_function(get_max_write_buffer,	(int*));
		def_function(set_timeout,			(int));
		def_function(get_timeout,			(int*));
		def_function(set_callback,			(ITcpCallback*));
		def_function(get_callback,			(ITcpCallback**));
		def_function(wait_for_event,		(IEventPoll*));
    };
    _interface_inherit_bind(ITcpServer, IObject, network, 00FA19335677486C, B10F26CE684C9767)
}
#endif//_include_network_tcpserver_h