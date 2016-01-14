#ifndef _include_network_signalcallback_h
#define _include_network_signalcallback_h
#include "Include/Def/Inct.h"

namespace network
{
    _interface IEventPoll;

    _interface_inherit_def(ISignalCallback, ICallback)
    {
        def_function(on_signal, (int id, IEventPoll* poll));
    };
    _interface_inherit_bind(ISignalCallback, ICallback, network, 7F8E61944BCD4293, A9746D6C8AFF75BB)

    __callback_sink_begin(CSignalCallback)
    __callback_function(ret, on_signal, (int id, IEventPoll* poll), (id, poll))
    __callback_sink_end()
    __callback_sink_ptr(CSignalCallback, ISignalCallback)
}
#endif//_include_network_signalcallback_h