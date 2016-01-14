#include "Include/Common/Factory.h"
#include "Include/Common/IRegister.h"
#include "Thread.h"
#include "ThreadSolo.h"
#include "ThreadPool.h"
#include "Module.h"

__api ret __module_init()
{
    return ret_success;
}

__api ret __module_factory(const _guid& _u, const _guid& _d, void** ppOut)
{
    if_return(ppOut == nullptr_t, ret_args_error);
    return common::__module_register::create(_u, _d, ppOut) ? ret_success : ret_fail;
}

__api ret __module_register(const common::IRegister* _reg)
{
    if_return(_reg == nullptr_t, ret_args_error);
    __register(common::Thread,      _reg);
    __register(common::ThreadSolo,  _reg);
    __register(common::ThreadPool,  _reg);
    return ret_success;
}