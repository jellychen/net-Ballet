#include "Include/Def/Inct.h"
#include "Include/Common/Inct.h"
#include "Include/Network/IEventPoll.h"
#include "Include/Network/ITcpServer.h"
#include "Include/Network/ITcpProtocol.h"
#include "Include/Network/ITcpCallback.h"
#include "Include/Network/ITcpConnection.h"
#include "Include/Network/IHttpServer.h"
#include "Include/Network/IHttpConnection.h"


using namespace network;
class HttpServerTest
{
public:
    std::string _web_str;
    CHttpCallbackPtr<HttpServerTest> _callback_key;
    int _http_times;
    __sp<network::IEventPoll> _poll;

    HttpServerTest():_callback_key(this),_http_times(0)
    {
        _callback_key->hook_on_http_connect(&HttpServerTest::on_http_connect);
        _callback_key->hook_on_http_begin(&HttpServerTest::on_http_begin);
        _callback_key->hook_on_http_header(&HttpServerTest::on_http_header);
        _callback_key->hook_on_http_body(&HttpServerTest::on_http_body);
        _callback_key->hook_on_http_complete(&HttpServerTest::on_http_complete);
        _callback_key->hook_on_http_error(&HttpServerTest::on_http_error);
        _callback_key->hook_on_http_close(&HttpServerTest::on_http_close);
        for (int i = 0; i < 200; ++i)
        {
            //_web_str += "guodongguodongguodongguodongguodongguodongguodongguodong";
        }
    }

    bool test()
    {
        
        common::let_object(_type_id(network::IEventPoll), (void**)&_poll);
        if (_poll) printf("%s\n", "let_object IEventPoll success");
        _poll->init();

        __sp<network::IHttpServer> _server;
        common::let_object(_type_id(network::IHttpServer), (void**)&_server);
        if (_server) printf("%s\n", "let_object IHttpServer success");

        _server->set_timeout(1000000);
        _server->set_max_write_buffer(500*1024*1024);
        _server->set_http_callback(_callback_key);
        _server->listen("0.0.0.0", 1024, 1024);
        _server->wait_for_event(_poll);

        _poll->event_loop();

        printf("%s\n", "end");
        return true;
    }

    ret on_http_connect(IHttpConnection* _conn, bool _success)
    {

        //printf("%s\n", "on_http_connect");
        return ret_success;
    }

    ret on_http_begin(IHttpConnection* _conn)
    {
        //printf("%s\n", "on_http_begin");
        return ret_success;
    }
    
    ret on_http_header(IHttpConnection* _conn)
    {
        //printf("%s\n", "on_http_header");
        return ret_success;
    }
    
    ret on_http_body(IHttpConnection* _conn, const byte*, int)
    {
        //printf("%s\n", "on_http_body");
        return ret_success;
    }
    
    
    ret on_http_complete(IHttpConnection* _conn)
    {
        //printf("%s\n", "on_http_complete");
        
        //std::string _str;
        //_conn->get_http_url(&_str);
        //printf("url => %s\n", _str.c_str());
        std::string _www_file = "/tmp/www";
        std::string _url;
        _conn->get_http_url(&_url);
        _www_file += _url;

        FILE* _file_handle = fopen(_www_file.c_str(), "r");
        if (nullptr_t == _file_handle)
        {
            _conn->http_respond_begin();
            _conn->http_respond_version(1, 1);
            _conn->http_respond_status(200, "ok");
            _conn->http_respond_keep_alive();
            _conn->http_respond_content_length(0);
            _conn->http_respond_header_complete();
            //_conn->http_respond_body((byte*)(_web_str.c_str()), _web_str.length());
            _conn->http_respond_complete(false);  
        }
        else
        {
            fseek(_file_handle, 0, SEEK_END);
            long _file_size = ftell(_file_handle);
            rewind(_file_handle);
            char* _file_buffer = (char*)malloc(sizeof(char) * _file_size);
            size_t _read_size = fread(_file_buffer, 1, _file_size, _file_handle);

            _conn->http_respond_begin();
            _conn->http_respond_version(1, 1);
            _conn->http_respond_status(200, "ok");
            _conn->http_respond_keep_alive();
            _conn->http_respond_content_length((int)_file_size);
            _conn->http_respond_header_complete();
            _conn->http_respond_body(_file_buffer, (int)_file_size);
            _conn->http_respond_complete(false);

            free(_file_buffer);
            fclose(_file_handle);
        }
        
        return ret_success;
    }
    
    ret on_http_error(IHttpConnection* _conn, int _error)
    {
        return ret_success;
    }
    
    ret on_http_close(IHttpConnection* _conn, bool _accord)
    {
        //printf("%s\n", "on_http_close");
        return ret_success;
    }
};