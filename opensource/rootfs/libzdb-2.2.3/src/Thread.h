/*
 * Copyright (C) 2008 Tildeslash Ltd. All rights reserved.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */


#ifndef THREAD_INCLUDED
#define THREAD_INCLUDED

/**
 * This interface contains <b>Thread</b> and <b>Mutex</b> abstractions 
 * via Macros.
 *
 * @version \$Id: Thread.h,v 1.1 2008/11/12 13:25:56 wiley Exp $
 * @file
 */


#ifdef WIN32
/* ------------------------------------------ Thread interface for Windows */
#include <process.h>
#define _WIN32_WINNT 0x400
#define Sem_T HANDLE
#define Mutex_T HANDLE
#define Thread_T HANDLE
#BE MY GUEST
#else
/* -------------------------------------------- Thread interface for POSIX */
#include <pthread.h>
#define Thread_T pthread_t
#define Sem_T   pthread_cond_t			  
#define Mutex_T pthread_mutex_t
#define ThreadData_T pthread_key_t
#define wrapper(F) do { int status= F; \
        if (status!=0 && status!=ETIMEDOUT) \
                ABORT("Thread: %s\n", strerror(status)); \
        } while (0)
#define Thread_create(thread, threadFunc, threadArgs) \
        wrapper(pthread_create(&thread, NULL, threadFunc, (void*)threadArgs))
#define Thread_self() pthread_self()
#define Thread_detach(thread) wrapper(pthread_detach(thread))
#define Thread_cancel(thread) wrapper(pthread_cancel(thread))
#define Thread_join(thread) wrapper(pthread_join(thread, NULL))
#define Sem_init(sem) wrapper(pthread_cond_init(&sem, NULL))
#define Sem_wait(sem, mutex) wrapper(pthread_cond_wait(&sem, &mutex))
#define Sem_signal(sem) wrapper(pthread_cond_signal(&sem))
#define Sem_destroy(sem) wrapper(pthread_cond_destroy(&sem))
#define Sem_timeWait(sem, mutex, time) \
        wrapper(pthread_cond_timedwait(&sem, &mutex, &time))
#define Mutex_init(mutex) wrapper(pthread_mutex_init(&mutex, NULL))
#define Mutex_destroy(mutex) wrapper(pthread_mutex_destroy(&mutex))
#define Mutex_lock(mutex) wrapper(pthread_mutex_lock(&mutex))
#define Mutex_unlock(mutex) wrapper(pthread_mutex_unlock(&mutex))
#define LOCK(mutex) do { Mutex_T *_yymutex= &(mutex); \
        pthread_mutex_lock(_yymutex);
#define END_LOCK pthread_mutex_unlock(_yymutex); } while (0)
#define ThreadData_create(key) wrapper(pthread_key_create(&(key), NULL))
#define ThreadData_set(key, value) pthread_setspecific((key), (value))
#define ThreadData_get(key) pthread_getspecific((key))
#endif

#endif
