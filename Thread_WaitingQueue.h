#ifndef __THREAD_WAITINGQUEUE__             // 광운대학교 소프트웨어학과 2020203011 배주환
#define __THREAD_WAITINGQUEUE__             // KWANGWOON University Software Department 2020203011 Bae Juhwan

#include "Thread.h"

int findWaitingThreadWithTid(thread_t tid);

Thread* referenceWaitingThreadWithTid(thread_t tid);

void putToWaitingThread(Thread* t);

Thread* getWaitingThreadWithTid(thread_t tid);

#endif