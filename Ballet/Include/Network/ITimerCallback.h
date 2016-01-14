#ifndef _include_network_timercallback_h
#define _include_network_timercallback_h
#include "Include/Def/Inct.h"

namespace network
{
    _interface IEventPoll;
    
    _interface_inherit_def(ITimerCallback, ICallback)
    {
        def_function(on_time, (int id, IEventPoll* poll));
    };
    _interface_inherit_bind(ITimerCallback, ICallback, network, 112585A399374290, 8C812756540550D0)

    __callback_sink_begin(CTimerCallback)
    __callback_function(ret, on_time, (int id, IEventPoll* poll), (id, poll))
    __callback_sink_end()
    __callback_sink_ptr(CTimerCallback, ITimerCallback)
}
#endif//_include_network_timercallback_h