#ifndef _include_service_servicemanage_h
#define _include_service_servicemanage_h
#include "Include/Def/Inct.h"
#include "Include/Service/IService.h"

namespace service
{
    _interface_inherit_def(IServiceManage, IObject)
    {
        def_function(init,          ());
        def_function(start,         (bool));
        def_function(attach_service,(IService*));
        def_function(detach_service,(IService*));
    };
    _interface_inherit_bind(IServiceManage, IObject, service, F09B466D1D0C474C, BC31582E76640491)
}
#endif//_include_service_servicemanage_h