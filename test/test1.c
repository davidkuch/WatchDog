#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

#include "watchdog.h"


int main(int argc, const char *argv[])
{
	int interval = 3;
	int i = 20;
	printf("test1 pid: %d \n", getpid());
	
	puts("am i newborn or ressurected?! \n");
	
	KeepMeAlive(argc, argv, interval);
	
	
	while(i > 0)
	{
		sleep(2);
		puts("i am here\n");
		--i;
	}
/*	
	printf("DNR  is called! \n");
	
	DNR();
	
	i = 10;
	
	while(i > 0)
	{
		sleep(2);
		puts("i am here\n");
		--i;
	}

*/
	return 0;
}
