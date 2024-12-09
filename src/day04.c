#include "base/file_handling.h"
#include "base/list.h"
#include "base/map.h"

Map* word_occurence_map(StringView* lines);
bool word_equals(void* left, void* right);
int count_xmas_samx(String* words);

typedef struct CrossMas {
    char left[3];
    char right[3];
} CrossMas;
CrossMas* find_cross_mas_words(StringView* lines);
bool is_cross_mas(CrossMas* cross_mas);

int main(void) {
    String file;
    if (!file_loadstr(&file, "../res/day04.txt")) {
        return 1;
    }

    StringView* lines = string_split_str(file, STR("\r\n"));

    Map* word_ocurences = word_occurence_map(lines);
    int* xmas_count = map_get(word_ocurences, STR("XMAS"));
    int* samx_count = map_get(word_ocurences, STR("SAMX"));

    printf("a: %d\n", *xmas_count + *samx_count);

    CrossMas* cross_mas_words = find_cross_mas_words(lines);

    s32 cross_mas_count = 0;
    list_foreach(cross_mas_words, CrossMas, cross_mas) {
        if (is_cross_mas(cross_mas)) {
            cross_mas_count++;
        }
    }

    printf("b: %d\n", cross_mas_count);
    
    map_free(word_ocurences);
    list_free(cross_mas_words);
    list_free(lines);
    free(file.chars);
}

CrossMas* find_cross_mas_words(StringView* lines) {
    CrossMas* words = list(CrossMas);

    int width = lines[0].length;
    int height = list_length(lines);

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            if (x < width-2 && y < height - 2) {
                CrossMas* cross_mas = list_push(words);
                cross_mas->left[0] = lines[y].chars[x];
                cross_mas->left[1] = lines[y+1].chars[x+1];
                cross_mas->left[2] = lines[y+2].chars[x+2];
                cross_mas->right[0] = lines[y].chars[x+2];
                cross_mas->right[1] = lines[y+1].chars[x+1];
                cross_mas->right[2] = lines[y+2].chars[x];
            }
        }
    }

    return words;
}

bool is_cross_mas(CrossMas* cross_mas) {
    bool left_mas = string_chars_startswith(cross_mas->left, STR("MAS")) || string_chars_startswith(cross_mas->left, STR("SAM"));
    bool right_mas = string_chars_startswith(cross_mas->right, STR("MAS")) || string_chars_startswith(cross_mas->right, STR("SAM"));
    return left_mas && right_mas;
}

void map_increment(Map* map, String key) {
    int* count = map_get(map, key);
    if (count) {
        int new_count = *count+1;
        map_put(map, key, &new_count);
    }
    else {
        int new_count = 1;
        map_put(map, key, &new_count);
    }
}

Map* word_occurence_map(StringView* lines) {
    Map* map = map(32, s32);
    char word[4];
    int width = lines[0].length;
    int height = list_length(lines);

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            if (x < width-3) {
                word[0] = lines[y].chars[x];
                word[1] = lines[y].chars[x + 1];
                word[2] = lines[y].chars[x + 2];
                word[3] = lines[y].chars[x + 3];
                map_increment(map, STRING_VIEW(&word[0], 4));
            }
            if (y < height - 3) {
                word[0] = lines[y].chars[x];
                word[1] = lines[y + 1].chars[x];
                word[2] = lines[y + 2].chars[x];
                word[3] = lines[y + 3].chars[x];
                map_increment(map, STRING_VIEW(&word[0], 4));
            }
            if (x >= 3 && y < height - 3) {
                word[0] = lines[y].chars[x];
                word[1] = lines[y + 1].chars[x - 1];
                word[2] = lines[y + 2].chars[x - 2];
                word[3] = lines[y + 3].chars[x - 3];
                map_increment(map, STRING_VIEW(&word[0], 4));
            }
            if (x < width - 3 && y < height - 3) {
                word[0] = lines[y].chars[x];
                word[1] = lines[y + 1].chars[x + 1];
                word[2] = lines[y + 2].chars[x + 2];
                word[3] = lines[y + 3].chars[x + 3];
                map_increment(map, STRING_VIEW(&word[0], 4));
            }
        }
    }

    return map;
}
