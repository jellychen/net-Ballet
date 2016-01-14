#ifndef common_loader_h
#define common_loader_h
#include <set>
#include <string>

#include "Include/Def/Inct.h"
#include "Include/Common/IRegister.h"
#include "Define.h"

namespace common
{
    typedef std::set<std::string> _t_set_module;

    class Loader
    {
    public:
        static Loader& get_instance()
        {
            static Loader _instance;
            return _instance;
        }

    public:
        bool load_module(const char* _module);

    private:
        _t_set_module   _pool;
    };
}
#endif//common_loader_h