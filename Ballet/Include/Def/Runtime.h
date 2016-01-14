#ifndef _include_def_runtime_h
#define _include_def_runtime_h
#include "Def.h"
#include "Guid.h"

#define _implement_def(_c)              typedef _c __self;   
#define _interface                      struct
#define _implement                      struct
#define _extend                         :public
#define _support                        ,
#define _guid_field(_d)                 ((int64_t)_d)
#define _interface_def(_i)              struct _i
#define _interface_inherit_def(_i, _p)  struct _i :public _p

class _object_internal_init
{ 
public:
    virtual bool internal_init          (){return true;}
    virtual bool internal_uninit        (){return true;}
};

template<typename t> struct _implement_guid_
{
    static const _guid _guid_d;
};
#define _type_id(_o) _implement_guid_<_o>::_guid_d

template<typename t> struct _implement_rtti_
{   
    typedef bool (*_t_convert_func)(t*, const _guid&, void**);
    static const _t_convert_func _p_function;
};
#define _type_convert(_o) _implement_rtti_<_o>::_p_function

#define _interface_bind(_i, _d1, _d2)                                                                                           \
    _interface _i; template<>                                                                                                   \
    __attribute__((weak)) const _guid _implement_guid_<_i>::_guid_d = {_guid_field(0x##_d1), _guid_field(0x##_d2)};

#define _interface_base_bind(_i, _space, _d1, _d2)                                                                              \
    template<>                                                                                                                  \
    __attribute__((weak)) const _guid _implement_guid_<_i>::_guid_d = {_guid_field(0x##_d1), _guid_field(0x##_d2)};             \
    namespace _i##_runtime                                                                                                      \
    {                                                                                                                           \
        inline bool __runtime_convert(_i* ptr, const _guid& _d, void** ppOut)                                                   \
        {                                                                                                                       \
            if_return(_type_id(_i) != _d, false);                                                                               \
            *ppOut = ptr; ptr->add_ref();                                                                                       \
            return true;                                                                                                        \
        }                                                                                                                       \
    }                                                                                                                           \
    template<>                                                                                                                  \
    __attribute__((weak)) const _implement_rtti_<_i>::_t_convert_func                                                           \
        _implement_rtti_<_i>::_p_function = _space::_i##_runtime::__runtime_convert;

#define _interface_base_bind_without_space(_i, _d1, _d2)                                                                        \
    template<>                                                                                                                  \
    __attribute__((weak)) const _guid _implement_guid_<_i>::_guid_d = {_guid_field(0x##_d1), _guid_field(0x##_d2)};             \
    namespace _i##_runtime                                                                                                      \
    {                                                                                                                           \
        inline bool __runtime_convert(_i* ptr, const _guid& _d, void** ppOut)                                                   \
        {                                                                                                                       \
            if_return(_type_id(_i) != _d, false);                                                                               \
            *ppOut = ptr; ptr->add_ref();                                                                                       \
            return true;                                                                                                        \
        }                                                                                                                       \
    }                                                                                                                           \
    template<>                                                                                                                  \
    __attribute__((weak)) const _implement_rtti_<_i>::_t_convert_func                                                           \
        _implement_rtti_<_i>::_p_function = _i##_runtime::__runtime_convert;

#define _interface_inherit_bind(_i, _p, _space, _d1, _d2)                                                                       \
    _interface _i; template<>                                                                                                   \
    __attribute__((weak)) const _guid _implement_guid_<_i>::_guid_d = {_guid_field(0x##_d1), _guid_field(0x##_d2)};             \
    namespace _i##_runtime                                                                                                      \
    {                                                                                                                           \
        inline bool __runtime_convert(_i* ptr, const _guid& _d, void** ppOut)                                                   \
        {                                                                                                                       \
            if (_type_id(_i) == _d)                                                                                             \
            {                                                                                                                   \
                *ppOut = ptr; ptr->add_ref(); return true;                                                                      \
            }                                                                                                                   \
            else                                                                                                                \
            {                                                                                                                   \
                return _p##_runtime::__runtime_convert(static_cast<_p*>(ptr), _d, ppOut);                                       \
            }                                                                                                                   \
        }                                                                                                                       \
    }                                                                                                                           \
    template<>                                                                                                                  \
    __attribute__((weak)) const _implement_rtti_<_i>::_t_convert_func                                                           \
        _implement_rtti_<_i>::_p_function = _space::_i##_runtime::__runtime_convert;                                                                                                                \

#define _interface_inherit_bind_without_space(_i, _p, _d1, _d2)                                                                 \
    _interface _i; template<>                                                                                                   \
    __attribute__((weak)) const _guid _implement_guid_<_i>::_guid_d = {_guid_field(0x##_d1), _guid_field(0x##_d2)};             \
    namespace _i##_runtime                                                                                                      \
    {                                                                                                                           \
        inline bool __runtime_convert(_i* ptr, const _guid& _d, void** ppOut)                                                   \
        {                                                                                                                       \
            if (_type_id(_i) == _d)                                                                                             \
            {                                                                                                                   \
                *ppOut = ptr; ptr->add_ref(); return true;                                                                      \
            }                                                                                                                   \
            else                                                                                                                \
            {                                                                                                                   \
                return _p##_runtime::__runtime_convert(static_cast<_p*>(ptr), _d, ppOut);                                       \
            }                                                                                                                   \
        }                                                                                                                       \
    }                                                                                                                           \
    template<>                                                                                                                  \
    __attribute__((weak)) const _implement_rtti_<_i>::_t_convert_func                                                           \
        _implement_rtti_<_i>::_p_function = _i##_runtime::__runtime_convert;

#define _interface_empty_inherit_def(_i, _p, _space, _d1, _d2)                                                                  \
    struct _i :public _p{};                                                                                                     \
    template<>                                                                                                                  \
    __attribute__((weak)) const _guid _implement_guid_<_i>::_guid_d = {_guid_field(0x##_d1), _guid_field(0x##_d2)};             \
    namespace _i##_runtime                                                                                                      \
    {                                                                                                                           \
        inline bool __runtime_convert(_i* ptr, const _guid& _d, void** ppOut)                                                   \
        {                                                                                                                       \
            if (_type_id(_i) == _d)                                                                                             \
            {                                                                                                                   \
                *ppOut = ptr; ptr->add_ref(); return true;                                                                      \
            }                                                                                                                   \
            else                                                                                                                \
            {                                                                                                                   \
                return _p##_runtime::__runtime_convert(static_cast<_p*>(ptr), _d, ppOut);                                       \
            }                                                                                                                   \
        }                                                                                                                       \
    }                                                                                                                           \
    template<>                                                                                                                  \
    __attribute__((weak)) const _implement_rtti_<_i>::_t_convert_func                                                           \
        _implement_rtti_<_i>::_p_function = _space::_i##_runtime::__runtime_convert;

#define _interface_empty_inherit_def_without_space(_i, _p, _d1, _d2)                                                            \
    struct _i :public _p{};                                                                                                     \
    template<>                                                                                                                  \
    __attribute__((weak)) const _guid _implement_guid_<_i>::_guid_d = {_guid_field(0x##_d1), _guid_field(0x##_d2)};             \
    namespace _i##_runtime                                                                                                      \
    {                                                                                                                           \
        inline bool __runtime_convert(_i* ptr, const _guid& _d, void** ppOut)                                                   \
        {                                                                                                                       \
            if (_type_id(_i) == _d)                                                                                             \
            {                                                                                                                   \
                *ppOut = ptr; ptr->add_ref(); return true;                                                                      \
            }                                                                                                                   \
            else                                                                                                                \
            {                                                                                                                   \
                return _p##_runtime::__runtime_convert(static_cast<_p*>(ptr), _d, ppOut);                                       \
            }                                                                                                                   \
        }                                                                                                                       \
    }                                                                                                                           \
    template<>                                                                                                                  \
    __attribute__((weak)) const _implement_rtti_<_i>::_t_convert_func                                                           \
        _implement_rtti_<_i>::_p_function = _i##_runtime::__runtime_convert;

#define _implement_bind(_i, _d1, _d2)                                                                                           \
    _implement _i; template<>                                                                                                   \
    __attribute__((weak)) const _guid _implement_guid_<_i>::_guid_d = {_guid_field(0x##_d1), _guid_field(0x##_d2)};             \
    _implement _i : public _object_internal_init

#define _implement_inherit_bind(_base, _i, _d1, _d2)                                                                                           \
    _implement _i; template<>                                                                                                   \
    __attribute__((weak)) const _guid _implement_guid_<_i>::_guid_d = {_guid_field(0x##_d1), _guid_field(0x##_d2)};             \
    _implement _i : public _base

#define _implement_support_begin()                                                                                              \
    public: ret convert(const _guid& _d, void** ppOut)                                                                          \
    {                                                                                                                           \
        if (NULL == ppOut) return ret_args_error;                                                                               \
        *ppOut = NULL;

#define _implement_support(_i)                                                                                                  \
        if_return(_i##_runtime::__runtime_convert(static_cast<_i*>(this), _d, ppOut), ret_success);
        
#define _implement_support_pointer(_p)                                                                                          \
        if_return(_p->convert(_d, ppOut));

#define _implement_support_end()                                                                                                \
        return ret_fail;                                                                                                        \
    }                                                                                                                           \
    template<typename _i> _i* __get_i_entity()                                                                                  \
    {                                                                                                                           \
        _i* _ptr = nullptr_t;                                                                                                   \
        this->convert(_type_id(_i), (void**)&_ptr);                                                                             \
        if (_ptr != nullptr_t) _ptr->del_ref();                                                                                 \
        return _ptr;                                                                                                            \
    }
#endif//_include_def_runtime_h