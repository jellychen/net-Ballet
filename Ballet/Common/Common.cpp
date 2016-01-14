#include "Common.h"
#include "Loader.h"
#include "Register.h"
#include "Module.h"

namespace common
{
    ret init()
    {
        ::__module_init();
        Register::get_instance().let_factory(::__module_factory);
        ::__module_register((IRegister*)(&(Register::get_instance())));

        return ret_success;
    }

    ret uninit()
    {
        return ret_success;
    }

    ret load_module(const char* _m)
    {
        if_return(nullptr_t == _m, ret_args_error);
        return __bool_ret(Loader::get_instance().load_module(_m));
    }

    ret load_config(const char* _c)
    {
        return ret_success;
    }

    ret let_object(const _guid& _d, void** ppOut)
    {
        return Register::get_instance().let_object(_d, ppOut) ? ret_success : ret_fail;
    }

    ret let_service(const _guid& _d, void** ppOut)
    {
        return ret_success;
    }
}