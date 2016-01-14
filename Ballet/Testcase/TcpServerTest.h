#include "Include/Def/Inct.h"
#include "Include/Common/Inct.h"
#include "Include/Network/IEventPoll.h"
#include "Include/Network/ITcpServer.h"

class TcpServerTest
{
public:
    TcpServerTest()
    {
    }

    bool test()
    {
        __sp<network::IEventPoll> _poll;
        common::let_object(_type_id(network::IEventPoll), (void**)&_poll);
        if (_poll) printf("%s\n", "let_object IEventPoll success");
        _poll->init();


        __sp<network::ITcpServer> _server;
        common::let_object(_type_id(network::ITcpServer), (void**)&_server);
        if (_server) printf("%s\n", "let_object ITcpServer success");

        _server->init();
        _server->listen("0.0.0.0", 88, 1024);
        _server->wait_for_event(_poll);

        _poll->event_loop();
        return true;
    }
};