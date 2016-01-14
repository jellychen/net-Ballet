#ifndef service_servicemanage_h
#define service_servicemanage_h
#include "Include/Def/Inct.h"
#include "Include/Common/Factory.h"
#include "Include/Service/IServiceManage.h"
#include "Module.h"

namespace service
{
    _implement_bind(ServiceManage, 5B0D19D5A3F54651, 87530DF1997EC30D)
    _support IServiceManage
    {
        _implement_support_begin()
        _implement_support(IServiceManage)
        _implement_support_end()

        typedef std::map<long, __sp<IService> > service_map_t;
    public:
        ServiceManage();
        virtual ~ServiceManage();
        bool internal_init();

    public:
        ret init            ();
        ret start           (bool _deamon);
        ret attach_service  (IService* _service);
        ret detach_service  (IService* _service);

    public:
        ret fork_service    (IService* _service);
        ret kill_all        ();
        ret find_exit       (pid_t _pid, bool _fork);
        ret reload_all      ();

    private:
        bool                exit_;
        service_map_t       service_pool_;
        service_map_t       progress_service_pool_;
    };
    _implement_register_ref_object(IServiceManage, ServiceManage, __rtns, __module_register)
}
#endif//service_servicemanage_h