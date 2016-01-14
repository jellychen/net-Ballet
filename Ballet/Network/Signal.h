#ifndef network_signal_h
#define network_signal_h
#include <map>

#include "Include/Def/Inct.h"
#include "Include/Network/IEventPoll.h"

namespace network
{
    typedef std::map<int, __sp<ISignalCallback> > _t_map_signal_pool;

    class Signal
    {
    public:
        Signal();
        virtual ~Signal();

    public:
        bool init       (IEventPoll* poll);
        bool set_signal (int _signal, ISignalCallback* callback);
        bool del_signal (int _signal);
        bool dispath    (int _signal);

    private:
        ret on_read     (int id, IEventPoll* poll);

    private:
        _t_map_signal_pool          _pool;
        int                         _fd[2];
        CEventCallbackPtr<Signal>   _callback_key;  
    };
}
#endif//network_signal_h