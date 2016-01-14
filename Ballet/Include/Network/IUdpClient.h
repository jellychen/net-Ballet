#ifndef _include_network_udpclient_h
#define _include_network_udpclient_h
#include "Include/Def/Inct.h"
#include "Include/Network/IUdpBase.h"

namespace network
{
    _interface_inherit_def(IUdpClient, IUdpBase)
    {
        def_function(close, ());
    };
    _interface_inherit_bind(IUdpClient, IUdpBase, network, 58391BA7AEA44F91 86985B6D6421A874)
}
#endif//_include_network_udpclient_h