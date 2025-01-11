#include "Thread.h"                         // 광운대학교 소프트웨어학과 2020203011 배주환
#include "Thread_ReadyQueue.h"              // KWANGWOON University Software Department 2020203011 Bae Juhwan
#include "Thread_WaitingQueue.h"
#include "Make_Thread_Run_or_Ready.h"
#include <stdio.h>
#include <stdlib.h>                 // TestCase 모두가 기본적으로 스레드 함수에서 sleep(2); 명령어를 먼저 실행하기 때문에,
#include <signal.h>                 // 프로그램 시작 후 약 6초 뒤에야 터미널에 출력들이 보일 것입니다.
#include <pthread.h>                // TestCase 특성 상 그러한 것이니, 실행 후에 조금만 기다려주시면 감사드리겠습니다!

Thread*     ReadyQHead = NULL;
Thread*     ReadyQTail = NULL;

/* head and tail pointers for waiting queue */                      // 과제 수행 기간 : 2023년 12월 17일 (일) 오후 06시 58분 ~ 2023년 12월 22일 (금) 오전 04시 30분
Thread*     WaitQHead = NULL;
Thread*     WaitQTail = NULL;

/*
/////////////////////////////////////////////////////// --- 스레드의 생성 단계 --- ///////////////////////////////////////////////////////


            Main Thread -----> thread_create(. . .) ------> Wrapper Thread (사실상, New Thread Creating)
                                        |                           |
            Main Thread <----------------                           v
                |                       ^                   Wrapper Thread 의 준비 완료 ------
                :                       |                                                   |
                :                       ------ Send Signal to scheduler_cond <---------------
                :
                v
            RunScheduler(. . .) =====>> Ready 스레드들을 가지고 돌려가며 논다.

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
*/

void* __wrapperFunc(void* arg)
{
    void* ret;
    WrapperArg* pArg = (WrapperArg*) arg;
    void* (*funcPtr)(void* arg);
    funcPtr = pArg->funcPtr;
    void* funcArg = (void*)pArg->funcArg;

    // Thread* pTh = (Thread*) malloc(sizeof(Thread));

    Thread* pTh = pArg->pThread; ///

    pTh->tid = pthread_self();

    pthread_mutex_lock(&scheduler_mutex);

    pthread_cond_signal(&scheduler_cond);

    scheduler_continue = 1;

    pthread_mutex_unlock(&scheduler_mutex);

    __thread_to_ready2(pTh);

    signal(SIGUSR1, __thread_to_ready);

    // __thread_to_ready2(pTh, funcPtr, funcArg);

    // pthread_join((pTh->tid), (void *)&ret);

    ret = (*funcPtr)(funcArg);

    // free(pTh);

    return ret;
}

int thread_create(thread_t *thread, thread_attr_t *attr, void *(*start_routine) (void *), void *arg)
{
    Thread* newThread = (Thread*) malloc(sizeof(Thread));

    int wrapper_ret;

    WrapperArg wrapperArg; //  = (WrapperArg*) malloc(sizeof(WrapperArg));
    wrapperArg.funcPtr = start_routine;
    wrapperArg.funcArg = arg;

    // wrapperArg.pThread = (Thread*) malloc(sizeof(Thread));

    wrapperArg.pThread = newThread;

    pthread_mutex_init(&(newThread->readyMutex), NULL);
    pthread_cond_init(&(newThread->readyCond), NULL);
    pthread_mutex_init(&(newThread->zombieMutex), NULL);
    pthread_cond_init(&(newThread->zombieCond), NULL);
    newThread->status = THREAD_STATUS_READY;
    newThread->pExitCode = NULL;
    newThread->bRunnable = 0;
    newThread->bZombie = 0;
    newThread->parentTid = pthread_self();
    newThread->pNext = NULL;
    newThread->pPrev = NULL;
    putToReadyThread(newThread);

    if(pthread_create(thread, attr, __wrapperFunc, &wrapperArg) < 0){ ///
        // printf("wrapper_pthread_create error()!\n");
        exit(-6);
    }
    
    // pthread_join((newThread->tid), (void *)&wrapper_ret);

    //getReadyThreadWithTid(newThread->tid);

    pthread_mutex_lock(&scheduler_mutex);                               // 스레드가 새로 생성되고, 그 스레드가 __thread_to_ready2() 에서 wait 하기 전에 스케줄러가 멋대로 그 스레드를 가지고 놀 수 없도록
                                                                        // 따로 Make_Thread_Run_or_Ready.h 에서 extern 으로 scheduler_cond 와 scheduler_mutex, 그리고 scheduler_continue 를 따로 만들어서
    while(scheduler_continue == 0)                                      // 이렇게 사용하였습니다. (구현 유의 사항대로, 스레드의 실행 흐름의 제어에 pthread condition variable 만을 사용하였습니다.)
        pthread_cond_wait(&scheduler_cond, &scheduler_mutex);

    scheduler_continue = 0;

    pthread_mutex_unlock(&scheduler_mutex);
    
    return *thread;
}


int thread_join(thread_t thread, void **retval)
{
    Thread* temp = referenceReadyThreadWithTid(thread);

    ////////////////////////////////////////////////////////////////////////////
    // __thread_to_join

    __thread_to_join(temp);

    // pthread_mutex_lock(&(temp->zombieMutex));

    // while(temp->bZombie == 0)
    //     pthread_cond_wait(&(temp->zombieCond), &(temp->zombieMutex));

    // pthread_mutex_unlock(&(temp->zombieMutex));

    ////////////////////////////////////////////////////////////////////////////

    // retval = &(temp->pExitCode); // 전역변수로 따로 필요??

    retval_temp = *(int*)(temp->pExitCode);  // 필요.

    *retval = &retval_temp;

    getReadyThreadWithTid(thread);

    free(temp); //

    pthread_detach(thread);

    return 0;
}


int thread_suspend(thread_t tid)
{
    Thread* suspendThread = getReadyThreadWithTid(tid);
    putToWaitingThread(suspendThread);

    suspendThread->status = THREAD_STATUS_BLOCKED;

    return 0;
}


int	thread_resume(thread_t tid)
{
    Thread* resumeThread = getWaitingThreadWithTid(tid);
    putToReadyThread(resumeThread);

    resumeThread->status = THREAD_STATUS_READY;

    return 0;
}




thread_t thread_self()
{
    return pthread_self();      // pthread_self() 함수를 절대 사용하지 말라는 주의 사항이 언급되지 않았기도 하였고, 강의 시간에 이 방법 말고 다른 방법을 배우지 않아서, 일단은 이렇게 thread_self() 를 작성하였습니다.
}	                            // 스레드의 id 를 구하는 또 다른 방법들이 궁금하여 인터넷에 찾아보니, gettid() 또는 thrd_current() 라는 함수를 사용해야 함을 알 수 있었습니다.


int thread_exit(void* retval)
{
    Thread* temp = referenceReadyThreadWithTid(thread_self());

    temp->status = THREAD_STATUS_ZOMBIE;

    temp->pExitCode = retval;

    ////////////////////////////////////////////////////////////////////////////
    // __thread_to_zombie

    __thread_to_zombie(temp);

    // pthread_mutex_lock(&(temp->zombieMutex));

    // temp->bZombie = 1;
    // pthread_cond_signal(&(temp->zombieCond));

    // pthread_mutex_unlock(&(temp->zombieMutex));

    ////////////////////////////////////////////////////////////////////////////

    // free(temp); // thread_exit 는 join 되기 전까지 스레드를 완전히 삭제하지 않고 그 상태를 유지한다.

    // pthread_detach(thread_self());

    return 0;
}