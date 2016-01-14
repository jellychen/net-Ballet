#include "Include/Common/Factory.h"
#include "Include/Common/IRegister.h"
#include "Module.h"

#include "Service.h"
#include "ServiceManage.h"

__api ret __module_init()
{
    return ret_success;
}

__api ret __module_factory(const _guid& _u, const _guid& _d, void** ppOut)
{
    if_return(ppOut == nullptr_t, ret_args_error);
    return service::__module_register::create(_u, _d, ppOut) ? ret_success : ret_fail;
}

__api ret __module_register(const common::IRegister* _reg)
{
    if_return(_reg == nullptr_t, ret_args_error);
    __register(service::Service,        _reg);
    __register(service::ServiceManage,  _reg);
    return ret_success;
}