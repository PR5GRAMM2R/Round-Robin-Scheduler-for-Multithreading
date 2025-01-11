#include "Init.h"                           // 광운대학교 소프트웨어학과 2020203011 배주환
#include "Thread.h"                         // KWANGWOON University Software Department 2020203011 Bae Juhwan
#include <signal.h>
#include <pthread.h>
#include "Make_Thread_Run_or_Ready.h"

void Init(void)
{
    ReadyQHead = NULL;
    ReadyQTail = NULL;
    WaitQHead = NULL;
    WaitQTail = NULL;

    pthread_mutex_init(&scheduler_mutex, NULL);
    pthread_cond_init(&scheduler_cond, NULL);

    return;
}