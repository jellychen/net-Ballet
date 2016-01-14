#include "TimeHeapPool.h"

namespace network
{
    TimeHeapPool::TimeHeapPool()
    {
        this->_cache_info = 20000;
        timer_node _node = {nullptr_t, 0, 0};
        this->_heap_array.push_back(_node);
    }

    TimeHeapPool::~TimeHeapPool()
    {
        this->_index_pool.clear();

        while (this->_info_pool.size() > 0)
        {
            delete(this->_info_pool.front());
            this->_info_pool.pop_front();
        }

        for (size_t i = 1; i < this->_heap_array.size(); ++i)
        {
            delete this->_heap_array[i]._info;
        }
        this->_heap_array.clear();

        _t_vec_timer_pool _empty;
        this->_heap_array.swap(_empty);
    }

    bool TimeHeapPool::set_timer_out(int _id, ITimerCallback* _callback, int _time)
    {
        return this->set_timer_node(_id, _callback, _time, false);
    }

    bool TimeHeapPool::set_timer_loop(int _id, ITimerCallback* _callback, int _time)
    {
        return this->set_timer_node(_id, _callback, _time, true);
    }

    bool TimeHeapPool::del_timer(int _id, ITimerCallback* _callback)
    {
        if_return(nullptr_t == _callback, false);

        long _callback_hash = 0;
        _callback->hascode(&_callback_hash);
        timer_index _time_index = {_id, _callback_hash};
        _t_map_timer_pool::iterator _it = this->_index_pool.find(_time_index);
        if_return(this->_index_pool.end() == _it, false);
        int _index = _it->second->_index;
        this->_index_pool.erase(_it);

        if (this->_info_pool.size() >= (size_t)(this->_cache_info))
        {
            delete(this->_heap_array[_index]._info);
        }
        else
        {
            this->_info_pool.push_back(this->_heap_array[_index]._info);
        }

        this->_heap_array[_index]._info = nullptr_t;
        return this->del_timer_node(_index);
    }

    bool TimeHeapPool::lastest_timeout(int64_t& _time)
    {
        if (1 >= this->_heap_array.size()) {_time = INT_MAX; return true;}
        _time = this->_heap_array[1]._timeout - this->get_current_time();
        if (0 >= _time) _time = 0;
        return true;
    }

    bool TimeHeapPool::tick(IEventPoll* _poll)
    {
        if_return(nullptr_t == _poll, false);
        int64_t _current = this->get_current_time();
        while (this->_heap_array.size() > 1
                && this->_heap_array[1]._timeout <= _current 
                    && this->_heap_array[1]._tick_time != _current)
        {
            timer_node _node = this->_heap_array[1];
            this->del_timer_node(1);

            if (false == _node._info->_loop)
            {
                long _callback_hash = 0;
                _node._info->_callback->hascode(&_callback_hash);
                timer_index _time_index = {_node._info->_id, _callback_hash};

                _t_map_timer_pool::iterator _it = this->_index_pool.find(_time_index);
                if (this->_index_pool.end() != _it)
                {
                    this->_index_pool.erase(_it);
                }

                _node._info->_callback->on_time(_node._info->_id, _poll);
                if (this->_info_pool.size() >= (size_t)(this->_cache_info))
                {
                    delete _node._info;
                }
                else
                {
                    this->_info_pool.push_back(_node._info);
                }
                _node._info = nullptr_t;
            }
            else
            {
                _node._tick_time = _current;
                _node._timeout = _node._timeout + _node._info->_timespec;
                this->add_timer_node(_node);
                _node._info->_callback->on_time(_node._info->_id, _poll);
            }
        }
        return true;
    }

    bool TimeHeapPool::add_timer_node(timer_node& _node)
    {
        _node._info->_index = this->_heap_array.size();
        this->_heap_array.push_back(_node);

        int _index = _node._info->_index;
        while (true)
        {
            int _parent_index = _index/2;
            if_break(0 == _parent_index);
            if_break(this->_heap_array[_parent_index]._timeout <= this->_heap_array[_index]._timeout);

            timer_node _node = this->_heap_array[_parent_index];
            this->_heap_array[_parent_index] = this->_heap_array[_index];
            this->_heap_array[_parent_index]._info->_index = _parent_index;
            this->_heap_array[_index] = _node;
            this->_heap_array[_index]._info->_index = _index;
        }
        return true;
    }

    bool TimeHeapPool::del_timer_node(int _index)
    {
        if_return(_index < 1 || _index >= (int)this->_heap_array.size(), false);

        int _last_index = (int)this->_heap_array.size() -1;
        if (_index == _last_index)
        {
            this->_heap_array.resize(_last_index);
        }
        else
        {
            this->_heap_array[_index] = this->_heap_array[_last_index];
            this->_heap_array[_index]._info->_index = _index;
            this->_heap_array.resize(_last_index);
            _last_index--;

            size_t _current_index = _index;
            while (true)
            {
                if_break(_current_index >= _last_index);
                
                size_t _min = _current_index;
                size_t _left = _current_index*2;
                if (_left <= _last_index && this->_heap_array[_min]._timeout > this->_heap_array[_left]._timeout)
                {
                    _min = _left;
                }

                size_t _right = _current_index*2+1;
                if (_right <= _last_index && this->_heap_array[_min]._timeout > this->_heap_array[_right]._timeout)
                {
                    _min = _right;
                }

                if_break(_min == _current_index);

                timer_node _node = this->_heap_array[_current_index];
                this->_heap_array[_current_index] = this->_heap_array[_min];
                this->_heap_array[_current_index]._info->_index = _current_index;
                this->_heap_array[_min] = _node;
                this->_heap_array[_min]._info->_index = _min;

                _current_index = _min;
            }
        }
        return true;
    }

    bool TimeHeapPool::set_timer_node(int _id, ITimerCallback* _callback, int _time, bool _loop)
    {
        if_return(nullptr_t == _callback, false);

        long _callback_hash = 0;
        _callback->hascode(&_callback_hash);
        timer_index _timer_index = {_id, _callback_hash};
        _t_map_timer_pool::iterator _it = this->_index_pool.find(_timer_index);
        if_return(this->_index_pool.end() != _it, false);

        timer_info* _info = nullptr_t;
        if (this->_info_pool.size() <= 0) 
        {
            _info = new(std::nothrow) timer_info();
        }
        else
        {
            _info = this->_info_pool.front();
            this->_info_pool.pop_front();
        }
        if_return(_info == nullptr_t, false);

        _info->_id = _id;
        _info->_index = -1;
        _info->_loop = _loop;
        _info->_timespec = _time;
        _info->_callback = _callback;

        this->_index_pool[_timer_index] = _info;
        timer_node _node = {_info, this->get_current_time() +_time, 0};
        return this->add_timer_node(_node);
    }
}