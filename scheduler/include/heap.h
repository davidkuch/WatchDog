#ifndef __HEAP_H__
#define __HEAP_H__

#include "vector.h"

typedef struct heap heap_ty;





/* 0 or Positive (data_a >= data_b);
        negative (data_a < data_b)
*/
typedef int (*heap_cmp_ty)(const void *data_a, const void *data_b, const void *param);


heap_ty *HeapCreate(heap_cmp_ty heap_cmp_func ,const void *param);



void HeapDestroy(heap_ty *heap);


int HeapPush(heap_ty *heap, void *to_add);


void HeapPop(heap_ty *heap);


void *HeapPeek(const heap_ty *heap);


size_t HeapSize(const heap_ty *heap);


int HeapIsEmpty(const heap_ty *heap);

/* match = 0 */
typedef int (*should_remove_ty)(const void *data,const  void *param);


void *HeapRemove(heap_ty *heap, should_remove_ty remove_func,const void *to_remove);


#endif 


