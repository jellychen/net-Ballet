#ifndef _include_network_tcpclient_h
#define _include_network_tcpclient_h
#include "Include/Def/Inct.h"
#include "Include/Network/IEventPoll.h"
#include "Include/Network/ITcpStatus.h"
#include "Include/Network/ITcpProtocol.h"
#include "Include/Network/ITcpConnection.h"
#include "Include/Network/ITcpCallback.h"

namespace network
{
    _interface_inherit_def(ITcpClient, ITcpConnection)
    {
        def_function(connect,               (const char*, int, int));
        def_function(connect_ipv6,          (const char*, int, int));
        def_function(cancel_connect,        ());
        def_function(set_protocol,          (ITcpProtocol*));
        def_function(get_protocol,          (ITcpProtocol**));
        def_function(set_max_read_buffer,   (int));
        def_function(set_max_write_buffer,  (int));
        def_function(get_max_read_buffer,   (int*));
        def_function(get_max_write_buffer,  (int*));
        def_function(set_timeout,           (int));
        def_function(get_timeout,           (int*));
        def_function(set_callback,          (ITcpCallback*));
        def_function(get_callback,          (ITcpCallback**));
        def_function(set_heartbeat,         (bool, int));
        def_function(get_heartbeat,         (bool*)); 
    };
    _interface_inherit_bind(ITcpClient, ITcpConnection, network, 0583C4EBC48745Ef, 8409BD5FCD22F938)
}
#endif//_include_network_tcpclient_h