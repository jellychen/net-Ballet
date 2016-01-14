#ifndef _include_def_task_h
#define _include_def_task_h
#include "Def.h"
#include "Guid.h"
#include "Runtime.h"
#include "Object.h"
#include "ObjectCreator.h"
#include "SmartPtr.h"
#include <stack>

class Task
{
public:
	virtual ~Task() {}
	virtual ret run() =0;
	virtual ret force_delete()
	{
		delete this;
		return ret_success;
	}
};

class TaskQueueEntry
{
public:
    virtual ret commit(const Task*) =0;
};

template<typename t> class TaskBind : public Task
{
	typedef TaskBind<t>* __p_task;
	typedef void (t::*__pfunc) ();

public:
	ret run()
	{
		(context_->*func_)();
		delete this;
		return ret_success;
	}

	static Task* create(const t* _context, __pfunc _func)
	{
		if_return(!_context || !_func, nullptr_t);
		__p_task _task = new(std::nothrow) TaskBind<t >();
		if_return(nullptr_t == _task, _task);
		_task->context_ = (t*)_context; _task->func_ = _func;
		return _task;
	}
public:
	t* context_; __pfunc func_;
};

template<typename _t> class TaskExecut : public Task, public _t
{
public:
	virtual ~TaskExecut(){}

public:
	ret run()
	{
		_t::do_task();
		_t::do_task_complete();
		delete this;
		return ret_success;
	}

	static Task* create()
	{
		return new(std::nothrow) TaskExecut<_t >();
	}
};

template<typename _t> class TaskExecutEcho : public Task, public _t
{
public:
	TaskExecutEcho(TaskQueueEntry* _entry)
	{
		this->_thread_entry = _entry;
		this->_thread_id = ::pthread_self();
	}
	virtual ~TaskExecutEcho(){}

public:
	ret run()
	{
		if (::pthread_self() != this->_thread_id)
		{
			_t::do_task();
			if_return(nullptr_t == _thread_entry, ret_fail);
			return this->_thread_entry->commit(this);
		}
		else
		{
			_t::do_task_complete();
			delete this;
		}
		return ret_success;
	}

	static Task* create(TaskQueueEntry* _entry, _t** _out)
	{
		if_return(nullptr_t == _entry, nullptr_t);
		TaskExecutEcho<_t >* _task = new(std::nothrow) TaskExecutEcho<_t >(_entry);
		if (nullptr_t != _out) *_out = _task;
		if_return(nullptr_t == _task, nullptr_t);
		return _task;
	}

private:
	unsigned long int	_thread_id;
	TaskQueueEntry* 	_thread_entry;
};

template<typename _t> class TaskExecutRoute : public Task, public _t
{

};
#endif//_include_def_task_h