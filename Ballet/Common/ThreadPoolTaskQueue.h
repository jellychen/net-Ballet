#ifndef common_threadpooltaskqueue_h
#define common_threadpooltaskqueue_h
#include "Include/Def/Inct.h"
#include <queue>

namespace common
{
    class ThreadPoolTaskQueue : public TaskQueueEntry
    {
        typedef std::queue<Task* > queue_task_t;
    public:
        ThreadPoolTaskQueue();
        virtual ~ThreadPoolTaskQueue();

    public:
        ret add_ref     ();
        ret del_ref     ();

    public:
        ret do_task     (int*);
        ret commit      (const Task*);
        ret wakeup_all  ();
        
    public:
        bool            exit_;
        AtomicInt32     ref_;
        queue_task_t    task_queue_;
        pthread_mutex_t task_locker_;
        pthread_cond_t  task_ready_;
    };
}
#endif//common_threadpooltaskqueue_h