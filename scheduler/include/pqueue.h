#ifndef __PQUEUE_H__
#define __PQUEUE_H__

#include <stddef.h>

typedef struct pqueue pqueue_ty;
typedef struct pqueue_itr pqueue_itr_ty;

/* 0  match; positive value obj1 > obj2; negative  otherwise */
typedef int (*PQCmpFunc)(const void *object1, const void *object2, const void *cmp_param);

pqueue_ty *PQueueCreate(const PQCmpFunc cmp_func_p, const void *cmp_param);


/*  O(pqueue_size) */
void PQueueDestroy(pqueue_ty *pqueue);

/*  O(pqueue_size) */
int PQueueEnqueue(pqueue_ty *pqueue, void *data);

/*  O(1) */
void PQueueDequeue(pqueue_ty *pqueue);

/* O(1) */
void *PQueuePeek(const pqueue_ty *pqueue);

/* O(1)  */
int PQueueIsEmpty(const pqueue_ty *pqueue);

/* O(pqueue_size) */
size_t PQueueCount(const pqueue_ty *pqueue);

/* O(pqueue_size) */
void PQueueClear(pqueue_ty *pqueue);

/* typedef int (*IsMatchFunc)(const void *data, const void *param); */ 
typedef  int (*PQIsMatch) (const void *,const void *); 
/* typedef IsMatchFunc PQIsMatch; */
/*  O(pqueue_size) */
void *PQueueErase(pqueue_ty *pqueue,  PQIsMatch match_func_p, const void *cmp_param); 


#endif /* __PQUEUE_H__ */


