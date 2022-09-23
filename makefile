
test1: sched 
	cc -I ./include -pthread test1.c \
	watch_imp.c watchdog.c  \
	scheduler.o task.o  uid.o Heap_Pq.o heap.o vector.o \
	-o test1




clean:
	rm *.o our_watchdog test1


our_watchdog: sched our_watchdog.o
	cc -I ./include -pthread  watch_imp.c our_watchdog.o \
	scheduler.o task.o  uid.o Heap_Pq.o heap.o vector.o \
	-o our_watchdog
	
	
	
our_watchdog.o: our_watchdog.c
	cc  -I ./include -pthread -c our_watchdog.c watch_imp.c



sched:  Heap_Pq  scheduler task  uid 
 


scheduler: ./src/scheduler.c
	cc -I ./include -c ./src/scheduler.c
	
	
uid : ./src/uid.c
	cc -I ./include -pthread -c ./src/uid.c
	
	
task:  
	cc -I ./include  -c ./src/task.c
	
	
Heap_Pq : ./src/Heap_Pq.c heap
	cc 	-I ./include -c ./src/Heap_Pq.c


heap: ./src/heap.c vector
	cc -I ./include -c ./src/heap.c
	
vector: ./src/vector.c
	cc -I ./include -c ./src/vector.c

