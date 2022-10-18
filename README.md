# Watchdog
A program that checks that a given client-process is running. Ii the client failed - the watchdog would restart it.


The current Project provides an API  with two main functions: run and stop watching.


### WD API Design
The program includes the following modules
Public to the user
- watchdog.h

Private modules
- keep_watching.h
- scheduler.h

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
