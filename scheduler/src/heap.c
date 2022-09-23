#include <stdlib.h>  /*  malloc, free */
#include <assert.h>

#include "vector.h"
#include "heap.h"

#define INIT_CAPACITY 100

#define FAIL 1
#define None 0

struct heap
{
      vector_ty *vector;
      heap_cmp_ty cmp_func;
      const void *param;
};

static void RemoveIMP(heap_ty *heap, size_t to_remove);

static size_t GetParentIMP(size_t index);

static size_t GetLeftChildIMP(size_t index);

static size_t GetRightChildIMP(size_t index);

static void HeapifyUpIMP(heap_ty *heap, size_t child);

static void HeapifyDownIMP(heap_ty *heap, size_t parent);

static size_t GetBadChild(heap_ty *heap, size_t parent);

static void Swap(heap_ty *heap, size_t a, size_t b);

static int IsBiggerIMP(heap_ty *heap, size_t a, size_t b);



heap_ty *HeapCreate(heap_cmp_ty cmp_func ,const void *param)
{
	/* malloc heap struct */
	heap_ty *heap = (heap_ty *)malloc(sizeof(heap_ty));
	
	if (heap == NULL)
	{
		return NULL;
	}
	
	/* call create vector() */
	heap->vector = VectorCreate(0, INIT_CAPACITY);
	
	if (heap->vector == NULL)
	{
		free(heap);
		
		return NULL;
	}
	
	assert(cmp_func != NULL);
	
	/* assign cmp_func */
	heap->cmp_func = cmp_func;
	
	/* assign param */
	heap->param = param;
	
	/* return heap */
	return heap;
}


void HeapDestroy(heap_ty *heap)
{
	assert(heap != NULL);	
	
	/* call vector destroy */
	VectorDestroy(heap->vector);
	
	/* free heap struct */
	free(heap);
}


int HeapPush(heap_ty *heap, void *to_add)
{
	assert(heap != NULL);
	assert(to_add != NULL);	

	/* call vector push back */
	if (VectorPushBack(heap->vector, to_add) == FAIL) 
	{
		return FAIL;
	}
	
	/* call heapify up on new last(by heap size) */
	HeapifyUpIMP(heap, HeapSize(heap) - 1);
	
	return 0;
}


void HeapPop(heap_ty *heap)
{
	assert(heap != NULL);
	assert(!HeapIsEmpty(heap));
	
	/* call removeIMP on first */
	RemoveIMP(heap, 0);
}


void *HeapPeek(const heap_ty *heap)
{
	assert(heap != NULL);
	assert(!HeapIsEmpty(heap));
	
	/* return vector get on first */
	return VectorGet(heap->vector, 0);
}


size_t HeapSize(const heap_ty *heap)
{
	assert(heap != NULL);
	
	return VectorSize(heap->vector);
}


int HeapIsEmpty(const heap_ty *heap)
{
	assert(heap != NULL);
	
	return HeapSize(heap) == 0;
}


void *HeapRemove(heap_ty *heap, should_remove_ty remove_func,const void *param)
{
	size_t heap_size;
	size_t i = 0;
	void *ret = NULL;
	
	assert(heap != NULL);
	assert(remove_func != NULL);
	
	heap_size = HeapSize(heap);
	
	/* check each item in vector with remove_func. */
	for (; i < heap_size; ++i)
	{	
		ret = VectorGet(heap->vector, i);
		
		/* if to remove: call-return removeIMP */
		if (remove_func(ret, param))
		{
			RemoveIMP(heap, i);
			
			break;
		}
		
		ret = NULL;
	}
	
	return ret;
}


static void RemoveIMP(heap_ty *heap, size_t to_remove)
{
	size_t last = HeapSize(heap) - 1;
	
	assert(to_remove < HeapSize(heap));
	
	/* swap with last */
	Swap(heap, to_remove, last);
	
	/* vector pop back */
	VectorPopBack(heap->vector);
	
	if (to_remove == last) /* special case, remove of last */
	{
		return;
	}
	
	/* call heapify down */
	HeapifyDownIMP(heap, to_remove);
}


static size_t GetParentIMP(size_t index)
{
	return (index - 1)  / 2;
}


static size_t GetLeftChildIMP(size_t index)
{
	return (index + 1) * 2 - 1;
}


static size_t GetRightChildIMP(size_t index)
{
	return (index + 1) * 2;
}


/* gets index where item possibly doesn't hold the
   heap property regarding its parent. */
static void HeapifyUpIMP(heap_ty *heap, size_t child)
{
	size_t parent = GetParentIMP(child);
	
	assert(child < HeapSize(heap));
	
	/* return if at head */
	if (child == 0)
	{
		return;
	}
	
	/* compare to the parent: return if property achieved */
	if (IsBiggerIMP(heap, parent, child))
	{
		return;
	}
	
	/* swap with parent */
	Swap(heap, parent, child);
	
	/* call on the new location*/
	HeapifyUpIMP(heap, parent);
}


/* gets index where item possibly doesn't hold the
   heap property regarding its children. */
static void HeapifyDownIMP(heap_ty *heap, size_t parent)
{
	/* find a child that needs swaping */
	size_t swap_with = GetBadChild(heap, parent);
	
	assert(parent < HeapSize(heap));
	
	/* return if property achieved */
	if (swap_with == None)
	{
		return;
	}
	
	/* swap with one that breaks the property */
	Swap(heap,parent ,swap_with);
	
	/*  call on the new location */
	HeapifyDownIMP(heap,swap_with);
}


/* returns index of child to swap with, if exists */
static size_t GetBadChild(heap_ty *heap, size_t parent)
{
	size_t heap_size = HeapSize(heap);
	
	size_t left_child = GetLeftChildIMP(parent);
	size_t right_child = GetRightChildIMP(parent);
	size_t maybe;
	
	/* if both exist - return the biggest of them if violates */
	if (right_child < heap_size) /* left is before right... no need to check */
	{
		maybe = right_child;
		
		if (IsBiggerIMP(heap, left_child ,right_child))
		{
			maybe =  left_child;
		}
		
		if (IsBiggerIMP(heap, maybe ,parent))
		{
			return maybe;
		}
		
		return None;
	}
	
	/* if only left exists - compare with it. return it if bad */
	if (left_child < heap_size)
	{
		if (!IsBiggerIMP(heap, parent ,left_child))
		{
			return left_child;
		}
	}
	
	return None;
}


static void Swap(heap_ty *heap, size_t a, size_t b)
{
	void *tmp;
	
	assert(a < HeapSize(heap));
	assert(b < HeapSize(heap));
	
	tmp = VectorGet(heap->vector, a);
	VectorSet(heap->vector, VectorGet(heap->vector,b), a);
	VectorSet(heap->vector,tmp , b);
}


static int IsBiggerIMP(heap_ty *heap, size_t a, size_t b)
{
	void *item_a;
	void *item_b;
	
	assert(a < HeapSize(heap));
	assert(b < HeapSize(heap));
	
	item_a = VectorGet(heap->vector, a);
	item_b = VectorGet(heap->vector, b);
	
	return (heap->cmp_func(item_a, item_b, heap->param) > 0);
}


