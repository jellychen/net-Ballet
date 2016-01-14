#ifndef _include_network_udpcallback_h
#define _include_network_udpcallback_h
#include "Include/Def/Inct.h"
#include "Include/Network/IUdpBase.h"

namespace network
{
    _interface_inherit_def(IUdpCallback, ICallback)
    {
        def_function(on_receive, (IUdpBase* _input, const byte* _buffer, int _size));
    };
    _interface_inherit_bind(IUdpCallback, ICallback, network, 03BEACBF3C1C45F2, 9B8649D5121B6777)

    __callback_sink_begin(CUdpCallback)
    __callback_function(ret, on_receive, (IUdpBase* _input, const byte* _buffer, int _size), (_input, _buffer, _size))
    __callback_sink_end()
    __callback_sink_ptr(CUdpCallback, IUdpCallback)
}
#endif//_include_network_udpinput_h