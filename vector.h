#pragma once

typedef struct vector vector;

vector* create_vector(size_t item_size);
void destroy_vector(vector* obj);

void* push_back(vector* obj); //return memory to fill (dereference it first!)
void* pop_back(vector* obj); 

void* item_ref(vector* obj, size_t index);

size_t size(vector* obj);
void clear(vector* obj);