#include "Scheduler.h"                      // 광운대학교 소프트웨어학과 2020203011 배주환
#include "Thread_ReadyQueue.h"              // KWANGWOON University Software Department 2020203011 Bae Juhwan
#include "Make_Thread_Run_or_Ready.h"
#include <signal.h>
#include <unistd.h>                 // TestCase 모두가 기본적으로 스레드 함수에서 sleep(2); 명령어를 먼저 실행하기 때문에,
#include <stdio.h>                  // 프로그램 시작 후 약 6초 뒤에야 터미널에 출력들이 보일 것입니다.
#include <stdlib.h>                 // TestCase 특성 상 그러한 것이니, 실행 후에 조금만 기다려주시면 감사드리겠습니다!

void __ContextSwitch(Thread* pCurThread, Thread* pNewThread)
{    
    __thread_to_run(pNewThread);

    pthread_kill(pCurThread->tid, SIGUSR1);
}

int	RunScheduler( void )
{
    int start = 0;
    int second = 0; //
    int thread_count_prev = 0;
    int thread_count_curr = 0;

    while(1){
        sleep(TIMESLICE);

        // printf("%d second passed,\n", second); //
        // viewReadyQueue();//


        if(ReadyQHead != NULL){
            Thread* pCurThread = findWhichThreadIsBeingExecuted();
            thread_count_prev = thread_count_curr;
            thread_count_curr = howManyThreadsInTheReadyQueue();

            // if(start == 0 || thread_count_curr == thread_count_prev - 1){
            if(start == 0){
                start = 1;
                pCurThread->status = THREAD_STATUS_RUN;

                __thread_to_run(pCurThread);

                // printf("%lu thread runs.\n", pCurThread->tid); //
            }
            else{
                if(ReadyQHead == ReadyQTail){
                    second++;
                    continue;
                }
                else{
                    putToReadyThread(getThreadBeingExecuted());
                    Thread* pNewThread = findWhichThreadIsBeingExecuted();

                    __ContextSwitch(pCurThread, pNewThread);

                    pCurThread->status = THREAD_STATUS_READY;
                    pNewThread->status = THREAD_STATUS_RUN;

                    // printf("%lu thread runs, %lu thread blocks.\n", pNewThread->tid, pCurThread->tid); //
                }
                
                // if(pCurThread->pNext != NULL){
                //     pCurThread->bRunnable = 0;
                //     Thread* pNewThread = pCurThread->pNext;

                //     putToReadyThread(getReadyThreadWithTid(pCurThread->tid));
                //     pCurThread->status = THREAD_STATUS_READY;
                //     pNewThread->status = THREAD_STATUS_RUN;

                //     __ContextSwitch(pCurThread, pNewThread);
                // }
            }
        }
        else{
            start = 0;
            thread_count_prev = 0;
            thread_count_curr = 0;
        }

        second++;
    }

    return 0;
}

// int	RunScheduler( void )
// {
//     pthread_t t;
//     int arg;

//     pthread_create(&t, NULL, Scheduler, &arg);

//     return 0;
// }