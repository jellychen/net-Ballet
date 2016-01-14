#ifndef _include_network_udpbase_h
#define _include_network_udpbase_h
#include "Include/Def/Inct.h"
#include "Include/Network/IEventPoll.h"
#include "Include/Network/IUdpProtocol.h"
#include "Include/Network/IUdpCallback.h"

namespace network
{
    _interface_inherit_def(IUdpBase, IObject)
    {
        def_function(init,                  ());
        def_function(use_ipv6,              (bool*));
        def_function(send_buffer,           (const byte*, int, const sockaddr*));
        def_function(send_raw_buffer,       (const byte*, int, const sockaddr*));
        def_function(set_protocol_callback, (IUdpProtocol*, IUdpCallback*));
        def_function(get_protocol_callback, (IUdpProtocol**, IUdpCallback**));
        def_function(wait_for_event,        (IEventPoll*));
    };
    _interface_inherit_bind(IUdpBase, IObject, network, FEBBA1178F304320, A2592D148BD90C22)
}
#endif//_include_network_udpinput_h