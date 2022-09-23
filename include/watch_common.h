#ifndef __WATCH_IMP_H__
#define __WATCH_IMP_H__

typedef struct watchdog
{
	pid_t watch_proc;
	pthread_t watch_thread;
	int counter;
	size_t interval;
	char *our_argv[100]; /* overkill size */

} watchdog_ty;  /* must be global, as follows: */

/* global var */
watchdog_ty watchdog_params;

/* if counter gets to this val - from 0 - we ressurect.
   counter increased in user defined time intervals */
#define OUT_OF_TIME 3


/* call that before calling Watch! */
void SetSignalHandler(void);


/*  assumes signal handler is already set , using the above func! 
	assumes global var is initialised: responsbility of the caller!
   	watch_proc - pid to ping.
   	argv - with the target app filename and args 	*/
void *Watch(void *argv);




#endif

