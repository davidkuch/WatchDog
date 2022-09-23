#include "task.h"
#include <stdlib.h>

struct task 
{
    TaskFunc func;
    void *params;
    size_t interval;
    size_t next_run;
    uid_ty id;
};


task_ty *CreateTask(time_t clock, TaskFunc func, void *params, time_t interval)
{
	/* assert */
	
	/* malloc for task_ty */
	task_ty *task = (task_ty *)malloc(sizeof(task_ty));
	uid_ty id;
	
	/* check null */
	if (task == NULL)
	{
		return NULL;
	}

	id = UIDGenerate();

	/* init fields from args */
	task->func =func;
	task->params = params;
	task->interval = interval;
	ScheduleTask(clock, task);
	task->id = id;
	
	/* return pointer */
	return task;
}


void ScheduleTask(time_t clock, task_ty *current_task)
{
	/* update next run: current + interval */
	current_task->next_run = clock + GetIntervalTask(current_task);
}


uid_ty GetIdTask(task_ty *task)
{
	return task->id;
}


TaskFunc GetFuncTask(task_ty *task)
{
	return task->func;
}


void *GetParamTask(task_ty *task)
{
	return task->params;
}


size_t GetIntervalTask(task_ty *task)
{
	return task->interval;
}


size_t GetNextRunTask(task_ty *task)
{
	return task->next_run;
}
