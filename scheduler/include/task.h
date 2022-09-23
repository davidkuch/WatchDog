#ifndef __TASK_K__
#define __TASK_K__

#include <stddef.h>
#include <time.h>
#include "uid.h"

typedef struct task task_ty;
typedef int (*TaskFunc)(void *package);

task_ty *CreateTask(time_t clock, TaskFunc func, void *params, time_t interval);


uid_ty GetIdTask(task_ty *task);


TaskFunc GetFuncTask(task_ty *task);


void *GetParamTask(task_ty *task);


size_t GetIntervalTask(task_ty *task);


size_t GetNextRunTask(task_ty *task);


void ScheduleTask(time_t clock, task_ty *current_task);


#endif
