#include <stdlib.h>
#include <assert.h>

#include "pqueue.h"
#include "heap.h"

struct pqueue
{
	heap_ty *heap;
};


pqueue_ty *PQueueCreate(const PQCmpFunc cmp_func, const void *param)
{
	pqueue_ty *pq = (pqueue_ty *)malloc(sizeof(pqueue_ty));
	
	if (pq == NULL)
	{
		return NULL;
	}
	
	assert(cmp_func != NULL);
	
	/* call heap create */
	pq->heap = HeapCreate(cmp_func ,param);
	
	if (pq->heap == NULL)
	{
		free(pq);
		
		return NULL;
	}
	
	return pq;
}


void PQueueDestroy(pqueue_ty *th_)
{
	assert(th_ != NULL);
	
	/* call heap destroy */
	HeapDestroy(th_->heap);
	
	free(th_);
}


int PQueueEnqueue(pqueue_ty *th_, void *data)
{
	assert(th_ != NULL);
	assert(data != NULL);
	
	/* call heap push */
	return HeapPush(th_->heap, data);
}


void PQueueDequeue(pqueue_ty *th_)
{
	assert(th_ != NULL);
	assert(!PQueueIsEmpty(th_));
	
	/* call heap pop */
	HeapPop(th_->heap);
}


void *PQueuePeek(const pqueue_ty *th_)
{	
	assert(th_ != NULL);
	assert(!PQueueIsEmpty(th_));

	/* call heap peek */
	return HeapPeek(th_->heap);
}


int PQueueIsEmpty(const pqueue_ty *th_)
{
	assert(th_ != NULL);
	
	/* check heap size == 0 */
	return  HeapSize(th_->heap) == 0;
}

size_t PQueueCount(const pqueue_ty *th_)
{
	assert(th_ != NULL);
	
	/* call heap size*/
	return  HeapSize(th_->heap);
}


void PQueueClear(pqueue_ty *th_)
{
	assert(th_ != NULL);
	
	/* pop until is empty */
	while (!PQueueIsEmpty(th_))
	{
		PQueueDequeue(th_);
	}
}


void *PQueueErase(pqueue_ty *th_,  PQIsMatch match_func, const void *param) 
{
	assert(th_ != NULL);
	assert(match_func != NULL);
	
	/* call heap remove if */
	return HeapRemove(th_->heap, match_func, param);
}
