#ifndef common_define_h
#define common_define_h
#include "Include/Def/Inct.h"

namespace common
{
    typedef ret (*p_init)       ();
    typedef ret (*p_factory)    (const _guid&, const _guid&, void**);
    typedef ret (*p_register)   (const IRegister*);
}
#endif//common_define_h