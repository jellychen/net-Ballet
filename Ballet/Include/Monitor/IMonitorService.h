#ifndef _include_monitor_monitordef_h
#define _include_monitor_monitordef_h
#include "Include/Def/Inct.h"
#include "Include/Monitor/IMonitorDef.h"

namespace monitor
{
	_interface_inherit_def(IMonitorService, IObject)
    {
        def_function(init,	());
    };
    _interface_inherit_bind(IMonitorService, IObject, monitor, 90DAF67C1BE94229, 8A189D627162AE12)
}
#endif//_include_monitor_monitordef_h