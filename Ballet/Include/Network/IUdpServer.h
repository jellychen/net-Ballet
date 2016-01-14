#ifndef _include_network_udpserver_h
#define _include_network_udpserver_h
#include "Include/Def/Inct.h"
#include "Include/Network/IUdpBase.h"

namespace network
{
    _interface_inherit_def(IUdpServer, IUdpBase)
    {
        def_function(listen,        (const char*, int, int));
        def_function(listen_ipv6,   (const char*, int, int));
        def_function(close,         ());
    };
    _interface_inherit_bind(IUdpServer, IUdpBase, network, 34B4F160AFCA44A6, B366966799BE4D22)
}
#endif//_include_network_udpserver_h