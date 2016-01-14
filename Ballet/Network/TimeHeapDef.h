#ifndef network_timeheapdef_h
#define network_timeheapdef_h
#include <map>
#include <vector>
#include <list>

#include "Include/Def/Inct.h"
#include "Include/Network/IEventPoll.h"
#include "Include/Network/ITimerCallback.h"

namespace network
{
    struct timer_index
    {
        int                     _id;
        long                    _callback_hash;
    };

    struct timer_info
    {
        int                     _id;
        int                     _index;
        int                     _timespec;
        bool                    _loop;
        __sp<ITimerCallback>    _callback;
    };

    struct timer_node
    {
        timer_info*             _info;
        int64_t                 _timeout;
        int64_t                 _tick_time;
    };

    inline bool operator < (const timer_index& _l, const timer_index& _r)
    {
        if (_l._id < _r._id) return true;
        if (_l._id > _r._id) return false;
        if (_l._callback_hash < _r._callback_hash) return true;
        return false;
    }

    typedef std::vector<timer_node>             _t_vec_timer_pool;
    typedef std::map<timer_index, timer_info*>  _t_map_timer_pool;
    typedef std::list<timer_info*>              _t_list_timer_pool;
}
#endif//network_timeheapdef_h