#ifndef _include_common_module_h
#define _include_common_module_h
#define __attribute_export

#include <map>
#include <vector>
#include "Include/Def/Inct.h"

__api ret __module_init();
__api ret __module_factory(const _guid& _u, const _guid& _d, void** ppOut);
__api ret __module_register(const common::IRegister* _reg);

namespace common
{
    class __module_register
    {
        typedef bool (*p_create)(const _guid&, void**);
        typedef std::map<_guid, p_create> _t_map_creator;
        
    public:
        static __module_register& get_instance()
        {
            static __module_register _object;
            return _object;
        }

        static bool register_factory(const _guid& _d, p_create _p)
        {
            if_return(_p == nullptr_t, false);
            __module_register::get_instance()._pool[_d] = _p;
            return true;
        }

        static bool create(const _guid& _u, const _guid& _d, void** ppOut)
        {
            if_return(ppOut == nullptr_t, false);
            *ppOut = nullptr_t;

            __module_register& _reg = __module_register::get_instance();
            _t_map_creator::iterator _it = _reg._pool.find(_d);
            if (_reg._pool.end() != _it && _it->second)
            {
                p_create pfunc = _it->second;
                pfunc(_u, ppOut);
            }
            return true;
        }
    public:
        _t_map_creator  _pool;
    };
}
#endif//_include_common_module_h