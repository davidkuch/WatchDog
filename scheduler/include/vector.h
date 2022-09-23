#ifndef __DYNAMIC_ARR_H__
#define __DYNAMIC_ARR_H__

typedef struct vector vector_ty;

/* return NULL if failed */
vector_ty *VectorCreate(size_t size, size_t capacity);

/* setting index bigger than size or negative is undefined */
void VectorSet(vector_ty *vector, void *value, size_t index);


void *VectorGet(vector_ty *vector, size_t index);


size_t VectorCapacity(vector_ty *vector);

size_t VectorSize(vector_ty *vector);

/* returns 0 on succes, 1 in fail */
int VectorPushBack(vector_ty *vector, void *item);

/* return 0 if succes */
int VectorShrink(vector_ty *vector);

int VectorResize(vector_ty *vector, size_t new_size);


int VectorReserve(vector_ty *vector, size_t new_capacity);


void *VectorPopBack(vector_ty *vector);

void VectorDestroy(vector_ty *vector);

#endif
