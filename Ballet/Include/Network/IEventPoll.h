#ifndef _include_network_eventpoll_h
#define _include_network_eventpoll_h
#include "Include/Def/Inct.h"
#include "Include/Network/ITimerCallback.h"
#include "Include/Network/IEventCallback.h"
#include "Include/Network/ISignalCallback.h"

namespace network
{
    _interface_inherit_def(IEventPoll, IObject)
    {
        def_function(init,                  ());
        def_function(add_delay_release,     (IObject* _object));
        def_function(add_timer_once,        (int id, ITimerCallback* callback, int _time));
        def_function(add_timer_interval,    (int id, ITimerCallback* callback, int _time));
        def_function(del_timer,             (int id, ITimerCallback* callback));
        def_function(set_event_listener,    (int id, int flag, IEventCallback* callback));
        def_function(del_event_listener,    (int id, int flag));
        def_function(set_signal_listener,   (int id, ISignalCallback* callback));
        def_function(del_signal_listener,   (int id));
        def_function(event_loop_entry,      (TaskQueueEntry**));
        def_function(event_loop,            ());
        def_function(event_loop_once,       (int _time));
        def_function(exit_event_loop,       ());
    };
    _interface_inherit_bind(IEventPoll, IObject, network, 79FB00A6FC344554, AE5D572C15952153)
}
#endif//_include_network_eventpoll_h