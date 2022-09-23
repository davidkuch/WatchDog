#ifndef __WATCHDOG_H__
#define __WATCHDOG_H__
#include <stddef.h>

 
 /* pass argc and argv from main's arguments */
int KeepMeAlive(int argc, const char *argv[], size_t interval);

/* make sure to call that before exiting on purpose */
int DNR(void);


#endif
