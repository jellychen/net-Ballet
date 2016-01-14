#include "ServiceManage.h"

namespace service
{
    ServiceManage* __g_center = nullptr_t;

    void __sig_handler(int _sig)
    {
        if_return_void(nullptr_t == __g_center);
        if (SIGCHLD == _sig) (void)(0);
        if (SIGINT == _sig) __g_center->kill_all();
        if (SIGQUIT == _sig) __g_center->kill_all();
    }

    void __install_sig()
    {
        signal(SIGINT,  SIG_IGN);       // 终端中断  
        signal(SIGHUP,  SIG_IGN);       // 连接挂断  
        signal(SIGQUIT, SIG_IGN);       // 终端退出  
        signal(SIGPIPE, SIG_IGN);       // 向无读进程的管道写数据  
        signal(SIGTTOU, SIG_IGN);       // 后台程序尝试写操作  
        signal(SIGTTIN, SIG_IGN);       // 后台程序尝试读操作  
        signal(SIGTERM, SIG_IGN);       // 终止
        signal(SIGCHLD, __sig_handler);
        signal(SIGQUIT, __sig_handler);
        signal(SIGINT,  __sig_handler);
    }

    ServiceManage::ServiceManage()
    {
        this->exit_ = false;
        __g_center = (ServiceManage*) this;
    }

    ServiceManage::~ServiceManage()
    {
        __g_center = nullptr_t;
    }

    ret ServiceManage::start(bool _deamon)
    {
        if (true == _deamon)
        {
            if_return(::fork() > 0, ret_success);
            ::setsid();
        }

        __install_sig();

        service_map_t::iterator _it = this->service_pool_.begin();
        while (this->service_pool_.end() != _it)
        {
            if_continue(!_it->second);

            bool _available = false;
            _it->second->is_available(&_available);
            if_continue(false == _available);
            _it->second->on_start();

            int _count = 0;
            _it->second->get_progress_count(&_count);
            for (int i = 0; i < _count; ++i)
            {
                this->fork_service(_it->second);
            }

            _it++;
        }

        while (true)
        {
            int _status = 0;
            pid_t _id = ::waitpid(-1, &_status, WUNTRACED);
            if (false == this->exit_)
            {
                this->find_exit(_id, true);
            }
            else
            {
                this->find_exit(_id, false);
                if_break(0 == this->progress_service_pool_.size());
            }
        }
        return ret_success;
    }

    ret ServiceManage::attach_service(IService* _service)
    {
        if_return(nullptr_t == _service, ret_args_error);
        long _hash; _service->hascode(&_hash);
        this->service_pool_[_hash] = _service;
        return ret_success;
    }

    ret ServiceManage::detach_service(IService* _service)
    {
        if_return(nullptr_t == _service, ret_args_error);
        long _hash; _service->hascode(&_hash);
        service_map_t::iterator _it = this->service_pool_.find(_hash);
        if_return(this->service_pool_.end() == _it, ret_fail);
        this->service_pool_.erase(_it);
        return ret_success;
    }

    ret ServiceManage::fork_service(IService* _service)
    {
        if_return(nullptr_t == _service, ret_args_error);
        pid_t _id = ::fork();
        if_return(0 > _id, ret_fail);
        
        if (_id > 0)
        {
            this->progress_service_pool_[(int)_id] = _service;
        }
        else if (0 == _id)
        {
            signal(SIGINT,  SIG_IGN);
            signal(SIGHUP,  SIG_IGN);
            signal(SIGQUIT, SIG_IGN);
            signal(SIGPIPE, SIG_IGN);
            signal(SIGTTOU, SIG_IGN);
            signal(SIGTTIN, SIG_IGN);
            signal(SIGTERM, SIG_IGN);

            _service->on_progress_init();
            _service->on_progress_run();
            _service->on_progress_release();

            ::exit(0);
        }
        return ret_success;
    }

    ret ServiceManage::kill_all()
    {
        this->exit_ = true;
        service_map_t::iterator _it = this->progress_service_pool_.begin();
        for (; _it != this->progress_service_pool_.end(); ++_it)
        {
            if (_it->first > 0)
            {
                ::kill(_it->first, SIGRTMIN + 1);
            }

            if (_it->second)
            {
                _it->second->on_release();
            }
        }
        return ret_success;
    }

    ret ServiceManage::find_exit(pid_t _pid, bool _fork)
    {
        service_map_t::iterator _it = this->progress_service_pool_.find((int)_pid);
        if (this->progress_service_pool_.end() != _it)
        {
            __sp<IService> _service = _it->second;
            this->progress_service_pool_.erase(_it);
            
            if (true == _fork)
            {
                this->fork_service(_service);
            }
        }
        return ret_success;
    }

    ret ServiceManage::reload_all()
    {
        return ret_success;
    }
}