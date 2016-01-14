#include "Current.h"

namespace network
{
    int64_t Current::get_current_time()
    {
        struct timeval _curTime; gettimeofday(&_curTime, 0);
        return ((int64_t)_curTime.tv_sec * 1000) + _curTime.tv_usec / 1000;
    }
}