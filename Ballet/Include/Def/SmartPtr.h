#ifndef _include_def_smartptr_h
#define _include_def_smartptr_h
#include "Def.h"
#include "Guid.h"
#include "Runtime.h"
#include "Object.h"
#include "ObjectCreator.h"

template<typename t> class __sp_base
{
protected:
    __sp_base() throw()
    {
        this->_ptr = nullptr_t;
    }

    __sp_base(int _null) throw()
    {
        (void) _null;
        this->_ptr = nullptr_t;
    }

    __sp_base(const t* ptr) throw()
    {
        this->_ptr = const_cast<t*>(ptr);
        if (this->_ptr != nullptr_t)
        {
            this->_ptr->add_ref();
        }
    }
public:
    virtual ~__sp_base() throw()
    {
        if (this->_ptr != nullptr_t)
        {
            this->_ptr->del_ref();
            this->_ptr = nullptr_t;
        }
    }

    operator t*() const throw()
    {
        return this->_ptr;
    }

    t& operator*() const
    {
        assert(_ptr != nullptr_t);
        return *(this->_ptr);
    }

    t** operator&() throw()
    {
        assert(_ptr == nullptr_t);
        return &_ptr;
    }

    t* operator->() const throw()
    {
        assert(_ptr != nullptr_t);
        return _ptr;
    }

    bool operator!() const throw()
    {
        return this->_ptr == nullptr_t;
    }

    bool operator== (const t* ptr) const throw()
    {
        return static_cast<t*>(ptr) == this->_ptr;
    }

    bool operator!= (const t* ptr) const throw()
    {
        return static_cast<t*>(ptr) != this->_ptr;
    }

    bool release()
    {
        if (_ptr != nullptr_t)
        {
            _ptr->del_ref();
            _ptr = nullptr_t;
        }
        return true;
    }
public:
    t* _ptr;
};

template <typename t> class __sp :public __sp_base<t>
{
public:
    __sp()                      throw(){}
    __sp(int _null)             throw():__sp_base<t>(_null){}
    __sp(const t* _p)           throw():__sp_base<t>(_p){}
    __sp(const __sp<t>& _p)     throw():__sp_base<t>(_p._ptr){}

public:
    operator bool() const
    {
        return this->_ptr != nullptr_t;
    }

    bool operator= (const t* _ptr) throw()
    {
        if_return(const_cast<t*>(_ptr) == this->_ptr, true);
        this->release();
        if_return(_ptr == nullptr_t, true);
        this->_ptr = const_cast<t*>(_ptr);
        this->_ptr->add_ref();
        return true;
    }

    bool operator= (int _null) throw()
    {
        this->release();
        return true;
    }

    bool operator= (const __sp<t>& _p) throw()
    {
        return operator=(_p._ptr);
    }
};

template<typename t> class __wp_base
{
protected:
    __wp_base() throw()
    {
        this->_ptr = nullptr_t;
    }

    __wp_base(int _null) throw()
    {
        (void) _null;
        this->_ptr = nullptr_t;
    }

    __wp_base(const t* ptr) throw()
    {
        this->_ptr = nullptr_t;
        if_return_void(nullptr_t == ptr);
        ptr->weak_convert(_type_id(IWeak), (void**)&_host);
        if_return_void(!_host);
        this->_ptr = (t*)(ptr);
    }

public:
    operator t*() const throw()
    {
        return this->_ptr;
    }

    t& operator*() const
    {
        assert(_ptr != nullptr_t);
        return *(this->_ptr);
    }

    t** operator&() throw()
    {
        assert(_ptr == nullptr_t);
        return &_ptr;
    }

    t* operator->() const throw()
    {
        assert(_ptr != nullptr_t);
        return _ptr;
    }

    bool operator!() const throw()
    {
        return this->_ptr == nullptr_t;
    }

    bool operator==(const t* ptr) const throw()
    {
        return static_cast<t*>(ptr) == this->_ptr;
    }

    bool operator!=(const t* ptr) const throw()
    {
        return static_cast<t*>(ptr) != this->_ptr;
    }
    
public:
    t*              _ptr;
    __sp<IWeak>     _host;
};

template <typename t> class __wp :public __wp_base<t>
{
public:
    __wp()                  throw(){}
    __wp(int _null)         throw():__wp_base<t>(_null){}
    __wp(const t* _p)       throw():__wp_base<t>(_p){}
    
    __wp(const __wp<t>& _p) throw()
    {
        this->_ptr = _p._ptr;
        this->_host = _p._host;
    }

    __wp(const __sp<t>& _p) throw()
    {
        this->_ptr = nullptr_t;
        this->_host = nullptr_t;
        if_return_void(!_p);
        _p->weak_convert(_type_id(IWeak), (void**)&(this->_host));
        if_return_void(!this->_host);
        this->_ptr = _p._ptr;
    }

public:
    bool operator= (int _null) throw()
    {
        this->_ptr = nullptr_t;
        this->_host = nullptr_t;
        return true;
    }

    operator bool() const
    {
        if_return(nullptr_t == this->_ptr || !this->_host, false);
        bool _available = false;
        this->_host->available(&_available);
        return _available;
    }

    bool promote(const _guid& _d, void** ppOut)
    {
        if_return(nullptr_t == ppOut, false);
        *ppOut = nullptr_t;
        if_return(nullptr_t == this->_ptr, true);
        return __bool_ret(this->_ptr->convert(_d, ppOut));
    }

    bool operator= (const t* ptr) throw()
    {
        this->_ptr = nullptr_t;
        this->_host = nullptr_t;
        if_return(nullptr_t == ptr, true);
        ((t*)(ptr))->weak_convert(_type_id(IWeak), (void**)&(this->_host));
        if_return(!this->_host, false);
        this->_ptr = (t*)(ptr);
        return true;
    }

    bool operator= (const __wp<t>& _p) throw()
    {
        this->_ptr = nullptr_t;
        this->_host = _p._host;
        if_return(!this->_host, false);
        this->_host->obtain(_type_id(t), (void**)&(this->_ptr));
        if (!this->_ptr) {this->_host = nullptr_t; return false;}
        this->_ptr->del_ref();
        return true;
    }

    bool operator= (const __sp<t>& _p) throw()
    {
        this->_ptr = nullptr_t;
        this->_host = nullptr_t;
        _p->weak_convert(_type_id(IWeak), (void**)&(this->_host));
        if_return(!this->_host, false);
        this->_host->obtain(_type_id(t), (void**)&(this->_ptr));
        if (!this->_ptr) {this->_host = nullptr_t; return false;}
        this->_ptr->del_ref();
        return true;
    }
};
#endif//_include_def_smartptr_h