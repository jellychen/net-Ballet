#ifndef _include_def_atomic_h
#define _include_def_atomic_h
#include <stdint.h>

template <typename t> class TAtomicInt
{
public:
    TAtomicInt():_t(0){}

public:
    t get()
    {
        return __sync_val_compare_and_swap(&_t, 0, 0);
    }

    t get_and_add(t _x)
    {
        return __sync_fetch_and_add(&_t, _x);
    }

    t add_get_get(t _x)
    {
        return get_and_add(_x) + _x;
    }

    t get_and_set(t _x)
    {
        return __sync_lock_test_and_set(&_t, _x);
    }

    t increment_and_get()
    {
        return add_get_get(1);
    }

    t decrement_and_get()
    {
        return add_get_get(-1);
    }
private:
    volatile t _t;
};

typedef TAtomicInt<int32_t> AtomicInt32;
typedef TAtomicInt<int64_t> AtomicInt64;

#endif//_include_def_atomic_h