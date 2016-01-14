#ifndef _include_def_object_creator_h
#define _include_def_object_creator_h
#include "Def.h"
#include "Atomic.h"
#include "Guid.h"
#include "Runtime.h"
#include "Object.h"

_interface_inherit_def(IWeak, IObject)
{
    def_function(obtain,    (const _guid&, void**));
    def_function(available, (bool*));
};
_interface_inherit_bind_without_space(IWeak, IObject, 0000000000000000, 0000000000000001);

class CWeakImplement: public IWeak
{
public:
    CWeakImplement():_object(NULL){}
    virtual ~CWeakImplement()
    {
    }

public:
    ret add_ref()
    {
        _ref_count.increment_and_get();
        return ret_success;
    }

    ret del_ref()
    {
        int32_t _ref = _ref_count.decrement_and_get();
        if (0 == _ref) { delete this;}
        return ret_success;
    }

    ret del_ref_no_free()
    {
        _ref_count.decrement_and_get();
        return ret_success;
    }

    ret hascode(long* pcode)
    {
        if_return(nullptr_t == pcode, ret_args_error);
        *pcode = (long)(void*)this;
        return ret_success;
    }

    ret convert(const _guid& _d, void** ppOut)
    {
        if_return(nullptr_t == ppOut, ret_args_error);
        *ppOut = NULL;
        typedef bool (*_t_convert_func)(IWeak*, const _guid&, void**);
        _t_convert_func _func = IWeak_runtime::__runtime_convert;
        return __bool_ret(_func(static_cast<IWeak*>(this), _d, ppOut));
    }

    ret weak_convert(const _guid& _d, void** ppOut)
    {
        if_return(nullptr_t == ppOut, ret_args_error);
        *ppOut = NULL;
        return ret_success;
    }

    ret obtain(const _guid& _d, void** ppOut)
    {
        if_return(nullptr_t == ppOut, ret_args_error);
        *ppOut = NULL;
        if_return(NULL == this->_object, ret_fail);
        return this->_object->convert(_d, ppOut);
    }

    ret available(bool* _p)
    {
        if_return(nullptr_t == _p, ret_args_error);
        *_p = (NULL != _object);
        return ret_success;
    }
public:
    static bool create_instance(const _guid& _d, void** ppOut)
    {
        if_return((void**)NULL == ppOut, false);
        *ppOut = NULL;

        CWeakImplement* _p = new(std::nothrow) CWeakImplement();
        if_return(NULL == _p, false);

        _p->add_ref();
        if (__success(_p->convert(_d, ppOut)))
        {
            _p->del_ref_no_free();
        }
        else
        {
            _p->del_ref();
        }
        return true;
    }

    static CWeakImplement* create_instance()
    {
        return new(std::nothrow) CWeakImplement();
    }

    bool host_ptr(IObject* _object)
    {
        this->_object = _object;
        return true;
    }
protected:
    AtomicInt32     _ref_count;
    IObject*        _object;
};

template <typename t> class CObjectImplement : public t
{
public:
    CObjectImplement():_ref_count(0),_weak_host(NULL){}
    virtual ~CObjectImplement(){}
    
public:
    ret add_ref()
    {
        this->internal_add_ref(_ref_count); 
        return ret_success;
    }

    ret del_ref()
    {
        this->internal_del_ref(_ref_count);
        if (0 == _ref_count)
        {
            this->internal_uninit();
            delete this;
        }
        return ret_success;
    }

    ret del_ref_no_free()
    {
        this->internal_del_ref(_ref_count);
        return ret_success;
    }

    ret hascode(long* pcode)
    {
        if_return(nullptr_t == pcode, ret_args_error);
        *pcode = (long)(void*)this;
        return ret_success;
    }

    ret convert(const _guid& _d, void** ppOut)
    {
        if_return(nullptr_t == ppOut, ret_args_error);
        *ppOut = NULL;
        return t::convert(_d, ppOut);
    }

    ret weak_convert(const _guid& _d, void** ppOut)
    {
        if_return(nullptr_t == ppOut, ret_args_error);
        *ppOut = NULL;
        if_return(!_weak_host, ret_fail);
        return _weak_host->convert(_d, ppOut);
    }

protected:
    virtual bool internal_init()
    {
        if_return(!t::internal_init(), false);
        this->_weak_host = CWeakImplement::create_instance();
        if_return(!this->_weak_host, false);
        this->_weak_host->add_ref();
        IObject* _object_entry = NULL;
        this->convert(_type_id(IObject), (void**)&_object_entry);
        if_return(!_object_entry, false);
        _object_entry->del_ref();
        this->_weak_host->host_ptr(_object_entry);

        return true;
    }

    virtual bool internal_uninit()
    {
        if_return(!this->_weak_host, false);
        this->_weak_host->host_ptr(NULL);
        this->_weak_host->del_ref();
        return t::internal_uninit();
    }

public:
    static CObjectImplement<t>* create_instance()
    {
        CObjectImplement<t>* _p = new(std::nothrow) CObjectImplement<t>();
        if_return(NULL == _p, NULL);

        _p->add_ref();
        if (_p->internal_init())
        {
            _p->del_ref_no_free();
        }
        else
        {
            _p->del_ref();
            _p = NULL;
        }
        return _p;
    }

    static bool create_instance(const _guid& _d, void** ppOut)
    {
        if_return((void**)NULL == ppOut, false);
        *ppOut = NULL;

        CObjectImplement<t>* _p = new(std::nothrow) CObjectImplement<t>();
        if_return(NULL == _p, false);

        _p->add_ref();
        if (_p->internal_init() && __success(_p->convert(_d, ppOut)))
        {
            _p->del_ref_no_free();
        }
        else
        {
            _p->del_ref();
        }
        return true;
    }

protected:
    long            _ref_count;
    CWeakImplement* _weak_host;
};

template <typename _i, typename t> class CObjectCreator
{
public:
    static bool create(_i** ppOut)
    {
        if_return(NULL == ppOut, false);
        *ppOut = NULL;

        CObjectImplement<t>* _p = new(std::nothrow) CObjectImplement<t>();
        if_return(NULL == _p, false);

        _p->add_ref();
        _p->internal_init();

        if (__success(_p->convert(_type_id(_i), ppOut)))
        {
            _p->del_ref_no_free();
        }
        else
        {
            _p->del_ref();
        }
        return true;
    }
};

#endif//_include_def_object_creator_h