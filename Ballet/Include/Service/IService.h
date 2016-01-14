#ifndef _include_service_service_h
#define _include_service_service_h
#include "Include/Def/Inct.h"
#include "Include/Service/IServiceCallback.h"
#include <string>

namespace service
{
    _interface_inherit_def(IService, IObject)
    {
        def_function(init,                  ());
        def_function(is_available,          (bool*));
        def_function(set_name,              (const char*));
        def_function(get_name,              (std::string*));
        def_function(set_progress_count,    (int _count));
        def_function(get_progress_count,    (int* _count));
        def_function(set_callback,          (IServiceCallback*));
        def_function(get_callback,          (IServiceCallback**));

        def_function(on_start,              ());
        def_function(on_progress_init,      ());
        def_function(on_progress_run,       ());
        def_function(on_progress_release,   ());
        def_function(on_release,            ());
    };
    _interface_inherit_bind(IService, IObject, service, 878F626249E8431f, A4FE46E9E9254732)
}
#endif//_include_service_service_h