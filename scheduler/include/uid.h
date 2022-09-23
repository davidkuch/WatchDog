#ifndef __UID_H__
#define __UID_H__

#include <sys/types.h>	/* pid_t, time_t */
#include <unistd.h>	    /* getpid */
#include <time.h>	    /* time() */

typedef struct uid uid_ty;

struct uid
{
    size_t counter;
    time_t time;
    pid_t pid;
};

/*  O(1)  */
uid_ty UIDGenerate(void);


/* O(1)  */
int UIDIsSame(uid_ty uid1, uid_ty uid2);

extern const uid_ty BAD_UID;

#endif
