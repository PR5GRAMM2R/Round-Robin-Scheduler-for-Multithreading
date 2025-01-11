#include "Thread_WaitingQueue.h"            // 광운대학교 소프트웨어학과 2020203011 배주환
#include <stdio.h>                          // KWANGWOON University Software Department 2020203011 Bae Juhwan
#include <stdlib.h>


////////////////////////--- WaitingQueue Functions ---////////////////////////

/*
        WaitQTail                                       WaitQHead
         --------        --------        --------        --------
        |        |  ->  |        | .... |        |  ->  |        |
         --------        --------        --------        --------
                            pNext <----> pPrev
*/

//Thread * WaitQHead = NULL;
//Thread * WaitQTail = NULL;

int findWaitingThreadWithTid(thread_t tid) // 해당 pthread (tid) 에 따른 스레드가 있는지 판단.
{
    if(WaitQHead == NULL){
        // printf("There is no Thread in the Ready Queue.\n");
        exit(-2);
        //return -1;
    }
    
    for(Thread * t = WaitQHead; t != NULL; t = t->pNext){
        if(t->tid == tid){
            // printf("Found Waiting Thread that has %lu tid.\n", t->tid);
            return 0;
        }
    }

    // printf("There is no Thread that matches.\n");
    return -1;
}

Thread* referenceWaitingThreadWithTid(thread_t tid) // 해당 pthread (tid) 에 따른 Waiting 스레드가 있는지 찾고 그것을 참조.
{
    if(WaitQHead == NULL){
        // printf("There is no Thread in the Ready Queue.\n");
        exit(-9);
        //return -1;
    }
    
    for(Thread * t = WaitQHead; t != NULL; t = t->pNext){
        if(t->tid == tid){
            // printf("Found Thread that has %lu tid.\n", t->tid);
            return t;
        }
    }

    // printf("There is no Thread that matches.\n");
    return NULL;
}

void putToWaitingThread(Thread* t) // Waiting Queue 에 해당 스레드를 집어넣음.
{
    if(WaitQHead == NULL){
        WaitQHead = t;
        WaitQTail = t;
        t->pNext = NULL;
        t->pPrev = NULL;
        return;
    }

    Thread* temp = WaitQTail;
    WaitQTail = t;
    t->pPrev = temp;
    temp->pNext = t;
    t->pNext = NULL;

    return;
}

Thread* getWaitingThreadWithTid(thread_t tid) // 해당 pthread (tid) 에 따른 스레드가 있는지 판단.
{
    if(findWaitingThreadWithTid(tid) < 0){
        // printf("Cannot get %lu tid Thread from Waiting Queue.\n", tid);
        exit(-5);
        // return NULL;
    }
    else{
        for(Thread * t = WaitQHead; t != NULL; t = t->pNext){
            if(t->tid == tid){
                // printf("Found Waiting Thread that has %lu tid.\n", t->tid);

                if(t == WaitQHead && t == WaitQTail){
                    Thread* temp = WaitQHead;
                    WaitQHead = NULL;
                    WaitQTail = NULL;
                    
                    temp->pNext = NULL;
                    temp->pPrev = NULL;

                    return temp;
                }
                else if(t == WaitQHead){
                    Thread* temp = WaitQHead;
                    WaitQHead = WaitQHead->pNext;
                    WaitQHead->pPrev = NULL;

                    temp->pNext = NULL;
                    temp->pPrev = NULL;

                    return temp;
                }
                else if(t == WaitQTail){
                    Thread* temp = WaitQTail;
                    WaitQTail = WaitQTail->pPrev;
                    WaitQTail->pNext = NULL;

                    temp->pNext = NULL;
                    temp->pPrev = NULL;

                    return temp;
                }

                Thread* tempNext = t->pNext;
                Thread* tempPrev = t->pPrev;

                tempNext->pPrev = tempPrev;
                tempPrev->pNext = tempNext;

                t->pNext = NULL;
                t->pPrev = NULL;

                return t;
            }
        }
    }
}

////////////////////////--- WaitingQueue Functions ---////////////////////////