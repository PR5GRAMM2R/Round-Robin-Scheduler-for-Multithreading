#ifndef __THREAD_READYQUEUE__               // 광운대학교 소프트웨어학과 2020203011 배주환
#define __THREAD_READYQUEUE__               // KWANGWOON University Software Department 2020203011 Bae Juhwan

#include "Thread.h"

int findReadyThreadWithTid(thread_t tid);

Thread* referenceReadyThreadWithTid(thread_t tid);

Thread* findWhichThreadIsBeingExecuted();

Thread* getThreadBeingExecuted();

void putToReadyThread(Thread* t);

Thread* getReadyThreadWithTid(thread_t tid);

void viewReadyQueue();

int howManyThreadsInTheReadyQueue();

#endif