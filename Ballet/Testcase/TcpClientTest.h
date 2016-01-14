#include "Include/Def/Inct.h"
#include "Include/Common/Inct.h"
#include "Include/Network/IEventPoll.h"
#include "Include/Network/ITcpServer.h"
#include "Include/Network/ITcpClient.h"
#include "Include/Network/ITcpCallback.h"
#include "Include/Network/IHttpServer.h"
#include "Include/Network/IHttpConnection.h"

class TcpClientTest
{
public:
    TcpClientTest():_callback(this)
    {
        _callback->hook_on_connect(&TcpClientTest::on_connect);
    }

    bool test()
    {
        __sp<network::IEventPoll> _poll;
        common::let_object(_type_id(network::IEventPoll), (void**)&_poll);
        if (_poll) printf("%s\n", "let_object IEventPoll success");
        _poll->init();

        __sp<network::IHttpServer> _server;
        common::let_object(_type_id(network::IHttpServer), (void**)&_server);
        if (_server) printf("%s\n", "let_object IHttpServer success");

        _server->set_timeout(1000000);
        _server->set_max_write_buffer(500*1024*1024);
        _server->listen("0.0.0.0", 1024, 1024);
        _server->wait_for_event(_poll);

        __sp<network::ITcpClient> _client;
        common::let_object(_type_id(network::ITcpClient), (void**)&_client);
        if (_client) printf("%s\n", "let_object ITcpClient success");

        _client->init();
        _client->wait_for_event(_poll);
        _client->set_callback(_callback);
        _client->connect("0.0.0.0", 1024, 3000);

        _poll->event_loop();
        return true;
    }

    ret on_connect(ITcpConnection* _conn, bool _success)
    {
        if (_success)
        {
            printf("%s\n", "connect success 1");
        }
        else
        {
            printf("%s\n", "connect fail");
        }
        return ret_success;
    }

    CTcpCallbackPtr<TcpClientTest> _callback;
};