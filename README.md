# Watchdog
A program that checks that a given client-process is running. If the client failed - the watchdog would restart it.


The current Project provides an API  with two main functions: run and stop watching.


### WD API Design
The program includes the following modules
Public to the user
- watchdog.h

Private modules
- keep_watching.h
- scheduler.h

### Flow
This implementation is based on signal-passing.<br>
Both the client process and the watchdog share one function in common: KeepWatching. <br>
The function takes a PID of a process to watch as argument, creates a Scheduler that sends signals to indicate that it is alive,
and expects to get such signals as well.
If no signal arrives and the time limit is finished, it restarts the other process.
So, the call to KeepWatching is a blocking call. That is fine for the watchdog app, but not for the client!
To do so, the wrapper for the client starts a new thread, where the call is made.
When StopWatching is called, the thread is terminated.

## Usage
Download the following repo in order to use the watchdog.
Run `make` cmd to build all the necessary files.

```bash
    make watchdog
```
TODO : project file structure have changed, makefile has to be updated

* Make sure both files exists in your main program directory.



## Tests
See `./test` directory.
