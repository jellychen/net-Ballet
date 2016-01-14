#include "Include/Def/Inct.h"
#include "Include/Common/Inct.h"
#include "Include/Network/IEventPoll.h"

class SignalTest
{
public:
    SignalTest():_key(this)
    {
        _key->hook_on_signal(&SignalTest::on_signal);
    }

    __sp<network::IEventPoll> _poll;
    network::CSignalCallbackPtr<SignalTest> _key;
    
    ret on_signal(int id, network::IEventPoll* base)
    {
        printf("SignalTest on_signal %d\n", id);
        _poll->exit_event_loop();
        return true;
    }

    bool test()
    {
        /*
        signal(SIGINT,  SIG_IGN);           // 终端中断  
        signal(SIGHUP,  SIG_IGN);           // 连接挂断  
        signal(SIGQUIT, SIG_IGN);           // 终端退出  
        signal(SIGPIPE, SIG_IGN);           // 向无读进程的管道写数据  
        signal(SIGTTOU, SIG_IGN);           // 后台程序尝试写操作  
        signal(SIGTTIN, SIG_IGN);           // 后台程序尝试读操作  
        signal(SIGTERM, SIG_IGN);           // 终止
        */
        
        common::let_object(_type_id(network::IEventPoll), (void**)&_poll);
        if (_poll) printf("%s\n", "let_object success");
        _poll->init();
        // 监控ctrl c
        _poll->set_signal_listener(60, _key);
        _poll->event_loop();
        return true;
    }
};