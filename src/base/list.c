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

bool list_remove(void* list, u32 idx) {
    u32 length = list_length(list);
    if (idx >= length) {
        return false;
    }

    if (idx == length-1) {
        ListHeader* header = list_header(list);
        header->length--;
        return true;
    }

    u32 last_index = length-1;
    ListHeader* header = list_header(list);
    u32 elements_to_move = last_index - idx;
    memcpy(list + header->element_byte_size * idx, list + header->element_byte_size * (idx + 1), header->element_byte_size * elements_to_move);
    header->length--;
    return true;
}
