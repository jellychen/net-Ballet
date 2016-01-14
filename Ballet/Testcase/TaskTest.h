#include "Include/Def/Inct.h"
#include "Include/Common/Inct.h"
#include "Include/Network/IEventPoll.h"

class TaskTest
{
public:
	void task()
	{
		printf("%s\n", "task");
	}

	bool test()
	{
		Task* _task = TaskBind<TaskTest>::create(this, &TaskTest::task);
		_task->run();
		return true;
	}
};