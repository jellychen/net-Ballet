#ifndef _include_def_object_h
#define _include_def_object_h
#include "Def.h"
#include "Guid.h"
#include "Runtime.h"

_interface_def(IObject)
{
    def_function(add_ref,       ());
    def_function(del_ref,       ());
    def_function(hascode,       (long*));
    def_function(convert,       (const _guid&, void**));
    def_function(weak_convert,  (const _guid&, void**));
};
_interface_base_bind_without_space(IObject, 0000000000000000, 0000000000000000)
#endif//_include_def_object_h