#ifndef common_register_h
#define common_register_h
#include "Include/Def/Inct.h"
#include "Include/Common/IRegister.h"
#include "Define.h"
#include <map>

namespace common
{
    class Register: public IRegister
    {
        struct RegisterItem
        {
            _guid       _g;
            p_factory   _factory;
        };
        typedef std::map<_guid, RegisterItem> _t_map_pool;

    public:
        static Register& get_instance()
        {
            static Register _object;
            return _object;
        }

    public:
        bool let_factory    (p_factory _factory);
        bool let_object     (const _guid& _d, void** _out);
        bool let_service    (const _guid& _d, void** _out);

    public:
        ret register_object (const _guid& _i, const _guid& _g);
        ret register_service(const _guid& _i, const _guid& _g);
        
    private:
        p_factory           _current_factory;
        _t_map_pool         _object_pool;
        _t_map_pool         _service_pool;
    };
}
#endif//common_register_h