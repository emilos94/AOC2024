#include "list.h"

void* list_create(u32 initial_capacity, u32 element_byte_size) {
    void* result = NULL;
    ListHeader* header = malloc(initial_capacity * element_byte_size + sizeof(ListHeader));
    if (header) {
        header->capacity = initial_capacity;
        header->length = 0;
        header->element_byte_size = element_byte_size;
        result = header+1;
        memset(result, 0, initial_capacity * element_byte_size);
    }

    return result;
}

void* list_ensure_capacity(void* array, u32 capacity) {
    ListHeader* header = list_header(array);
    u32 remaining_capacity = header->capacity - header->length;
    if (remaining_capacity < header->length + capacity) {
        u32 new_capacity = header->capacity * 2;
        while (new_capacity < header->length + capacity) {
            new_capacity *= 2;
        }

        header = realloc(header, new_capacity * header->element_byte_size + sizeof(ListHeader));
        header->capacity = new_capacity;
    }
    return header+1;
}

void list_free(void* arr) {
    ListHeader* header = list_header(arr);
    free(header);
}
