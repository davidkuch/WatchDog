#include <stdlib.h>
#include <assert.h>

#include "pqueue.h"
#include "scheduler.h"
#include "task.h"

#define SHOULD_RUN 1
#define SHOULD_NOT_RUN 0
#define FOUND 1
#define NOT_FOUND 1


struct scheduler
{
    pqueue_ty *tasks;
    time_t clock;
    task_ty *current_task;
    int should_run;
};


static void InitSched(sched_ty *s, pqueue_ty *tasks);

static void SleepStrict(const time_t sleep_time);

static int PQCompare(const void *old, const void *newcomer, const void *cmp_param);
static int MatchCompare(const void *old, const void *param);

int RunTask(task_ty *task);
int RemoveFromCurrentTask(sched_ty *s,uid_ty uid_to_remove);
int RemoveFromTasks(sched_ty *s,uid_ty uid_to_remove);
void GetNextTask(sched_ty *s);
void ReturnTask(sched_ty *s);
void RemoveTasks(sched_ty *s);


sched_ty *SchedCreate(void)
{
	sched_ty *s  = NULL;
	pqueue_ty *tasks = NULL;
	
	s = (sched_ty *)malloc(sizeof(sched_ty));
	if (s  == NULL)
	{
		return NULL;
	}
	
	/* call create pq, with our cmp func and param */
	tasks = PQueueCreate(PQCompare , NULL);

	if (tasks == NULL)
	{
		free(s);
		return NULL;
	}
	
	/* init fields */
	InitSched(s, tasks); 
			
	return s;
}


void SchedDestroy(sched_ty *s)
{
	/* assert */
	assert(s != NULL);
	
	/* remove and free all tasks in queue */
	RemoveTasks(s);
			
	/* call pq destory */
	PQueueDestroy(s->tasks);

	/* free current task if present */
	if (s->current_task != NULL)
	{
		free(s->current_task);
	}
			
	/* free sched struct */
	free(s);
}


sched_id_ty SchedAdd(sched_ty *s, SchedFunc func, void *params, time_t interval)
{
	
	int enqueue_stt = 0;
	task_ty *task = NULL;
	
	/* assert */
	assert(s != NULL);
	assert(func != NULL);
	
	
	/* Create task struct */
	task = CreateTask(s->clock, func, params, interval);
	/* check null */
	if (task == NULL)
	{
		return BAD_UID;
	}
	
	/* call pq enqueue with pointer to the struct */
	enqueue_stt = PQueueEnqueue(s->tasks, task);
		
	/* check succes */
	if (enqueue_stt != 0)
	{
		return BAD_UID;
	}

	/* return uid */
	return GetIdTask(task);
}

/* remove task from queue / current task variable */
int SchedRemove(sched_ty *s, uid_ty uid_to_remove)
{
	int is_found = NOT_FOUND; /* default, changed if found */
	
	assert(s != NULL);
	
	/* check if task is in var and remove if it is */
	is_found = RemoveFromCurrentTask(s, uid_to_remove);
			
	/* wasn't in current task: check in tasks queue */
	if (!is_found)
	{
		is_found = RemoveFromTasks(s, uid_to_remove);
	}
	
	return is_found;
}


/*enum run_stat = {empty, pause}; */
run_status_ty SchedRun(sched_ty *s)
{
	int task_stt = 0;
	
	/* assert */
	assert(s != NULL);
	
	s->should_run = SHOULD_RUN;
	
	/* while not stoped and pq not empty */
	while((s->should_run && !SchedIsEmpty(s)))
	{
		/*  peek and save in current task */
		s->current_task = PQueuePeek(s->tasks);
				
		/* take out of tasks */
		PQueueDequeue(s->tasks);
		
		/* sleep till next run time */
		SleepStrict(GetNextRunTask(s->current_task) - s->clock);
					
		/* update clock */
		s->clock = GetNextRunTask(s->current_task);/* clock updated only here */
				
		task_stt = RunTask(s->current_task); /*  run task and get return status */		
		
		/*  if status to run again, and task didn't remove itself */
		if((task_stt == 0 && s->current_task != NULL))
		{
				/* reschedule */
				ScheduleTask(s->clock, s->current_task);
						
				/* enqueue */
				PQueueEnqueue(s->tasks, s->current_task); /* check failure? */
			}
		else
		{
			if (s->current_task != NULL)
			{
				free(s->current_task);
			}
		}
		
		/* to avoid redundent information */
		s->current_task = NULL; 	
	}		

	/* return ??? */
	return (!s->should_run);
}


