#ifndef network_eventpoll_h
#define network_eventpoll_h
#include "Include/Def/Inct.h"
#include "Include/Common/Factory.h"
#include "Include/Network/IEventPoll.h"
#include "EventPollDef.h"
#include "TimeHeapPool.h"
#include "ThreadSwitch.h"
#include "Signal.h"
#include "Module.h"

namespace network
{
    _implement_bind(EventPoll, EAEAA5A64E4649d0, BB961641CD89D63A)
    _support IEventPoll
    {
        _implement_support_begin()
        _implement_support(IEventPoll)
        _implement_support_end()
    
    public:
        EventPoll();
        virtual ~EventPoll();

    public:
        ret init                ();
        ret add_delay_release   (IObject* _object);
        ret add_timer_once      (int id, ITimerCallback* callback, int _time);
        ret add_timer_interval  (int id, ITimerCallback* callback, int _time);
        ret del_timer           (int id, ITimerCallback* callback);
        ret set_event_listener  (int id, int flag, IEventCallback* callback);
        ret del_event_listener  (int id, int flag);
        ret set_signal_listener (int id, ISignalCallback* callback);
        ret del_signal_listener (int id);

        ret event_loop_entry    (TaskQueueEntry** _entry);
        ret event_loop          ();
        ret event_loop_once     (int _time);
        ret exit_event_loop     ();

    protected:
        ret exc_ready_list      (IEventPoll* poll);
        ret add_ready_list      (int index, int id, int ready,
                                        _t_map_event_iterator _iter, IEventCallback* callback);
        ret del_ready_list      (int index, int id, int ready);

    private:
        int                     efd_;
        bool                    should_exit_;
        bool                    ready_list_protected_;
        TimeHeapPool            time_heap_pool_;
        ThreadSwitch            thread_switch_;
        _t_list_event_pool      release_pool_;
        _t_map_event_pool       event_pool_;
        _t_vector_event_pool    event_ready_list_;
    };
    _implement_register_ref_object(IEventPoll, EventPoll, __rtns, __module_register)
}
#endif//network_eventpoll_h