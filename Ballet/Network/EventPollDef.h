#ifndef network_eventpolldef_h
#define network_eventpolldef_h
#include <list>
#include <vector>
#include <map>

#include "Include/Def/Inct.h"
#include "Include/Network/IEventPoll.h"

namespace network
{
    struct _event_callback
    {
        unsigned int            _flag;
        int                     _index;
        __sp<IEventCallback>    _callback;
    };

    typedef std::map<int, _event_callback>  _t_map_event_pool;
    typedef _t_map_event_pool::iterator     _t_map_event_iterator;
    
    struct _event_ready_item
    {
        int                     _id;
        _t_map_event_iterator   _iter;
        unsigned int            _read:1;
        unsigned int            _write:1;
        __sp<IEventCallback>    _callback;
    };
    typedef std::vector<_event_ready_item>  _t_vector_event_pool;
    typedef std::list<__sp<IObject> >       _t_list_event_pool;
}
#endif//network_eventpolldef_h