#include <memory.h>
#include <stdlib.h>

#include "vector.h"

struct vector
{
	char* data;
	size_t item_size;
	size_t item_count;
	size_t pos;
};

vector* create_vector(size_t item_size)
{	
	vector* tmp = (vector*)malloc(sizeof(vector));

	tmp->item_size = item_size;
	tmp->item_count = 4;
	tmp->pos = 0;
	tmp->data = (char*)malloc(tmp->item_count * tmp->item_size);

	return tmp;
}

void destroy_vector(vector* obj)
{
	free(obj->data);
	free(obj);
}

void* push_back(vector* obj)
{
	if (obj->pos >= obj->item_count)
	{
		char* new_data = (char*)realloc(obj->data, 2 * obj->item_count * obj->item_size);

		if (new_data)
		{
			obj->data = new_data;
			obj->item_count *= 2;
		}

		else
			return NULL;
	}

	return obj->data + ((obj->pos++) * obj->item_size);
}

void* pop_back(vector* obj)
{	
	if (obj->pos == 0)
		return NULL;

	--obj->pos;

	return obj->data + (obj->pos * obj->item_size);
}

size_t size(vector* obj)
{
	return obj->pos;
}

void* item_ref(vector* obj, size_t index)
{
	if (index >= obj->pos)
		return NULL;

	return obj->data + (index * obj->item_size);
}

void clear(vector* obj)
{
	obj->pos = 0;
}