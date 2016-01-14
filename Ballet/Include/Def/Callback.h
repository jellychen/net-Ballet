#ifndef _include_def_callback_h
#define _include_def_callback_h
#include "Def.h"
#include "Guid.h"
#include "Runtime.h"
#include "Object.h"
#include "Reference.h"
#include "ObjectCreator.h"

_interface_inherit_def(ICallback, IObject)
{
    def_function(available, (bool*));
};
_interface_inherit_bind_without_space(ICallback, IObject, 0000000000000000, 0000000000000002)

#define __callback_sink_begin(sink_name)                                                            \
    template <typename _host, typename _sink>                                                       \
    class sink_name                                                                                 \
        :public _sink                                                                               \
        ,public _object_internal_init                                                               \
        ,public ObjectInternal<ref_thread_not_safe>                                                 \
    {                                                                                               \
        typedef sink_name _class_inner;                                                             \
    public:                                                                                         \
        ret convert(const _guid& _d, void** ppOut)                                                  \
        {                                                                                           \
            if_return(ppOut == nullptr_t, ret_args_error);                                          \
            *ppOut = nullptr_t;                                                                     \
            bool _ret = _type_convert(_sink)((_sink*)(this), _d, ppOut);                            \
            return _ret? ret_success: ret_fail;                                                     \
        }                                                                                           \
        ret available(bool* _p)                                                                     \
        {                                                                                           \
            if_return(_p == nullptr_t, ret_args_error);                                             \
            *_p = (this->_phost != nullptr_t);                                                      \
            return ret_success;                                                                     \
        }                                                                                           \
        ret weak_convert(const _guid&, void** ppOut)                                                \
        {                                                                                           \
            if_return(nullptr_t == ppOut, ret_args_error);                                          \
            *ppOut = nullptr_t;                                                                     \
            return ret_fail;                                                                        \
        }                                                                                           \
    public:                                                                                         \
        sink_name()                                                                                 \
        {                                                                                           \
            this->_phost = nullptr_t;                                                               \
        }                                                                                           \
    public:                                                                                         \
        _host* _phost;

#define __callback_sink_inherit_begin(sink_name, _parent)                                           \
    template <typename _host, typename _sink>                                                       \
    class sink_name : public _parent<_host, _sink>                                                  \
    {                                                                                               \
        typedef _parent<_host, _sink> _class_inner;

template<typename t> struct __callback_ptr_self_init
{
    __callback_ptr_self_init()
    {
        _t = (t)0;
    }
    t _t;
};

#define __callback_function(_ret, _name, _param, _args)                                             \
    public:                                                                                         \
        typedef ret (_host::*__pf##_name) _param;                                                   \
        __callback_ptr_self_init<__pf##_name > _pf##_name##_ptr;                                    \
        bool hook_##_name(__pf##_name pf##_name)                                                    \
        {                                                                                           \
            _pf##_name##_ptr._t = pf##_name;                                                        \
            return true;                                                                            \
        }                                                                                           \
        bool unhook_##_name()                                                                       \
        {                                                                                           \
            _pf##_name##_ptr._t = f_nullptr_t;                                                      \
            return true;                                                                            \
        }                                                                                           \
        _ret _name _param                                                                           \
        {                                                                                           \
            if_return(_class_inner::_phost == f_nullptr_t                                           \
                || _pf##_name##_ptr._t == f_nullptr_t, _ret(0));                                    \
            return (_class_inner::_phost->*(_pf##_name##_ptr._t)) _args;                            \
        }

#define __callback_sink_end()                                                                       \
    };

#define __callback_sink_ptr(sink_name, _sink)                                                       \
    template <typename _host> class sink_name##Ptr                                                  \
    {                                                                                               \
        typedef sink_name<_host, _sink> _i_type;                                                    \
    public:                                                                                         \
        sink_name##Ptr(const _host* phost = nullptr_t)                                              \
        {                                                                                           \
            _object = CObjectImplement<_i_type>::create_instance();                                 \
            _object->add_ref();                                                                     \
            _object->_phost = const_cast<_host*>(phost);                                            \
            _object->convert(_type_id(_sink), (void**)&_callback);                                  \
            _object->del_ref();                                                                     \
            if (!_callback) _object = nullptr_t;                                                    \
        }                                                                                           \
        virtual ~sink_name##Ptr()                                                                   \
        {                                                                                           \
            if (_object != nullptr_t)                                                               \
            {                                                                                       \
                _object->_phost = nullptr_t;                                                        \
            }                                                                                       \
        }                                                                                           \
    public:                                                                                         \
        bool put_host(const _host* phost)                                                           \
        {                                                                                           \
            if (_object != nullptr_t)                                                               \
            {                                                                                       \
                _object->_phost = const_cast<_host*>(phost);                                        \
            }                                                                                       \
            return true;                                                                            \
        }                                                                                           \
    public:                                                                                         \
        CObjectImplement<_i_type>* operator->() const                                               \
        {                                                                                           \
            return _object;                                                                         \
        }                                                                                           \
        operator _sink*() const                                                                     \
        {                                                                                           \
            return _callback;                                                                       \
        }                                                                                           \
    private:                                                                                        \
        __sp<_sink> _callback;                                                                      \
        CObjectImplement<_i_type>* _object;                                                         \
    };

#endif//_include_def_callback_h