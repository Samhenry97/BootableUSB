////////////////////////////////////////////////
// File:        threadpool.h                  //
// Author:      Sam Henry                     //
// Date:        March 17, 2017                //
// Class:       CpS 320, Unix Programming     //
// Desc:        Declarations for a threadpool //
//				implementation in C           //
////////////////////////////////////////////////

#ifndef THREADPOOL_H
#define THREADPOOL_H

#include <pthread.h>    // Threading library (pthreads)
#include <signal.h>
#include <stdbool.h>	// bool/true/false
#include <stdlib.h>
#include <stdio.h>

#include "eznet.h"		// Access to client_info_t
#include "utils.h"		// Access to other methods

// The thread pool's limits - we don't want this to get TOO crazy ;)
#define MAX_THREADS 256
#define MAX_QUEUE 268435456 // 2 ** 28

// This structure holds data for a threadpool instance.
// The threadpool has worker threads, which grab data
// out of a queue. It is basically a way of doing the 
// producer-consumer problem. One method sends data into the
// queue, and the threads wait for data coming out.
// https://github.com/mbrossard/threadpool/blob/master/src/threadpool.c
// This was a fun thing to implement because it goes along with OS well ;)
typedef struct threadpool {
	pthread_mutex_t lock;		// Mutual exclusion for thread access
	pthread_cond_t notify;		// Condition variable for notifying when item arrives
	pthread_t *threads;			// Threads in the pool
	client_info_t **queue;		// The queue of clients
	int threadCount;			// Total worker threads
	int queueSize;				// Total size of queue
	int head, tail;				// Start and end of queue (array)
	int count;					// Total items in queue (condition variable)
	int active;					// Total active threads
	bool started;				// If server has running worker threads
	bool shutdown;				// Are we shutting down?
	bool verbose;				// Extra debug info?
} threadpool_t;

// Initializes a threadpool, claiming any resources that it needs.
// <threadCount> The number of worker threads in the pool
// <queueSize> The size of the queue for data being passed in.
// <return> The newly created threadpool
threadpool_t *threadpoolInit(int threadCount, int queueSize);

// Adds some new data (client_info) into the threadpool's queue for
// a worker thread to get onto. 
// <pool> The threadpool
// <arg> The data to add
int threadpoolAdd(threadpool_t *pool, struct client_info *arg);

// Destroys a threadpool. It shuts down any currently running threads,
// cleans up, and then calls threadpoolFree(pool);
// <pool> The pool to destroy
// <return> 0 on success.
int threadpoolDestroy(threadpool_t *pool);

// Frees the resources allocated to a threadpool. We don't want to waist memory!
// <pool> The threadpool to free
// <return> 0 on success.
int threadpoolFree(threadpool_t *pool);

// This method is what every thread in the threadpool runs. It uses
// a condition variable to notify when data has arrived. When data arives,
// it calls handle_client to process the client and send back the right data.
// <masterPool> The pool that this thread is associated with
void *threadpoolThread(void *masterPool);

#endif // THREADPOOL_H