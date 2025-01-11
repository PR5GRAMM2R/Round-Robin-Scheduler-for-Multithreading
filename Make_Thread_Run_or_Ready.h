#ifndef __MAKE_THREAD_RUN_OR_READY__        // 광운대학교 소프트웨어학과 2020203011 배주환
#define __MAKE_THREAD_RUN_OR_READY__        // KWANGWOON University Software Department 2020203011 Bae Juhwan

#include "Thread.h"

extern int scheduler_continue;
extern pthread_mutex_t scheduler_mutex;
extern pthread_cond_t scheduler_cond;

extern int retval_temp;

void __thread_to_ready(int signo);

void __thread_to_ready2(Thread* pTh);

void __thread_to_run(Thread* pTh);

void __thread_to_join(Thread* temp);

void __thread_to_zombie(Thread* temp);

#endif
