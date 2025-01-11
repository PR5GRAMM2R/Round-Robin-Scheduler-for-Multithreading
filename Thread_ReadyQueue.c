#include "Thread_ReadyQueue.h"              // 광운대학교 소프트웨어학과 2020203011 배주환
#include <stdio.h>                          // KWANGWOON University Software Department 2020203011 Bae Juhwan
#include <stdlib.h>

////////////////////////--- ReadyQueue Functions ---////////////////////////

/*
        ReadyQTail                                          ReadyQHead
         --------        --------        --------        ----------------
        |        |  ->  |        | .... |        |  ->  | Being Executed |
         --------        --------        --------        ----------------
                            pNext <----> pPrev
*/

//Thread * ReadyQHead = NULL;
//Thread * ReadyQTail = NULL;

int findReadyThreadWithTid(thread_t tid) // 해당 pthread (tid) 에 따른 스레드가 있는지 찾기.
{
    if(ReadyQHead == NULL){
        // printf("There is no Thread in the Ready Queue.\n");
        exit(-1);
        //return -1;
    }
    
    for(Thread * t = ReadyQHead; t != NULL; t = t->pNext){
        if(t->tid == tid){
            // printf("Found Thread that has %lu tid.\n", t->tid);
            return 0;
        }
    }

    // printf("There is no Thread that matches.\n");
    return -1;
}

Thread* referenceReadyThreadWithTid(thread_t tid) // 해당 pthread (tid) 에 따른 Ready 스레드가 있는지 찾고 그것을 참조.
{
    if(ReadyQHead == NULL){
        // printf("There is no Thread in the Ready Queue.\n");
        exit(-1);
        //return -1;
    }
    
    for(Thread * t = ReadyQHead; t != NULL; t = t->pNext){
        if(t->tid == tid){
            // printf("Found Thread that has %lu tid.\n", t->tid);
            return t;
        }
    }

    // printf("There is no Thread that matches.\n");
    return NULL;
}

Thread* findWhichThreadIsBeingExecuted() // Ready Queue 에서 실행중이던 Head 스레드를 참조.
{
    if(ReadyQHead == NULL){
        // printf("There is no Thread to get out in the Ready Queue.\n");
        exit(-7);
        //return NULL;
    }

    return ReadyQHead;
}

Thread* getThreadBeingExecuted() // Ready Queue 에서 실행중이던 Head 스레드를 꺼냄.
{
    if(ReadyQHead == NULL){
        // printf("There is no Thread to get out in the Ready Queue.\n");
        exit(-3);
        //return NULL;
    }

    if(ReadyQHead == ReadyQTail){
        Thread* temp = ReadyQHead;
        ReadyQHead = NULL;
        ReadyQTail = NULL;

        temp->pNext = NULL;
        temp->pPrev = NULL;

        return temp;
    }

    Thread* temp = ReadyQHead;
    ReadyQHead = temp->pNext;
    ReadyQHead->pPrev = NULL;

    temp->pNext = NULL;
    temp->pPrev = NULL;
    
    return temp;
}

void putToReadyThread(Thread* t) // Ready Queue 에 해당 스레드를 집어넣음.
{
    if(ReadyQHead == NULL){
        ReadyQHead = t;
        ReadyQTail = t;
        t->pNext = NULL;
        t->pPrev = NULL;

        return;
    }

    Thread* temp = ReadyQTail;
    ReadyQTail = t;
    t->pPrev = temp;
    temp->pNext = t;
    t->pNext = NULL;

    return;
}

Thread* getReadyThreadWithTid(thread_t tid) // 해당 pthread (tid) 에 따른 스레드를 꺼냄.
{
    if(findReadyThreadWithTid(tid) < 0){
        // printf("Cannot get %lu tid Thread from Ready Queue.\n", tid);
        exit(-4);
        // return NULL;
    }
    else{
        for(Thread * t = ReadyQHead; t != NULL; t = t->pNext){
            if(t->tid == tid){
                // printf("Getting Thread that has %lu tid.\n", t->tid);

                if(t == ReadyQHead && t == ReadyQTail){
                    Thread* temp = ReadyQHead;
                    ReadyQHead = NULL;
                    ReadyQTail = NULL;

                    temp->pNext = NULL;
                    temp->pPrev = NULL;

                    return temp;
                }
                else if(t == ReadyQHead){
                    Thread* temp = ReadyQHead;
                    ReadyQHead = ReadyQHead->pNext;
                    ReadyQHead->pPrev = NULL;

                    temp->pNext = NULL;
                    temp->pPrev = NULL;

                    return temp;
                }
                else if(t == ReadyQTail){
                    Thread* temp = ReadyQTail;
                    ReadyQTail = ReadyQTail->pPrev;
                    ReadyQTail->pNext = NULL;

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

void viewReadyQueue() // Ready Queue 에서 Head 부터 순차적으로 Ready 스레드들의 id 를 출력.
{
    Thread* t;
    int count = 1;
    for(t = ReadyQHead; t != NULL; t = t->pNext){
        // printf("%d th Thread's tid is : %lu \n", count, t->tid);
        count++;
    }

    return;
}

int howManyThreadsInTheReadyQueue() // Ready Queue 에 얼마나 많은 Ready 스레드가 있는지 세어서 그 개수를 반환.
{
    Thread* t;
    int count = 1;
    for(t = ReadyQHead; t != NULL; t = t->pNext){
        count++;
    }

    return count;
}

////////////////////////--- ReadyQueue Functions ---////////////////////////