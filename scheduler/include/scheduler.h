#ifndef __sched_ty_H__
#define __sched_ty_H__


typedef struct scheduler sched_ty;

#include "uid.h"
#include <time.h>

sched_ty *SchedCreate(void);

void SchedDestroy(sched_ty *sched_ty);

/* returns if should be re-scheduled */
typedef int (*SchedFunc)(void *package);
typedef uid_ty sched_id_ty;

sched_id_ty SchedAdd(sched_ty *sched_ty, SchedFunc, void *params, time_t interval);

int SchedRemove(sched_ty *sched_ty, uid_ty uid_to_remove);

typedef enum run_status
{
    EMPTY = 0, 
    PAUSE = 1
}run_status_ty;

run_status_ty SchedRun(sched_ty *sched_ty);


void SchedPause(sched_ty *sched_ty);


size_t SchedSize(sched_ty *sched_ty);

int SchedIsEmpty(sched_ty *sched_ty);


void SchedClear(sched_ty *sched_ty);



#endif /* __sched_ty_H__ */
