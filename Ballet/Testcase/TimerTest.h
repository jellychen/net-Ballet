#include "Include/Def/Inct.h"
#include "Include/Common/Inct.h"
#include "Include/Network/IEventPoll.h"


class TimerTest
{
public:
    TimerTest():_key(this)
    {
        _key->hook_on_time(&TimerTest::on_time);
    }

    network::CTimerCallbackPtr<TimerTest> _key;
    
    ret on_time(int id, network::IEventPoll* base)
    {
        printf("%s %d\n", "on_time", id);
        return true;
    }

    bool test()
    {
        __sp<network::IEventPoll> _poll;
        common::let_object(_type_id(network::IEventPoll), (void**)&_poll);
        if (_poll) printf("%s\n", "let_object success");
        _poll->init();
        //_poll->add_timer_interval(0, _key, 1000);
        for (int i = 0; i < 5000000; ++i)
        {
            _poll->add_timer_interval(i, _key, 10000);
        }
        
        _poll->event_loop();
        return true;
    }
};