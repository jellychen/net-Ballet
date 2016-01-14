#ifndef _include_def_def_h
#define _include_def_def_h
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <iostream>

#define if_break(op)                                                        \
    if (!!(op)) break;
#define if_continue(op)                                                     \
    if (!!(op)) continue;
#define if_return(op, _ret)                                                 \
    if (!!(op)) return _ret;
#define if_return_void(op)                                                  \
    if (!!(op)) return;
#define if_do(op, _do)                                                      \
    if (!!(op)) _do;
#define safe_ptr(ptr)                                                       \
    assert(ptr); ptr
#define safe_delete(ptr)                                                    \
    if (NULL != ptr) delete ptr; ptr = NULL;
#define safe_delete_array(ptr)                                              \
    if (NULL != ptr) delete[] ptr; ptr = NULL;

#ifdef __attribute_export
    #define __api   extern "C" __attribute__((visibility("default")))
#else
    #define __api   extern "C"
#endif//__attribute__export

struct nullptr_type_t
{
    template<typename t>  operator t*() const{return 0;}
};

inline nullptr_type_t __get_nullptr()
{
    static nullptr_type_t _t = {};
    return _t; 
}

#define nullptr_t                   __get_nullptr()
#define f_nullptr_t                 0

#define byte                        unsigned char
#define ret                         int
#define ret_success                 0
#define ret_fail                    1
#define ret_args_error              2
#define ret_inner_error             3
#define ret_no_implement            4
#define ret_no_support              5
#define ret_define_start            1024

#define __success(_ret)             (_ret == 0)
#define __fail(_ret)                (_ret != 0)
#define __bool_ret(_b)              (_b ? ret_success: ret_fail)
#define def_function(_name, _args)  virtual ret _name _args  =0;

template<typename t> t& __get_instance()
{
    static t _t; return _t;    
}
#endif//_include_def_def_h