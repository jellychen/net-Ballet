#ifndef _include_network_udpprotocol_h
#define _include_network_udpprotocol_h
#include "Include/Def/Inct.h"

namespace network
{
    enum udp_protocol_status
    {
        udp_protocol_success    = 0x0,
        udp_protocol_error      = 0x1,
    };

    _interface IUdpBase;
    _interface IUdpCallback;
    _interface_inherit_def(IUdpProtocol, IObject)
    {
        def_function(encode,    (const byte*, int, IUdpBase*, const sockaddr*));
        def_function(decode,    (const byte*, int, IUdpBase*, const sockaddr*));
        def_function(callback,  (IUdpCallback*));
    };
    _interface_inherit_bind(IUdpProtocol, IObject, network, FDB5010A3D2247D9, 8BC0637F387778C6)
}
#endif//_include_network_udpprotocol_h