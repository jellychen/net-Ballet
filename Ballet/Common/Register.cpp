#include "Register.h"

namespace common
{
    bool Register::let_factory(p_factory _factory)
    {
        this->_current_factory = _factory;
        return true;
    }

    ret Register::register_object(const _guid& _i, const _guid& _g)
    {
        if_return(this->_current_factory == nullptr_t, ret_fail);
        RegisterItem _item = {_g, this->_current_factory};
        this->_object_pool[_i] = _item;
        return ret_success;
    }

    ret Register::register_service(const _guid& _i, const _guid& _g)
    {
        if_return(this->_current_factory == nullptr_t, ret_fail);
        RegisterItem _item = {_g, this->_current_factory};
        this->_service_pool[_i] = _item;
        return ret_success;
    }

    bool Register::let_object(const _guid& _d, void** _out)
    {
        if_return(_out == nullptr_t, false);
        *_out = nullptr_t;
        _t_map_pool::iterator _it = this->_object_pool.find(_d);

        if (this->_object_pool.end() != _it)
        {
            p_factory _func = _it->second._factory;
            _func(_d, _it->second._g, _out);
        }
        return true;
    }

    bool Register::let_service(const _guid& _d, void** _out)
    {
        if_return(_out == nullptr_t, ret_args_error);
        *_out = nullptr_t;
        return true;
    }
}