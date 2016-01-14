#ifndef network_timeheappool_h
#define network_timeheappool_h
#include "Include/Def/Inct.h"
#include "Include/Network/IEventPoll.h"
#include "Include/Network/ITimerCallback.h"
#include "Current.h"
#include "TimeHeapDef.h"

namespace network
{
    class TimeHeapPool: public Current
    {
    public:
        TimeHeapPool();
        virtual ~TimeHeapPool();

    public:
        bool set_timer_out  (int _id, ITimerCallback* _callback, int _time);
        bool set_timer_loop (int _id, ITimerCallback* _callback, int _time);
        bool del_timer      (int _id, ITimerCallback* _callback);
        bool lastest_timeout(int64_t& _time);
        bool tick           (IEventPoll* _poll);

    private:
        bool add_timer_node (timer_node& _node);
        bool del_timer_node (int _index);
        bool set_timer_node (int _id, ITimerCallback* _callback, int _time, bool _loop);

    private:
        size_t              _cache_info;
        _t_list_timer_pool  _info_pool;
        _t_vec_timer_pool   _heap_array;
        _t_map_timer_pool   _index_pool;
    };
}
#endif//network_timeheappool_h