#ifndef _include_def_reference_h
#define _include_def_reference_h
#include "Def.h"
#include "Guid.h"
#include "Runtime.h"
#include "Object.h"
#include <unistd.h>
#include <pthread.h>
#include <assert.h>

template<typename t> class ObjectInternal
{
public:
    virtual void internal_add_ref   (long& l) {_t.add(l);}
    virtual void internal_del_ref   (long& l) {_t.del(l);}
private:
    t _t;
};

template<typename t, typename c> class ObjectInternalInherit :public c
{
public:
    virtual void internal_add_ref   (long& l) {_t.add(l);}
    virtual void internal_del_ref   (long& l) {_t.del(l);}
private:
    t _t;
};

class ref_thread_not_safe
{
public:
    void add(long& l) {l++;}
    void del(long& l) {l--;}
};
typedef ref_thread_not_safe __rtns;

class ref_thread_safe
{
public:
    ref_thread_safe()
    {
        pthread_mutex_init(&_lock, NULL);
    }
    virtual ~ref_thread_safe()
    {
        pthread_mutex_destroy(&_lock);
    }
    void add(long& l) 
    {
        pthread_mutex_lock(&_lock);
        assert(l >= 0);
        l++;
        pthread_mutex_unlock(&_lock);
    }
    void del(long& l)
    {
        pthread_mutex_lock(&_lock);
        assert(l > 0);
        l--;
        pthread_mutex_unlock(&_lock);
    }
private:
    pthread_mutex_t _lock;
};
typedef ref_thread_safe __rts;

class ref_thread_safe_lock_free
{
public:
    void add(long& l) {__sync_fetch_and_add(&l, 1);}
    void del(long& l) {__sync_fetch_and_add(&l, -1);}
};
typedef ref_thread_safe_lock_free __rtslf;

class ref_none
{
public:
    void add(long& l) {}
    void del(long& l) {}
};
typedef ref_none __rn;

#endif//_include_def_reference_h