void SchedPause(sched_ty *s)
{

	assert(s != NULL);
	
	/* set should run to stop */
	s->should_run = SHOULD_NOT_RUN;
}


size_t SchedSize(sched_ty *s)
{
	
	assert(s != NULL);
	
 	/* use pq size */
 	return PQueueCount(s->tasks);
}


int SchedIsEmpty(sched_ty *s)
{
 	assert(s != NULL);
 	
 	/* use pq isempty */
 	return PQueueIsEmpty(s->tasks);
}


void SchedClear(sched_ty *s)
{
	/* pq has pointers to task structs. we created them! */
	/* pq frees its structs - that only have pointers to our's */
	
	assert(s != NULL);
	
	/* remove and free all tasks */
	RemoveTasks(s);
			
 	/* reset clock to 0 */
	s->clock = 0;
}


/* --- helper funcs --- */

static void InitSched(sched_ty *s, pqueue_ty *tasks)
{
	/* assign pq to sched->tasks */
	s->tasks = tasks;
					
	s->current_task = NULL;
			
	s->should_run = SHOULD_NOT_RUN;
			
	s->clock = 0;
}


int RemoveFromCurrentTask(sched_ty *s,uid_ty uid_to_remove)
{
	if (s->current_task != NULL && UIDIsSame(GetIdTask(s->current_task), uid_to_remove))
	{
		/* if yes - free and set to null */
		free(s->current_task);
		s->current_task = NULL;
		
		return FOUND;
	}
	
	return NOT_FOUND;
}


int RemoveFromTasks(sched_ty *s,uid_ty uid_to_remove) /*TODO: think, how to replace s with tasks */
{
	int is_found = NOT_FOUND;
	void *erase_stt = NULL;
	
	erase_stt = PQueueErase(s->tasks, MatchCompare, &uid_to_remove);
	 
	if (erase_stt != NULL)
	{
		free(erase_stt);
		is_found = FOUND;
	}
	
	return is_found;
}

	
/*  call task func with task params, save return status */
int RunTask(task_ty *task)
{
	return GetFuncTask(task)(GetParamTask(task));
}
		

void RemoveTasks(sched_ty *s)
{
	task_ty *task_to_remove = NULL;
	
	while (!PQueueIsEmpty(s->tasks))
	{
		task_to_remove = PQueuePeek(s->tasks);
		PQueueDequeue(s->tasks);
		free(task_to_remove);
	}
}


/* sleep sleep_time */
static void SleepStrict(const time_t sleep_time)
{
	time_t sleep_end = time(NULL) + sleep_time; 
	time_t sleep_remainder = sleep_time;
	
	while (time(NULL) < sleep_end)
	{
		sleep_remainder = sleep(sleep_remainder);
	}
}


/* should return positive when old has to be before to newcomer */
int PQCompare(const void *old, const void *newcomer, const void *cmp_param)
{
	/* assert */
	task_ty *old_t = (task_ty *)(old);
	task_ty *newcomer_t = (task_ty *)(newcomer);
	(void)cmp_param;
	return GetNextRunTask(old_t) < GetNextRunTask(newcomer_t);
}


static int MatchCompare(const void *old, const void *id)
{
	/*task_ty *old_t = (task_ty *)(old);
	*/
	return UIDIsSame(GetIdTask((task_ty *)old), *(uid_ty *)id);
}
