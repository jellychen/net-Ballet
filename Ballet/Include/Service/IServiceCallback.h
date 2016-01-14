#ifndef _include_service_servicecallback_h
#define _include_service_servicecallback_h
#include "Include/Def/Inct.h"

namespace service
{
    _interface IService;
    _interface_inherit_def(IServiceCallback, ICallback)
    {
        def_function(on_init,   (IService* _service));
        def_function(on_uninit, (IService* _service));
        def_function(on_start,  (IService* _service));
        def_function(on_run,    (IService* _service));
        def_function(on_stoped, (IService* _service));
    };
    _interface_inherit_bind(IServiceCallback, ICallback, service, A3C239D0D42F47CF, 90BA0EC86288AD61)

    __callback_sink_begin(CServiceCallback)
    __callback_function(ret, on_init,   (IService* _service), (_service))
    __callback_function(ret, on_uninit, (IService* _service), (_service))
    __callback_function(ret, on_start,  (IService* _service), (_service))
    __callback_function(ret, on_run,    (IService* _service), (_service))
    __callback_function(ret, on_stoped, (IService* _service), (_service))
    __callback_sink_end()
    __callback_sink_ptr(CServiceCallback, IServiceCallback)
}
#endif//_include_service_servicecallback_h