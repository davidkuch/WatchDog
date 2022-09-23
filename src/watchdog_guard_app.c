#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <signal.h>

#include "watchdog.h"
#include "watch_imp.h"



/* ------ our executable ------- */

/* watch app */
int main(int argc, char *argv[])/* argv: our_exe, interval, user_app, user_args ..., NULL */
{
	/*(void)argc;
	*/
	puts("watchdog process is up and running!\n");
	printf("watchdog pid: %d \n", getpid());
	
	/* init global watchdog_params struct */
	
	/* process */
	watchdog_params.watch_proc = getppid();

	/* thread  UNUSED*/
	
	/* counter */
	watchdog_params.counter = 0;

	/* interval */
	watchdog_params.interval = *argv[1];

	/* sets a hanlder that resets counter on signal */
	SetSignalHandler();
	
	/* signal "ready" to the user app */
	kill(watchdog_params.watch_proc, SIGUSR1);

	puts("our_watchdog is about to call Watch()!\n");
	
	/* call Watch with argv - user_app, user args ... , NULL */
	Watch(argv + 2);
	
	
	return 0;
}
