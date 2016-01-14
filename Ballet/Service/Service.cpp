#include "Service.h"

namespace service
{
	Service::Service()
	{
		this->progress_count_ = 1;
		this->service_name_ = "none";
	}

	Service::~Service()
	{

	}

	bool Service::internal_init()
	{
		return true;
	}

	ret Service::init()
	{
		return ret_success;
	}

	ret Service::is_available(bool* _available)
	{
		if_return(nullptr_t == _available, ret_args_error);
		*_available = !!this->callback_;
		return ret_success;
	}

    ret Service::set_name(const char* _name)
    {
    	this->service_name_ = _name;
    	return ret_success;
    }

    ret Service::get_name(std::string* _name)
    {
    	if_return(nullptr_t == _name, ret_args_error);
    	*_name = this->service_name_;
    	return ret_success;
    }

    ret Service::set_progress_count(int _count)
    {
    	this->progress_count_ = _count;
    	return ret_success;
    }

    ret Service::get_progress_count(int* _count)
    {
    	if_return(nullptr_t == _count, ret_args_error);
    	*_count = this->progress_count_;
    	return ret_success;
    }

	ret Service::set_callback(IServiceCallback* _callback)
	{
		this->callback_ = _callback;
		return ret_success;
	}

	ret Service::get_callback(IServiceCallback** _callback)
	{
		if_return(nullptr_t == _callback, ret_args_error);
		*_callback = nullptr_t;
		if_return(nullptr_t == this->callback_, ret_success);
		return this->callback_->convert(_type_id(IServiceCallback), (void**)_callback);
	}

	ret Service::on_start()
	{
		if (this->callback_)
		{
			this->callback_->on_init(__get_i_entity<IService>());
		}
		return ret_success;
	}

	ret Service::on_progress_init()
	{
		if (this->callback_)
		{
			this->callback_->on_start(__get_i_entity<IService>());
		}
		return ret_success;
	}

	ret Service::on_progress_run()
	{
		if (this->callback_)
		{
			this->callback_->on_run(__get_i_entity<IService>());
		}
		return ret_success;
	}

	ret Service::on_progress_release()
	{
		if (this->callback_)
		{
			this->callback_->on_stoped(__get_i_entity<IService>());
		}
		return ret_success;
	}

	ret Service::on_release()
	{
		if (this->callback_)
		{
			this->callback_->on_uninit(__get_i_entity<IService>());
		}
		return ret_success;
	}
}