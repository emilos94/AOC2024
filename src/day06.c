#include "base/file_handling.h"
#include "base/definitions.h"

// p1
typedef struct v2 {
    u32 x, y;
} v2;

typedef struct Guard {
    v2 position;
    char direction;
} Guard;

Guard identify_position(StringView* lines);
v2 next_position(Guard guard);
bool is_in_map(StringView* lines, v2 position);
bool should_rotate(StringView* lines, Guard guard);
void rotate(Guard* guard);
char char_at(StringView* lines, v2 position);
u32 count_visited_positions(StringView* lines, u8* visited, u32 square_count);

// p2
u32 count_obstructions_for_loops(StringView* lines, bool* visited);

int main(void) {
    String file;
    if (!file_loadstr(&file, "../res/day06.txt")) {
        return 1;
    }

    StringView* lines = string_split_str(file, STR("\r\n"));

    u32 square_count = list_length(lines) * lines[0].length;
    u8* visited = malloc(square_count * sizeof(u8));

    u32 visited_count = count_visited_positions(lines, visited, square_count);
    printf("a: %d\n", visited_count);

    u32 possible_loops = count_obstructions_for_loops(lines, visited);
    printf("b: %d\n", possible_loops);

    free(visited);
    list_free(lines);
    free(file.chars);
    return 0;
}

Guard identify_position(StringView* lines) {
    for (int y = 0; y < list_length(lines); y++)
    for (int x = 0; x < lines[0].length; x++) {
        char c = lines[y].chars[x];
        if (c == 'v' || c == '<' || c == '^' || c == '>') {
            return (Guard) {
                .position = (v2){.x = x, .y = y},
                .direction = c
            };
        }
    }

    assert(false);
}

v2 next_position(Guard guard) {
    v2 result = guard.position;

    switch (guard.direction)
    {
    case '>':
        result.x += 1;
        break;
    case '<':
        result.x -= 1;
        break;
    case '^':
        result.y -= 1;
        break;
    case 'v':
        result.y += 1;
        break;
    }

    return result;
}

bool is_in_map(StringView* lines, v2 position) {
    u32 height = list_length(lines);
    u32 width = lines[0].length;

    bool result = position.x >= 0 && position.x < width && 
                  position.y >= 0 && position.y < height;
    return result;
}

bool should_rotate(StringView* lines, Guard guard) {
    v2 next = next_position(guard);
    if (!is_in_map(lines, next)) {
        return false;
    }

    return char_at(lines, next) == '#';
}

void rotate(Guard* guard) {
    switch (guard->direction)
    {
    case '>':
        guard->direction = 'v';
        break;
    case '<':
        guard->direction = '^';
        break;
    case '^':
        guard->direction = '>';
        break;
    case 'v':
        guard->direction = '<';
        break;
    }
}

char char_at(StringView* lines, v2 position) {
    return lines[position.y].chars[position.x];
}

u32 count_visited_positions(StringView* lines, u8* visited, u32 square_count) {
    memset(visited, 0, square_count * sizeof(u8));
    Guard guard = identify_position(lines);
    visited[guard.position.x + guard.position.y * list_length(lines)] = true;
    bool running = true;
    u32 running_visited_count = 0;
    while(running) {
        while(should_rotate(lines, guard)) {
            rotate(&guard);
        }

        v2 next = next_position(guard);
        if (is_in_map(lines, next)) {
            guard.position = next;
            visited[next.x + next.y * list_length(lines)] += 1;
            running_visited_count++;
        }
        else {
            running = false;
        }

        if (running_visited_count > square_count) {
            return running_visited_count;
        }
    }

    u32 visited_count = 0;
    for (int i = 0; i < square_count; i++) {
        if (visited[i] > 0) {
            visited_count++;
        }
    }
    return visited_count;
}

u32 count_obstructions_for_loops(StringView* lines, u8* visited) {
    u32 square_count = list_length(lines) * lines[0].length;

    Guard guard = identify_position(lines);
    bool running = true;
    u32 possible_loop_count = 0;

    for (int y = 0; y < list_length(lines); y++)
    for (int x = 0; x < lines[0].length; x++) {
        if (x == guard.position.x && y == guard.position.y || char_at(lines, (v2){.x = x, .y = y}) == '#') {
            continue;
        }

        char old_char = lines[y].chars[x];
        lines[y].chars[x] = '#';
        u32 visited_count = count_visited_positions(lines, visited, square_count);
        if (visited_count > square_count) {
            possible_loop_count++;
        }
        lines[y].chars[x] = old_char;
    }
    
    return possible_loop_count;
}