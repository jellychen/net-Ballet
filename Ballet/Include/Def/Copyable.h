#ifndef _include_def_copyable_h
#define _include_def_copyable_h

class Copyable_{};
typedef Copyable_ Copyable;

class NoCopyable_
{
protected:
    NoCopyable_(){}
    ~NoCopyable_(){}

private:
    NoCopyable_(const NoCopyable_&){}
    const NoCopyable_& operator=(const NoCopyable_&){return *this;}
};
typedef NoCopyable_ NoCopyable;

#endif//_include_def_copyable_h