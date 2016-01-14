#include "Include/Common/Factory.h"
#include "Include/Common/IRegister.h"
#include "Module.h"
#include "EventPoll.h"
#include "TcpClient.h"
#include "TcpServer.h"
#include "TcpConnection.h"
#include "HttpServer.h"
#include "HttpConnection.h"

__api ret __module_init()
{
    return ret_success;
}

__api ret __module_factory(const _guid& _u, const _guid& _d, void** ppOut)
{
    if_return(ppOut == nullptr_t, ret_args_error);
    return network::__module_register::create(_u, _d, ppOut) ? ret_success : ret_fail;
}

__api ret __module_register(const common::IRegister* _reg)
{
    if_return(_reg == nullptr_t, ret_args_error);
    __register(network::EventPoll,      _reg);
    __register(network::TcpClient,      _reg);
    __register(network::TcpServer,		_reg);
    __register(network::TcpConnection,	_reg);
    __register(network::HttpServer,     _reg);
    __register(network::HttpConnection, _reg);
    return ret_success;
}