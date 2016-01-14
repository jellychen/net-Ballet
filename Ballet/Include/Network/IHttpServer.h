#ifndef _include_network_httpserver_h
#define _include_network_httpserver_h
#include "Include/Def/Inct.h"
#include "Include/Network/ITcpServer.h"
#include "Include/Network/IHttpCallback.h"

namespace network
{
    _interface_inherit_def(IHttpServer, ITcpServer)
    {
        def_function(set_http_callback, (IHttpCallback*));
        def_function(get_http_callback, (IHttpCallback**));
    };
    _interface_inherit_bind(IHttpServer, ITcpServer, network, B66C672DA9994a99, B33871CC04925893)
}
#endif//_include_network_httpserver_h