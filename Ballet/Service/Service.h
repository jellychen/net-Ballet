#ifndef service_service_h
#define service_service_h
#include "Include/Def/Inct.h"
#include "Include/Common/Factory.h"
#include "Include/Service/IService.h"
#include "Module.h"

namespace service
{
    _implement_bind(Service, 716313B93E9344ee, 994B66A48591A847)
    _support IService
    {
        _implement_support_begin()
        _implement_support(IService)
        _implement_support_end()
    public:
        Service();
        virtual ~Service();
        bool internal_init();

    public:
        ret init                ();
        ret is_available        (bool*);
        ret set_name            (const char*);
        ret get_name            (std::string*);
        ret set_progress_count  (int _count);
        ret get_progress_count  (int* _count);
        ret set_callback        (IServiceCallback*);
        ret get_callback        (IServiceCallback**);

        ret on_start            ();
        ret on_progress_init    ();
        ret on_progress_run     ();
        ret on_progress_release ();
        ret on_release          ();
    public:
        int                     progress_count_;
        std::string             service_name_;
        __sp<IServiceCallback>  callback_;
    };
    _implement_register_ref_object(IService, Service, __rtns, __module_register)
}
#endif//network_eventpoll_h