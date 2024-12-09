#ifndef MAP_H
#define MAP_H

#include "definitions.h"
#include "mystring.h"
#include "string.h"
#include "stdlib.h"

typedef struct Map {
    u32 count;
    u32 max_key_size;
    u32 value_size;
    u32 capacity;
    char* keys;
    void* values;
    bool* occupied;
} Map;

Map* map_create(u32 max_key_size, u32 value_size, u32 initial_capacity);
void map_put(Map* map, String key, void* value);
void* map_get(Map* map, String key);
void map_free(Map* map);
/*
#define MAP_DEFINE(value_type)\
typedef struct Map##value_type##KV {\
    String key;\
    value_type value;\
    bool occupied;\
} Map##value_type##KV;\
typedef struct Map##value_type{\
    Map##value_type##KV* items;\
    u32 capacity;\
    u32 count;\
} Map##value_type;

s32 _map_djb2_hash(char* to_hash, u32 length);

#define map_find_index(map, key)\
({\
    s32 index = _map_djb2_hash(key, key->length) % map->capacity;\
    u32 counter = 0;\
    while(\
        map->items[index].occupied &&\
        !string_equals(key, map->items[index].key) &&\
        counter < map->capacity) {\
            index = (index + 1) % map->capacity;\
            counter++;\
        };\
    if (counter >= map->capacity || !map->items[index].occupied) index = -1;\
    index;\
})

#define map_put(map, key, value)\
do {\
    s32 index = map_find_index(map, key);\
    if (index >= 0) {\
        if (!map->items[index].occupied) {\
            map->items[index].occupied = true;\
            memcpy(map->items)
        }
        map->items[index].value = value;
        map->items[index].key = key;
    }
} while (0);


#define map_find(map, key)\
({\
    void* result = NULL;\
    s32 index = map_find_index(map, key);\
    if (index >= 0) result = &map->items[index].value;\
    result;\
});
*/
#define _map_key_chars_at(map, index) (map->keys + (map->max_key_size + sizeof(u32)) * index)
#define _map_key_length_at(map, index) (_map_key_chars_at(map, index) + map->max_key_size)
#define _map_key_at(map, index)\
((String){\
    .chars = _map_key_chars_at(map, index),\
    .length = *_map_key_length_at(map, index)\
})
#define _map_value_at(map, index) (map->values + (map->value_size * index))
#define MAP_DEFAULT_INITIAL_CAPACITY 64
#define map(key_max_length, value_type) map_create(key_max_length, sizeof(value_type), MAP_DEFAULT_INITIAL_CAPACITY)
#define map_foreach_key(map, key_name)\
    for(int i = 0; i < map->capacity; i++)\
    for(bool occupied = map->occupied[i]; occupied;)\
    for(String key_name = _map_key_at(map, i); occupied; occupied = false)


#endif