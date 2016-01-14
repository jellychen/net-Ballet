#ifndef _include_network_tcpconnection_h
#define _include_network_tcpconnection_h
#include "Include/Def/Inct.h"
#include "Include/Network/IEventPoll.h"
#include "Include/Network/ITcpStatus.h"
#include "Include/Network/ITcpServer.h"
#include "Include/Network/ITcpProtocol.h"
#include "Include/Network/ITcpCallback.h"

namespace network
{
	_interface_inherit_def(ITcpConnection, IObject)
	{
		def_function(init,					());
		def_function(close,					(bool));
		def_function(follow_fd,				(int, ITcpServer*));
		def_function(use_ipv6,				(bool*));
		def_function(get_status,			(int*));
		def_function(send_buffer, 			(const byte*, int));
		def_function(send_raw_buffer,		(const byte*, int));
		def_function(set_delay_delete,		(bool));
		def_function(get_delay_delete,		(bool*));
		def_function(get_local_info,		(std::string*, int*));
		def_function(get_peer_info,			(std::string*, int*));
		def_function(wait_for_event,		(IEventPoll*));
	};
	_interface_inherit_bind(ITcpConnection, IObject, network, 1B1EB5F4B3724Af9, 90834DC2131F3CC5)
}
#endif//_include_network_tcpconnection_h