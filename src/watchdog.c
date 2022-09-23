#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <pthread.h>

#include "watchdog.h"
#include "watch_imp.h"
#include <stdio.h>
#define ARG_BUFFER 100

static void SetSignalHandlerSetThread(void);
static void ready_handler(int sig, siginfo_t *info, void *ucontext);
static void CreateThread(void);
static void PrepareOurArgv(int argc, const char *argv[],int interval);


/* needs to get the args the user app got from command line */
int KeepMeAlive(int argc, const char *argv[], size_t interval)
{
	pid_t forked;

	/* init global watchdog_params struct */

	/*  SAME counter */
	watchdog_params.counter = 0;
	
	/*  SAME interval  */
	watchdog_params.interval = interval;
	
	/* SAME argv[] to pass in exec: our_exe, interval, user_app, user args... , NULL */
	PrepareOurArgv(argc, argv, interval);
	
	/* handles ping: resets clock */
	SetSignalHandler();
	
	/* case NEWBORN */
	if (getenv("WD_PID") == NULL)
	{
		puts("new-born! \n");
		
		/* starts watch thread on "ready" signal */
		SetSignalHandlerSetThread();
			
		forked = fork();
		
		/* for parent, client procces: */
		watchdog_params.watch_proc = forked;
		
		/* child procces */
		if (forked == 0)
		{
			/* change to our executable */
			execv(watchdog_params.our_argv[0], watchdog_params.our_argv);
			
			puts("execv didn't work...\n");
			exit(0);
		}
	
	}
	else /* case REVIVED */
	{
		puts("revived!\n");
		
		watchdog_params.watch_proc = getppid();
		
		/* no wait for signal "ready" */
		CreateThread();
	}
	
	return 0;
}


int DNR(void)
{
	 /* kill watchdog procces */
	 kill(watchdog_params.watch_proc, SIGKILL);
	 
	 /* kill user-app thread watch */
	 pthread_cancel(watchdog_params.watch_thread);
	 
	 /* free allocations of argv */
	 
	 
	 return 0;
}


static void SetSignalHandlerSetThread(void)
{
	struct sigaction sa;


	/* init the sigaction struct */
  	memset(&sa, 0, sizeof(sa));
  	sa.sa_sigaction = ready_handler;

  	/* register a signal handler */
	sigaction(SIGUSR1, &sa, 0);

}


static void ready_handler(int sig, siginfo_t *info, void *ucontext)
{
	puts("ready signal handler!\n");
	
	CreateThread();
}


static void CreateThread(void)
{
	/* set pinging thread */
	pthread_create(&watchdog_params.watch_thread, NULL, Watch, watchdog_params.our_argv);
}


static void PrepareOurArgv(int argc, const char *argv[],int interval)
{
	int i = 0;
	char user_app_path[ARG_BUFFER] = {0};
	
	/* prepare path postfixed with "/" */
	getcwd(user_app_path,ARG_BUFFER);
	strcat(user_app_path, "/");

	for (i = 0; i < argc + 3; ++i)
	{
		watchdog_params.our_argv[i] = (char *)malloc(ARG_BUFFER);
	}

	/* our_exe */
	strcpy(watchdog_params.our_argv[0], user_app_path);    /* copy path */
	strcat(watchdog_params.our_argv[0], "our_watchdog"); /* concat our exe filename */
	
	/* interval */
	*watchdog_params.our_argv[1] = interval;
	
	/* user app path+filename */
	strcpy(watchdog_params.our_argv[2], user_app_path); /* copy path */
	strcat(watchdog_params.our_argv[2], argv[0] + 2); /* concat user filename, avoid the "./" */
	
	/* copy user args */
	for (i = 3; i < argc + 2; ++i)
	{
		watchdog_params.our_argv[i] = argv[i -2];
	}

	watchdog_params.our_argv[argc + 2] = NULL;
}


/*
	for( i = 0; i <= 100; ++i)
	{
		if (watchdog_params.our_argv[i] == NULL)
		{
			puts("NULL \n");
			break;
		}
		
		printf("argv[%d] = %s \n", i , watchdog_params.our_argv[i]);
		
	}
	*/
