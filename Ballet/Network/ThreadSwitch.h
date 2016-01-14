#ifndef network_threadswitch_h
#define network_threadswitch_h
#include "Include/Def/Inct.h"
#include "Include/Network/IEventPoll.h"
#include "ThreadTaskQueue.h"
#include <map>

namespace network
{
    class ThreadSwitch : public ThreadTaskQueue
    {
    public:
        ThreadSwitch();
        virtual ~ThreadSwitch();

    public:
        ret init    (IEventPoll* base);
        ret commit  (const Task*);

    private:
        ret on_read (int id, IEventPoll* poll);

    private:
        int                             event_fd_;
        CEventCallbackPtr<ThreadSwitch> callback_key_;
    };
}
#endif//network_threadswitch_h