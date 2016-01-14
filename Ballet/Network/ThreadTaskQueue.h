#ifndef network_threadtaskqueue_h
#define network_threadtaskqueue_h
#include "Include/Def/Inct.h"
#include <queue>

namespace network
{
    class ThreadTaskQueue : public TaskQueueEntry
    {
        typedef std::queue<Task* > queue_task_t;
    public:
        ThreadTaskQueue();
        virtual ~ThreadTaskQueue();

    public:
        ret do_task ();
        ret commit  (const Task*);

    public:
        queue_task_t    task_queue_a_;
        queue_task_t    task_queue_b_;
        queue_task_t*   task_input_;
        queue_task_t*   task_output_;
        pthread_mutex_t task_locker_;
    };
}
#endif//network_threadtaskqueue_h