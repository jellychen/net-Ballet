#ifndef _include_common_factory_h
#define _include_common_factory_h
#include "Include/Def/Inct.h"
#include "Include/Common/IRegister.h"

#define _implement_bind_factory(_i)                                                                 \
    struct _i##_factory                                                                             \
    {                                                                                               \
        static bool create(const _guid& _d, void** ppOut)                                           \
        {                                                                                           \
            if_return(ppOut == nullptr_t, ret_args_error);                                          \
            *ppOut = nullptr_t;                                                                     \
            CObjectImplement<_i>* _object = CObjectImplement<_i>::create_instance();                \
            if_return(_object == nullptr_t, ret_fail);                                              \
            _object->add_ref();                                                                     \
            _object->convert(_d, ppOut);                                                            \
            _object->del_ref();                                                                     \
            return true;                                                                            \
        }                                                                                           \
    };

typedef bool(*p_create)(const _guid&, void**);

#define _implement_factory(_i)                                                                      \
    struct _i##_factory                                                                             \
    {                                                                                               \
        static bool create(const _guid& _d, void** ppOut)                                           \
        {                                                                                           \
            if_return(ppOut == nullptr_t, ret_args_error);                                          \
            *ppOut = nullptr_t;                                                                     \
            CObjectImplement<_i>* _object = CObjectImplement<_i>::create_instance();                \
            if_return(_object == nullptr_t, ret_fail);                                              \
            _object->add_ref();                                                                     \
            _object->convert(_d, ppOut);                                                            \
            _object->del_ref();                                                                     \
            return true;                                                                            \
        }                                                                                           \
    };

#define _implement_register_object(_i, _c, _reg)                                                    \
    namespace _c##_factory                                                                          \
    {                                                                                               \
        static bool create(const _guid& _d, void** ppOut)                                           \
        {                                                                                           \
            if_return(ppOut == nullptr_t, ret_args_error);                                          \
            *ppOut = nullptr_t;                                                                     \
            CObjectImplement<_c>* _object = CObjectImplement<_c>::create_instance();                \
            if_return(_object == nullptr_t, ret_fail);                                              \
            _object->add_ref();                                                                     \
            _object->convert(_d, ppOut);                                                            \
            _object->del_ref();                                                                     \
            return true;                                                                            \
        }                                                                                           \
        inline bool register_factory(::common::IRegister* _r)                                       \
        {                                                                                           \
            if_return(_r == nullptr_t, false);                                                      \
            _reg::register_factory(_type_id(_c), &create);                                          \
            _r->register_object(_type_id(_i), _type_id(_c));                                        \
            return true;                                                                            \
        }                                                                                           \
    };

#define _implement_register_ref_object(_i, _c, _ref, _reg)                                          \
    namespace _c##_factory                                                                          \
    {                                                                                               \
        static bool create(const _guid& _d, void** ppOut)                                           \
        {                                                                                           \
            if_return(ppOut == nullptr_t, ret_args_error);                                          \
            *ppOut = nullptr_t;                                                                     \
            typedef ObjectInternalInherit<_ref, _c> _t;                                             \
            CObjectImplement<_t >* _object = CObjectImplement<_t >::create_instance();              \
            if_return(_object == nullptr_t, ret_fail);                                              \
            _object->add_ref();                                                                     \
            _object->convert(_d, ppOut);                                                            \
            _object->del_ref();                                                                     \
            return true;                                                                            \
        }                                                                                           \
        inline bool register_factory(::common::IRegister* _r)                                       \
        {                                                                                           \
            if_return(_r == nullptr_t, false);                                                      \
            _reg::register_factory(_type_id(_c), &create);                                          \
            _r->register_object(_type_id(_i), _type_id(_c));                                        \
            return true;                                                                            \
        }                                                                                           \
    };

#define _implement_register_service(_i, _c, _reg)                                                   \
    namespace _c##_factory                                                                          \
    {                                                                                               \
        static bool create(const _guid& _d, void** ppOut)                                           \
        {                                                                                           \
            if_return(ppOut == nullptr_t, ret_args_error);                                          \
            *ppOut = nullptr_t;                                                                     \
            CObjectImplement<_c>* _object = CObjectImplement<_c>::create_instance();                \
            if_return(_object == nullptr_t, ret_fail);                                              \
            _object->add_ref();                                                                     \
            _object->convert(_d, ppOut);                                                            \
            _object->del_ref();                                                                     \
            return true;                                                                            \
        }                                                                                           \
        inline bool register_factory(::common::IRegister* _r)                                       \
        {                                                                                           \
            if_return(_r == nullptr_t, false);                                                      \
            _reg::register_factory(_type_id(_c), &create);                                          \
            _r->register_service(_type_id(_i), _type_id(_c));                                       \
            return true;                                                                            \
        }                                                                                           \
    };

#define _implement_register_ref_service(_i, _c, _ref, _reg)                                         \
    namespace _c##_factory                                                                          \
    {                                                                                               \
        static bool create(const _guid& _d, void** ppOut)                                           \
        {                                                                                           \
            if_return(ppOut == nullptr_t, ret_args_error);                                          \
            *ppOut = nullptr_t;                                                                     \
            typedef ObjectInternalInherit<_ref, _c> _t;                                             \
            CObjectImplement<_t >* _object = CObjectImplement<_t >::create_instance();              \
            if_return(_object == nullptr_t, ret_fail);                                              \
            _object->add_ref();                                                                     \
            _object->convert(_d, ppOut);                                                            \
            _object->del_ref();                                                                     \
            return true;                                                                            \
        }                                                                                           \
        inline bool register_factory(::common::IRegister* _r)                                       \
        {                                                                                           \
            if_return(_r == nullptr_t, false);                                                      \
            _reg::register_factory(_type_id(_c), &create);                                          \
            _r->register_service(_type_id(_i), _type_id(_c));                                       \
            return true;                                                                            \
        }                                                                                           \
    };

#define __register(_c, _r)                                                                          \
    _c##_factory::register_factory((::common::IRegister*)(_r));


template <typename _i, typename _c, typename _ref> ret let_object_with_implement(_i** _pptr)
{
    if_return(_pptr == nullptr_t, ret_args_error);
    *_pptr = nullptr_t;

    typedef ObjectInternalInherit<_ref, _c> _t;
    CObjectImplement<_t>* _object = CObjectImplement<_t>::create_instance();
    if_return(_object == nullptr_t, ret_fail);
    _object->add_ref();
    _object->convert(_type_id(_i), (void**)_pptr);
    _object->del_ref();
    return ret_success;
}

#endif//_include_common_factory_h