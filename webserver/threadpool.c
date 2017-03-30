////////////////////////////////////////////////
// File:        threadpool.c                  //
// Author:      Sam Henry                     //
// Date:        March 17, 2017                //
// Class:       CpS 320, Unix Programming     //
// Desc:        Definitions for a threadpool  //
//				implementation in C           //
////////////////////////////////////////////////

#include "threadpool.h"

threadpool_t *threadpoolInit(int threadCount, int queueSize) {
	threadpool_t *pool;

	if(threadCount > MAX_THREADS || queueSize > MAX_QUEUE) {
		return NULL;
	}

	if((pool = (threadpool_t *) malloc(sizeof(threadpool_t))) == NULL) {
		return NULL;
	}

	pool->threadCount = 0;
	pool->queueSize = queueSize;
	pool->head = pool->tail = pool->count = pool->active = 0;
	pool->shutdown = pool->started = pool->verbose = false;

	pool->threads = (pthread_t*) malloc(sizeof(pthread_t) * threadCount);
	pool->queue = (client_info_t**) malloc(sizeof(client_info_t*) * queueSize);

	if(pthread_mutex_init(&(pool->lock), NULL) != 0 || 
		pthread_cond_init(&(pool->notify), NULL) != 0 || 
		pool->threads == NULL || pool->queue == NULL) {
		goto error;
	}

	for(int i = 0; i < threadCount; i++) {
		if(pthread_create(&(pool->threads[i]), NULL, threadpoolThread, (void*) pool) != 0) {
			threadpoolDestroy(pool);
			return NULL;
		}

		pool->threadCount++;
		pool->started++;
	}

	return pool;

error:
	if(pool) { threadpoolFree(pool); }
	return NULL;
}

int threadpoolAdd(threadpool_t *pool, client_info_t *arg) {
	int ret = -1;

	if(pool == NULL || arg == NULL) { return -1; }

	pthread_mutex_lock(&(pool->lock));	// Let us do our work

	// Overflow or invalid
	if(pool->count == pool->queueSize || pool->shutdown) { goto cleanup; }

	pool->queue[pool->tail] = arg;
	pool->tail = (pool->tail + 1) % pool->queueSize;
	pool->count += 1;

	if(pool->verbose) {
		printf(COLOR_CYAN "[ThreadPool]: Adding item to queue.\n" COLOR_RESET);
		printf(COLOR_CYAN "[ThreadPool]: Queue Items = %d, Active Threads = %d.\n" COLOR_RESET, pool->count, pool->active);
	}

	pthread_cond_signal(&(pool->notify));	// Signal any waiting thread that we have a new client!
	ret = 0;

cleanup:
	pthread_mutex_unlock(&(pool->lock));	// The pool is free again
	return ret;
}

int threadpoolDestroy(threadpool_t *pool) {
	if(pool == NULL) { return -1; }

	pthread_mutex_lock(&(pool->lock));

	if(pool->shutdown) { return -1; }
	pool->shutdown = true;

	if((pthread_cond_broadcast(&(pool->notify)) != 0) || (pthread_mutex_unlock(&(pool->lock)) != 0)) {
		return -1;
	}

	for(int i = 0; i < pool->threadCount; i++) {
		if(pthread_kill(pool->threads[i], SIGKILL) != 0) {	// KILL THEM ALL!
			return -1;
		}
	}

	threadpoolFree(pool);
	return 0;
}

int threadpoolFree(threadpool_t *pool) {
	if(pool == NULL) { return -1; }

	if(pool->threads) {
		free(pool->threads);
		free(pool->queue);

		pthread_mutex_lock(&(pool->lock));
		pthread_mutex_destroy(&(pool->lock));
		pthread_cond_destroy(&(pool->notify));
	}

	free(pool);
	return 0;
}

void *threadpoolThread(void *masterPool) {
	threadpool_t *pool = (threadpool_t *) masterPool;
	client_info_t *client = NULL;

	while(true) {
		pthread_mutex_lock(&(pool->lock));

		while((pool->count == 0) && (!pool->shutdown)) {
			pthread_cond_wait(&(pool->notify), &(pool->lock));
		}

		if(pool->count == 0 || pool->shutdown) { break; }

		client = pool->queue[pool->head];
		pool->head = (pool->head + 1) % pool->queueSize;
		pool->count--;
		pool->active++;

		pthread_mutex_unlock(&(pool->lock));
		
		handle_client(client);

		pthread_mutex_lock(&(pool->lock));
		pool->active--;
		pthread_mutex_unlock(&(pool->lock));
	}

	pool->started--;

	pthread_mutex_unlock(&(pool->lock));
	pthread_exit(NULL);
	return NULL;
}