#ifndef common_threadtaskqueue_h
#define common_threadtaskqueue_h
#include "Include/Def/Inct.h"
#include <queue>

namespace common
{
    class ThreadTaskQueue : public TaskQueueEntry
    {
        typedef std::queue<Task* > queue_task_t;
    public:
        ThreadTaskQueue();
        virtual ~ThreadTaskQueue();

    public:
        ret do_task     (bool);
        ret commit      (const Task*);
        ret should_exit (bool*);

    public:
        bool            exit_;
        queue_task_t    task_queue_a_;
        queue_task_t    task_queue_b_;
        queue_task_t*   task_input_;
        queue_task_t*   task_output_;
        pthread_mutex_t task_locker_;
        pthread_cond_t  task_ready_;
    };
}
#endif//common_threadtaskqueue_h