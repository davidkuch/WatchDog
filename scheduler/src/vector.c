#include <stdlib.h> /* malloc, realloc */
#include <assert.h> /* assert */
#include <string.h> /* memset */
#include "vector.h"

#define SIZE (size_t)(vector->last - vector->first)
#define GROW_PARAM 2

struct vector
{
    void **last;
    void **first;
    size_t capacity; 
};

/* return NULL if failed */
vector_ty *VectorCreate(size_t size, size_t capacity_elements)
{		
	 vector_ty *vector = (vector_ty *)malloc(1 * sizeof(vector_ty));
	 void **arr = (void **)malloc(capacity_elements * sizeof(void **));
	 
	 assert(vector != NULL && arr != NULL);
	 
	 if (vector == NULL || arr == NULL)
	 {
	 	free(vector);
	 	free(arr);
	 	return NULL;
	 }
	 
	 vector->first = arr;
	 vector->last = arr + size;
     vector->capacity = capacity_elements;
	 	
	 memset(vector->first, 0, size * sizeof(void **));
	 
	 return vector;
}

/* setting index bigger than size or negative is undefined */
void VectorSet(vector_ty *vector, void *value, size_t index)
{
	assert(vector != NULL);
	assert(index < SIZE);
	
	*(vector->first + index) = value;
}

void *VectorGet(vector_ty *vector, size_t index)
{
	assert(vector != NULL);
	assert(index < SIZE);
	
	return *(vector->first + index);
}


size_t VectorCapacity(vector_ty *vector)
{
	assert(vector != NULL);
	
	return vector->capacity;
}


size_t VectorSize(vector_ty *vector)
{
	assert(vector != NULL);
	
	return SIZE;
}


int VectorPushBack(vector_ty *vector, void *item)
{
	int err = 0;
	
	assert(vector != NULL);
	
	if (SIZE == vector->capacity) /* reason to realloc */
	{
		err = VectorReserve(vector, vector->capacity * GROW_PARAM);
		if (err != 0)
		{
			return 1;
		}
	}
	
	*vector->last = item; 
	++vector->last;	
	
	return err;
}

int VectorShrink(vector_ty *vector)
{
	int err = 0;
	
	assert(vector != NULL);
	
	err = VectorReserve(vector, SIZE);
	vector->capacity = SIZE;
	
	return err;
}

int VectorResize(vector_ty *vector, size_t new_size)
{
	assert(vector != NULL);
	assert(new_size < vector->capacity);
	
	vector->last =  vector->first + new_size;
	
	return 0; /* TODO: fix err return value */
}

int VectorReserve(vector_ty *vector, size_t new_capacity_elements)
{
	size_t size = SIZE;
	size_t new_capacity_bytes = new_capacity_elements * sizeof(void **);
	void **new_location = NULL;
	
	assert(vector != NULL);
	
	new_location = realloc((void *)vector->first, new_capacity_bytes);
	
	if (new_location == NULL)
	{
		return 1;
	}
	
	vector->first = new_location; 
	vector->last = vector->first + size;
	vector->capacity = new_capacity_elements;
	
	return 0; 
}

void *VectorPopBack(vector_ty *vector)
{
	assert(vector != NULL);
	assert(SIZE != 0);
	
	--vector->last;

	return *(vector->last);
	
}
void VectorDestroy(vector_ty *vector)
{
	assert(vector != NULL);
	
	free(vector->first);
	free(vector);	
}

