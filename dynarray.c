#include <stdio.h>
#include <stdlib.h>

struct dynarray {
	void *array;
	size_t length;
	size_t capacity;
	size_t elem_size;
};

struct dynarray *dynarray_create(size_t capacity, size_t elem_size)
{
	void *array = calloc(capacity, elem_size);
	if (array == NULL)
		return NULL;

	struct dynarray *da = malloc(sizeof(struct dynarray));
	if (da == NULL) {
		free(array);
		return NULL;
	}

	da->array = array;
	da->length = 0;
	da->capacity = capacity;
	da->elem_size = elem_size;

	return da;
}

void dynarray_release(struct dynarray *da)
{
	if (da->array != NULL) {
		free(da->array);
		da->array = NULL;
	}
	free(da);
}

void *dynarray_push(struct dynarray *da)
{
	if (da->length == da->capacity) {
		da->capacity *= 2;
		da->array = realloc(da->array, da->capacity * da->elem_size);
	}

	da->length++;

	return da->array + da->elem_size * (da->length - 1);
}

void *dynarray_at(struct dynarray *da, size_t i)
{
	if (i >= da->length)
		return NULL;

	return da->array + da->elem_size * i;
}


struct op {
	enum { OP_COPY, OP_INSERT} type;
	size_t offset;
	size_t length;
};

int main()
{
	int i;
	struct dynarray *da = dynarray_create(16, sizeof(struct op));

	for (i = 0; i <= 20; i++) {
		struct op *op = dynarray_push(da);
		op->type = OP_COPY;
		op->offset = 3 * i;
		op->length = 5 * i;
	}

	printf("length: %zu, capacity: %zu\n", da->length, da->capacity);

	for (i = 0; i < da->length; i++) {
		struct op *op = dynarray_at(da, i);
		printf("offset = %zu, length = %zu\n", op->offset, op->length);
	}


	int max = 16;
	struct op *ops = calloc(max, sizeof(struct op));

	for (i = 0; i < max; i++) {
		ops[i].type = OP_INSERT;
		ops[i].offset = 7 * i;
		ops[i].length = 9 * i;
	}
	for (i = 0; i < max; i++) {
		struct op *op = ops + i;
		printf("o = %zu, l = %zu\n", op->offset, op->length);
	}

	return 0;
}
