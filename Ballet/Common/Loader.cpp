#include "Loader.h"
#include "Register.h"

namespace common
{
    bool Loader::load_module(const char* _module)
    {
        if_return(_module == nullptr_t, false);
        _t_set_module::iterator _it = this->_pool.find(_module);
        if_return(this->_pool.end() != _it, true);

        // load so
        void* _mdl = ::dlopen(_module, RTLD_NOW);
        if (0 == _mdl)
        {
            printf("%s\n", ::dlerror());
        }
        if_return(_mdl == nullptr_t, false);

        // init function
        void* _init_func = ::dlsym(_mdl, "__module_init");
        if (_init_func != nullptr_t)
        {
            ((p_init)(_init_func))();
        }

        // factory function
        void* _factory_func = ::dlsym(_mdl, "__module_factory");
        if (_factory_func != nullptr_t)
        {
            Register::get_instance().let_factory((p_factory)(_factory_func));
            void* _register_func = ::dlsym(_mdl, "__module_register");
            if (_register_func != nullptr_t)
            {
                ((p_register)(_register_func))(&(Register::get_instance()));
            }
        }

        this->_pool.insert(_module);
        return true;
    }
}