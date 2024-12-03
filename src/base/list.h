#ifndef LIST_H
#define LIST_H

#include "definitions.h"
#include "stdlib.h"
#include "string.h"

typedef struct ListHeader {
    u32 capacity;
    u32 length;
    u32 element_byte_size;
} ListHeader;

void* list_create(u32 initial_capacity, u32 element_byte_size);
void* list_ensure_capacity(void* list, u32 capacity);
void list_free(void* list);
bool list_remove(void* list, u32 idx);

#define list_DEFAULT_INITIAL_CAPACITY 16
#define list(t) (t*)list_create(list_DEFAULT_INITIAL_CAPACITY, sizeof(t));
#define list_header(list) ((ListHeader*)(list)-1)
#define list_length(list) list_header(list)->length
#define list_capacity(list) list_header(list)->capacity
#define list_append(list, v) (\
    list = list_ensure_capacity(list, 1),\
    list[list_header(list)->length++] = v)

#define list_push(list)(\
    list = list_ensure_capacity(list, 1),\
    &list[list_header(list)->length++])

#define list_popback(list)\
    &list[list_header(list)->length--]

#define list_peekback(list) &list[list_length(list)-1]

#define list_reset(list) list_header(list)->length = 0

#define list_foreach(list, type, name)\
for(int i = 0; i < list_length(list); i++)\
for(type* name = list + i;name != NULL; name = NULL)

#define list_copyto(source, destination)\
do {\
    for (int i = 0; i < list_length(source); i++) {\
        list_append(destination, source[i]);\
    }\
} while (0);\


#endif // LIST_H