#include "base/file_handling.h"
#include "base/list.h"
#include "base/map.h"

Map* parse_rules(StringView* lines);
typedef struct RuleList {
    s32* list;
} RuleList;
s32** parse_updates(StringView* lines);
s32 sum_middle_numbers_of_valid_updates(s32** updates, Map* rules_map);
void order_update_correctly(s32* update, s32* destination, Map* rules_map);
s32 sum_middle_numbers_of_invalid_reordered_updates(s32** updates, Map* rules_map);
bool is_update_valid(Map* rules_map, s32* update_list);

int main(void) {
    String file;
    if (!file_loadstr(&file, "../res/day05.txt")) {
        return 1;
    }
    StringView* lines = string_split_str(file, STR("\r\n"));
    Map* rules_map = parse_rules(lines);
    s32** updates = parse_updates(lines);
    s32 result = sum_middle_numbers_of_valid_updates(updates, rules_map);
    printf("a: %d\n", result);

    s32 result_b = sum_middle_numbers_of_invalid_reordered_updates(updates, rules_map);
    printf("b: %d\n", result_b);

    map_foreach_key(rules_map, key) {
        RuleList* rule_list = map_get(rules_map, key);
        if (rule_list->list) {
            list_free(rule_list->list);
        }
    }

    for(int i = 0; i < list_length(updates); i++) {
        s32* update_list = updates[i];
        list_free(update_list);
    }
    list_free(updates);

    map_free(rules_map);
    list_free(lines);
    free(file.chars);
}


Map* parse_rules(StringView* lines) {
    Map* rules_map = map(32, RuleList);

    list_foreach(lines, String, line) {
        if (string_contains_char(*line, '|')) {
            StringView* split = string_split(*line, '|');
            assert(list_length(split) == 2);

            s32 left = atoi(split[0].chars);
            s32 right = atoi(split[1].chars);

            RuleList* rule_list = map_get(rules_map, TO_STR(left));
            if (rule_list) {
                list_append(rule_list->list, right);
            }
            else {
                RuleList new_list;
                new_list.list = list(s32);
                list_append(new_list.list, right);
                map_put(rules_map, TO_STR(left), &new_list);
            }
            list_free(split);
        }
    }

    return rules_map;
}

s32** parse_updates(StringView* lines) {
    s32** update_lists = list(s32*);
    list_foreach(lines, StringView, line) {
        if (string_contains_char(*line, ',')) {
            s32* update_list = list(s32);
            StringView* split = string_split(*line, ',');
            for (int j = 0; j < list_length(split); j++) {
                s32 value = atoi(split[j].chars);
                list_append(update_list, value);
            }

            list_free(split);
            list_append(update_lists, update_list);
        }
    }

    return update_lists;
}

bool is_update_valid(Map* rules_map, s32* update_list) {
    for (int j = 0; j < list_length(update_list); j++) {
        RuleList* rules = map_get(rules_map, TO_STR(update_list[j]));
        if (j > 0 && rules) {
            for (int k = 0; k < j; k++) {
                if (list_contains(rules->list, update_list[k])) {
                    return false;
                }
            }
        }
    }

    return true;
}

s32 sum_middle_numbers_of_valid_updates(s32** updates, Map* rules_map) {
    s32* middle_number_list = list(s32);
    for (int i = 0; i < list_length(updates); i++) {
        s32* update_list = updates[i];
        if (is_update_valid(rules_map, update_list)) {
            s32 middle_number = update_list[(list_length(update_list)-1)/2];
            list_append(middle_number_list, middle_number);
        }
    }

    s32 result = 0;
    list_foreach(middle_number_list, s32, value) {
        result += *value;
    }
    
    list_free(middle_number_list);

    return result;
}

s32 sum_middle_numbers_of_invalid_reordered_updates(s32** updates, Map* rules_map) {
    s32* middle_number_list = list(s32);
    s32* temp_ordered_list = list(s32);
    for (int i = 0; i < list_length(updates); i++) {
        s32* update_list = updates[i];
        if (!is_update_valid(rules_map, update_list)) {
            order_update_correctly(update_list, temp_ordered_list, rules_map);
            s32 middle_number = temp_ordered_list[(list_length(temp_ordered_list)-1)/2];
            list_append(middle_number_list, middle_number);
            list_reset(temp_ordered_list);
        }
    }

    s32 result = 0;
    list_foreach(middle_number_list, s32, value) {
        result += *value;
    }
    
    list_free(middle_number_list);
    list_free(temp_ordered_list);

    return result;
}

void order_update_correctly(s32* update, s32* destination, Map* rules_map) {
    bool done = false;
    s32* temp_list = list(s32);
    s32* update_copy = list(s32);
    list_copyto(update, update_copy);
    s32 i = 0;

    while(!done) {
        list_copyto(update_copy, temp_list);
        list_remove(temp_list, i);
        s32 numberToCheck = update_copy[i];

        bool isValidToPlace = true;
        for (int j = 0; j < list_length(temp_list); j++) {
            RuleList* rule = map_get(rules_map, TO_STR(temp_list[j]));
            bool blockedByRule = rule && list_contains(rule->list, numberToCheck);
            if (blockedByRule) {
                isValidToPlace = false;
            }
        }

        if (isValidToPlace) {
            list_append(destination, numberToCheck);
            list_remove(update_copy, i);
        }
        done = list_length(update_copy) == 0;
        if (!done) {
            i = (i + 1) % list_length(update_copy);
            list_reset(temp_list);
        }
    }

    list_free(temp_list);
    list_free(update_copy);
}