#include "Make_Thread_Run_or_Ready.h"       // 광운대학교 소프트웨어학과 2020203011 배주환
#include "Thread_ReadyQueue.h"              // KWANGWOON University Software Department 2020203011 Bae Juhwan
#include "Thread_WaitingQueue.h"
#include <stdlib.h>
#include <stdio.h>

int scheduler_continue = 0;
pthread_mutex_t scheduler_mutex;
pthread_cond_t scheduler_cond;

int retval_temp = 0; //?

/////////////////--- Make Thread to be running or ready ---/////////////////

void __thread_to_ready(int signo) // Signal Handler Function
{
    Thread* pTh;

    if((pTh = referenceReadyThreadWithTid(thread_self())) == NULL){
        pTh = referenceWaitingThreadWithTid(thread_self());
    }

    pthread_mutex_lock(&(pTh->readyMutex));

    // pTh->bRunnable = 1;

    while(pTh->bRunnable == 1)
        pthread_cond_wait(&(pTh->readyCond), &(pTh->readyMutex));

    pTh->bRunnable = 1;
    
    pthread_mutex_unlock(&(pTh->readyMutex));
}

void __thread_to_ready2(Thread* pTh) // 스레드 초기화 시 사용.
{
    pthread_mutex_lock(&(pTh->readyMutex));

    pTh->bRunnable = 2;

    while(pTh->bRunnable == 2)
        pthread_cond_wait(&(pTh->readyCond), &(pTh->readyMutex));

    pTh->bRunnable = 1;
    
    pthread_mutex_unlock(&(pTh->readyMutex));
}

void __thread_to_run(Thread* pTh)
{
    pthread_mutex_lock(&(pTh->readyMutex));
    pTh->bRunnable = 0;
    pthread_cond_signal(&(pTh->readyCond));

    pthread_mutex_unlock(&(pTh->readyMutex));

    // pTh->bRunnable = 1;
}

void __thread_to_join(Thread* temp)
{
    pthread_mutex_lock(&(temp->zombieMutex));

    while(temp->bZombie == 0)
        pthread_cond_wait(&(temp->zombieCond), &(temp->zombieMutex));

    pthread_mutex_unlock(&(temp->zombieMutex));
}

void __thread_to_zombie(Thread* temp)
{
    pthread_mutex_lock(&(temp->zombieMutex));

    temp->bZombie = 1;
    pthread_cond_signal(&(temp->zombieCond));

    pthread_mutex_unlock(&(temp->zombieMutex));
}

/////////////////--- Make Thread to be running or ready ---/////////////////