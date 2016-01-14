#include "Include/Def/Inct.h"
#include "Include/Common/Inct.h"
#include "Include/Network/IEventPoll.h"
#include "Include/Network/ITcpServer.h"
#include "Include/Network/ITcpProtocol.h"
#include "Include/Network/ITcpCallback.h"
#include "Include/Network/ITcpConnection.h"

namespace network
{
    _implement_bind(Telnet, BA0CCC1C947C415C, BF20FE2B778C9385)
    _support ITcpProtocol
    {
        _implement_support_begin()
        _implement_support(ITcpProtocol)
        _implement_support_end()
    
    public:
        ret input(const byte* _buffer, int _size, int* _l, int* _info)
        {
            if_return(_buffer == nullptr_t || 0  >= _size, ret_args_error);
            if_return(_l == nullptr_t || _info == nullptr_t, ret_args_error);

            const void* _ptr = ::memchr((const void*)_buffer, '\n', _size);
            if (_ptr == nullptr_t) 
            {
                *_info = tcp_protocol_not_enough;
            }
            else
            {
                *_l = (const byte*)_ptr - _buffer + 1;
                *_info = tcp_protocol_success;
            }
            return ret_success;
        }

        ret encode(const byte* _buffer, int _size, ITcpConnection* _conn)
        {
            if_return(_buffer == nullptr_t || 0  >= _size || _conn == nullptr_t, ret_args_error);

            byte* _temp = (byte*)malloc(_size +1);
            if_return(_temp == nullptr_t, ret_fail);
            memcpy(_temp, _buffer, _size); _temp[_size] = '\n';
            _conn->send_raw_buffer(_temp, _size+1);
            free(_temp);
            return ret_success;
        }
        
        ret decode(const byte* _buffer, int _size, ITcpConnection* _conn)
        {
            if (callback_)
                this->callback_->on_receive(_conn, _buffer, _size-1);
            return ret_success;
        }

        ret callback(ITcpCallback* _callback)
        {
            this->callback_ = _callback;
            return ret_success;
        }

    private:
        __sp<ITcpCallback> callback_;
    };
}

class TelnetTest
{
public:
    network::CTcpCallbackPtr<TelnetTest> _callback_key;

    TelnetTest():_callback_key(this)
    {
        _callback_key->hook_on_connect(&TelnetTest::on_connect);
        _callback_key->hook_on_close(&TelnetTest::on_close);
        _callback_key->hook_on_error(&TelnetTest::on_error);
        _callback_key->hook_on_receive(&TelnetTest::on_receive);
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

        __sp<network::ITcpProtocol> _tcp_telnet_protocol;
        let_object_with_implement<network::ITcpProtocol, network::Telnet, __rtns>(&_tcp_telnet_protocol);
        if (_tcp_telnet_protocol) printf("%s\n", "let_object Telnet success");

        _server->set_timeout(10000);
        _server->set_protocol(_tcp_telnet_protocol);
        _server->set_callback(_callback_key);
        _server->listen("0.0.0.0", 1024, 1024);
        _server->wait_for_event(_poll);

        _poll->event_loop();
        return true;
    }

    ret on_connect(network::ITcpConnection* _conn, bool _error)
    {
        printf("%s\n", "TelnetTest on_connect");
        return ret_success;
    }

    ret on_close(network::ITcpConnection* _conn, bool _accord)
    {
        printf("%s\n", "TelnetTest on_close");
        return ret_success;
    }

    ret on_error(network::ITcpConnection* _conn, int _error)
    {
        return ret_success;
    }

    ret on_receive(network::ITcpConnection* _conn, const byte* _buffer, int _size)
    {
        printf("【recv】%.*s\n", _size, (char*)_buffer);
        if (_conn)
        {
            _conn->send_buffer(_buffer, _size);
        }
        return ret_success;
    }
};