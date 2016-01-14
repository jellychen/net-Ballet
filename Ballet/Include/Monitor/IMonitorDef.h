#ifndef _include_monitor_monitordef_h
#define _include_monitor_monitordef_h
#include "Include/Def/Inct.h"

namespace monitor
{
	const int8_t service_heartbeat		= 0;
	const int8_t service_register 		= 1;
	const int8_t service_unregister		= 2;
	const int8_t service_query			= 3;

	__serialize_def_begin(s_monitor_t)
	s_uint8_t				_mode;
	s_string_t				_serivce;
	s_list_t<s_string_t>	_addr;
	__serialize_def_end_3(_mode, _serivce, _addr)
}
#endif//_include_monitor_monitordef_h