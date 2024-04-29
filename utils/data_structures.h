/*
 *  Name : Elowan
 *  Creation : 29-04-2024 16:48:07
 *  Last modified : 29-04-2024 19:55:39
 */
#ifndef DATA_STRUCTURES_H
#define DATA_STRUCTURES_H

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#define DEFINE_RESIZABLE(TYPE, NAME) \
/*\
Returns an empty resizable array\
*/\
resizable_array_ ## NAME init_resizable_array_ ## NAME(){\
    resizable_array_ ## NAME a;\
    a.size = 0;\
    a.capacity = 1;\
    a.array = calloc(1, sizeof(TYPE));\
    return a;\
}\
/*\
Appends an element to a resizable array\
Params:\
    - resizable_array_ ## NAME* t : The resizable array \
*/\
void append_resizbl_arr_ ## NAME(resizable_array_ ## NAME* a, TYPE element){\
    if (a->size+1 >= a->capacity){\
        a->array = realloc(a->array, a->capacity*2*sizeof(TYPE));\
        a->capacity = a->capacity*2;\
    }\
\
    a->array[a->size] = element;\
    a->size++;\
}\
/*\
Gets the element at the case `i` if exists, else NULL\
*/\
TYPE get_resizbl_arr_ ## NAME(resizable_array_ ## NAME a, int i){\
    assert (i < a.size);\
    return a.array[i];\
}\
/*\
Sets the element at the case `i` if i < capacity\
*/\
TYPE set_resizbl_arr_ ## NAME(resizable_array_ ## NAME* a, int i, TYPE element){\
    assert (i < a->capacity);\
    a->array[i] = element;\
    return element;\
}\
/*\
Free the resizable array\
*/\
void free_resizbl_arr_ ## NAME(resizable_array_ ## NAME a){\
    free(a.array);\
}\


#define EXPORT_RESIZABLE(TYPE, NAME)  typedef struct { \
    int size; \
    int capacity; \
    TYPE * array; \
} resizable_array_ ## NAME; \
resizable_array_ ## NAME init_resizable_array_ ## NAME();\
void append_resizbl_arr_ ## NAME(resizable_array_ ## NAME* a, TYPE element);\
TYPE get_resizbl_arr_ ## NAME(resizable_array_ ## NAME a, int i);\
TYPE set_resizbl_arr_ ## NAME(resizable_array_ ## NAME* a, int i, TYPE element);\
void free_resizbl_arr_ ## NAME(resizable_array_ ## NAME a);

void sortArrayDesc(void** keys, double* values, int n);

#endif