#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "watchdog.h"
#include "watch_imp.h"
#include "scheduler.h"

static int timeTicTask(void *unused);
static int restartIfTask(void *sched);
static int pingTask(void *unused);

static void ping_handler(int sig, siginfo_t *info, void *ucontext);
static void TrunctPath(char *pathname);
#define RUN_AGAIN 0
enum {READY, NOT_READY};

static int client_ready = READY;


void *Watch(void *argv) /* takes: argv of app to keep alive */
{	
	sched_ty *sched;

	time_t interval = watchdog_params.interval;
	
	printf("Watch is called, argv[0]: %s \n", ((char **)argv)[0]);
	
	/* create scheduler */
	sched = SchedCreate();
	
	/* add watch Task */
	SchedAdd(sched, restartIfTask, argv, interval);

	/* add ping Task */
	SchedAdd(sched, pingTask, argv, interval);
	
	/* add time-tic task */
	SchedAdd(sched, timeTicTask, argv, interval);
	
	/* run sched */
	SchedRun(sched);
	
	/* wierd if we get here */
	SchedDestroy(sched);
}


/* sets handler for incoming pings */
void SetSignalHandler(void)
{
	struct sigaction sa;

	/* init the sigaction struct */
  	memset(&sa, 0, sizeof(sa));
  	sa.sa_sigaction = ping_handler;

  	/* register a signal handler */
	sigaction(SIGUSR2, &sa, 0);
}


/* check if counter out of time - and restart target app */
static int restartIfTask(void *argv_) /* need to get: argv of app to keep alive */
{
	pid_t pid; /* for fork */
	char **argv = (char **)argv_;
	char pathname[200] = {0};
	
	if (client_ready == READY) 
	{
		printf("restart if: target argv[0]: %s \n", ((char **)argv)[0]);
		
		/* if counter out of time: restart target app */
		if (watchdog_params.counter >= OUT_OF_TIME)
		{	
			puts("try revive user app!\n");
			
			/* make sure its dead*/
			kill(watchdog_params.watch_proc, SIGKILL);
			
			pid = fork();
			
			/* make sure you don't ping it before its ready */
			/* wait for signal before resuming ping iteration */
			if (pid != 0)
			{
				/* update pid to watch */
				watchdog_params.watch_proc = pid;
				
				client_ready = NOT_READY;
			}
			
			/* in child procces */
			if (pid == 0)
			{
				
				strcpy(pathname, argv[0]);
				TrunctPath(argv[0]);
					
				/* set env var to indicate there is a watchdog */
				setenv("WD_PID", "NOT_NEWBORN", 0);

				execv(pathname, argv);
				
				printf("watch restarted target app!\n");
			}
		}	
	}
	
	return RUN_AGAIN;
}


static int pingTask(void *argv)
{
	/* send signal to target pid */
	if (client_ready == READY)
	{
	/*	printf("ping: target argv[0]: %s \n", ((char **)argv)[0]); */
		
		kill(watchdog_params.watch_proc, SIGUSR2);
	}
	
	return RUN_AGAIN;
}


static int timeTicTask(void *argv)
{
	if (client_ready == READY)
	{
/*		printf("time tic on target argv[0]: %s \n", ((char **)argv)[0]); */
		++watchdog_params.counter;  
	}
	
	return RUN_AGAIN;
}

static void ping_handler(int sig, siginfo_t *info, void *ucontext)
{
/*	puts("ping handler! \n");  */
	
	/* we got a ping: client is ready */
	client_ready = READY;
	
	/* reset watchdog counter */
	watchdog_params.counter = 0; 
}


/* takes pointer to str, delets from begining up to last "/" */
static void TrunctPath(char *pathname)
{
	char *curr;

	for (curr = pathname; *curr != '\0'; ++curr)
	{
		if (*curr == '/')
		{
			strcpy(pathname, curr + 1);
			curr = pathname;
		}
	}
	
	memmove(pathname + 2, pathname, strlen(pathname) + 1);
	pathname[0] = '.';
	pathname[1] = '/';
}
