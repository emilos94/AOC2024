#include "map.h"

Map* map_create(u32 max_key_size, u32 value_size, u32 initial_capacity) {
    Map* map = malloc(sizeof(Map));
    if (map) {
        map->count = 0;
        map->max_key_size = max_key_size;
        map->value_size = value_size;
        map->capacity = initial_capacity;
        map->keys = malloc(initial_capacity * (max_key_size + sizeof(u32) + value_size + 1));
        memset(map->keys, 0, initial_capacity * (max_key_size + sizeof(u32) + value_size + 1));
        map->values = map->keys + initial_capacity * (max_key_size + sizeof(u32));
        map->occupied  = map->values + initial_capacity * value_size;
    }

    return map;
}

s32 _map_djb2_hash(char* to_hash, u32 length) {
    int result = 5381;

    for (int i = 0; i < length; i++) {
        result = ((result << 5) + result) + to_hash[i];
    }
    
    return result;
}

void _map_double_size(Map* map) {
    Map* new_map = map_create(map->max_key_size, map->value_size, map->capacity * 2);

    for (int i = 0; i < map->capacity; i++) {
        map_put(new_map, _map_key_at(map, i), _map_value_at(map, i));
    }

    void* old_items = map->keys;
    map->capacity = new_map->capacity;
    map->keys = new_map->keys;
    map->values = new_map->values;
    map->occupied = new_map->occupied;
    free(new_map);
    free(old_items);
}

void map_put(Map* map, String key, void* value) {
    assert(key.length <= map->max_key_size);
    
    // resize
    if (map->count >= map->capacity - 1) {
        _map_double_size(map);
    }

    s32 hash = _map_djb2_hash(key.chars, key.length);
    s32 index = hash % map->capacity;

    if (!map->occupied[index]) {
        map->count++;
    }

    while(map->occupied[index] && !string_equals(key, _map_key_at(map, index))) {
        index = (index + 1) % map->capacity;
    }

    memcpy(_map_key_chars_at(map, index), key.chars, key.length);
    memcpy(_map_key_length_at(map, index), &key.length, sizeof(u32));
    memcpy(_map_value_at(map, index), value, map->value_size);
    map->occupied[index] = true;
    map->count++;
}

void* map_get(Map* map, String key) {
    assert(key.length <= map->max_key_size);

    s32 hash = _map_djb2_hash(key.chars, key.length);
    s32 index = hash % map->capacity;

    if (!map->occupied[index]) {
        return NULL;
    }

    s32 search_counter = 0;
    while(map->occupied[index] && !string_equals(key, _map_key_at(map, index)) && search_counter < map->capacity) {
        index = (index + 1) % map->capacity;
        search_counter++;
    }

    return _map_value_at(map, index);
}

void map_free(Map* map) {
    free(map->keys);
    free(map);
}
