# Round-Robin-Scheduler-for-Multithreading

This is wrote in the Ubuntu Environment with C.

Timeslice of thread is 1 second.


## How to execute

1. Type 'make' in the terminal to make execute file 'a'.

2-1. Type './a 1' to run the following functions.

> Create thread 1, 2, 3, 4, 5 and execute each threads in order.

> Continue the iteration for 5 times without terminsting threads.



2-2. Type './a 2' to run the following functions.

> Create thread 1, 2, 3, 4, 5 and execute each threads in order.

> Continue the iteration for 10 times and terminate through joining threads.



2-3. Type './a 3' to run the following functions.

> Create thread 1, 2, 3, 4, 5 and execute each threads in order.

> Suspend threads in order and Resume threads in order after stopping all threads.

  

## Round Robin Schedular

        ReadyQTail                                          ReadyQHead
         --------        --------        --------        ----------------
        |        |  ->  |        | .... |        |  ->  | Being Executed |
         --------        --------        --------        ----------------
                            pNext <----> pPrev
                            

        WaitQTail                                       WaitQHead
         --------        --------        --------        --------
        |        |  ->  |        | .... |        |  ->  |        |
         --------        --------        --------        --------
                            pNext <----> pPrev
