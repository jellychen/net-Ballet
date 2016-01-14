#ifndef _include_network_eventcallback_h
#define _include_network_eventcallback_h
#include "Include/Def/Inct.h"

namespace network
{
    enum event_flag
    {
        event_none          = 0x0,
        event_read          = 0x1,
        event_write         = 0x2,
    };

    enum event_callback
    {
        callback_continue   = 0x0,
        callback_complete   = 0x1,
        callback_close      = 0x2,
        callback_fail       = 0x3,
        callback_again      = 0x4,
    };

    _interface IEventPoll;
    _interface_inherit_def(IEventCallback, ICallback)
    {
        def_function(notify_read,   (int id, IEventPoll* poll));
        def_function(notify_write,  (int id, IEventPoll* poll));
    };
    _interface_inherit_bind(IEventCallback, ICallback, network, F7808F848BE04b0C, 90F866DDC6A6A393)
    
    __callback_sink_begin(CEventCallback)
    __callback_function(ret, notify_read,   (int id, IEventPoll* poll), (id, poll))
    __callback_function(ret, notify_write,  (int id, IEventPoll* poll), (id, poll))
    __callback_sink_end()
    __callback_sink_ptr(CEventCallback, IEventCallback)
}
#endif//_include_network_eventcallback_h