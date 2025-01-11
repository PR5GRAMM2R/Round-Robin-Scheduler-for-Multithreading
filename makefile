CC = gcc

all : scheduler

scheduler : main.o Thread_WaitingQueue.o Thread_ReadyQueue.o Thread.o TestCase1.o TestCase2.o TestCase3.o Scheduler.o Make_Thread_Run_or_Ready.o Init.o
	$(CC) -o a main.o Thread_WaitingQueue.o Thread_ReadyQueue.o Thread.o TestCase1.o TestCase2.o TestCase3.o Scheduler.o Make_Thread_Run_or_Ready.o Init.o

Init.o :
	$(CC) -c Init.c

Make_Thread_Run_or_Ready.o :
	$(CC) -c Make_Thread_Run_or_Ready.c

Scheduler.o :
	$(CC) -c Scheduler.c

TestCase3.o :
	$(CC) -c TestCase3.c

TestCase2.o :
	$(CC) -c TestCase2.c

TestCase1.o : 
	$(CC) -c TestCase1.c

Thread.o :
	$(CC) -c Thread.c

Thread_ReadyQueue.o :
	$(CC) -c Thread_ReadyQueue.c

Thread_WaitingQueue.o :
	$(CC) -c Thread_WaitingQueue.c

main.o :
	$(CC) -c main.c
