#include "base/file_handling.h"
#include "stdlib.h"

int compare(const void* a, const void* b)  {
    int int_a = *(int*)a;
    int int_b = *(int*)b;
    if (int_a < int_b) {
        return 1;
    }
    else if (int_a > int_b) {
        return -1;
    }

    return 0;
}

typedef struct ListResult {
    int* left;
    int* right;
} ListResult;

ListResult parse_lists(String file) {
    StringView* lines = string_split(file, '\n');
    int* left = list(int);
    int* right = list(int);
    list_foreach(lines, StringView, line) {
        StringView* line_split = string_split_str((String)*line, STR("   "));
        assert(list_length(line_split) == 2);
        list_append(left, atoi(line_split[0].chars));
        list_append(right, atoi(line_split[1].chars));
        list_free(line_split);
    }
    list_free(lines);

    return (ListResult) {
        .left = left,
        .right = right
    };
}

void solve_a(int* left, int* right) {
    int sum = 0;
    for (int i = 0; i < list_length(left); i++) {
        sum += abs(left[i] - right[i]);
    }

    printf("a: %d\n", sum);
}

int count_occurence(int value, int* list) {
    int count = 0;
    for (int i = 0; i < list_length(list); i++) {
        if (list[i] == value) {
            count++;
        }
        else if (list[i] < value) {
            return count;
        }
    }

    return count;
}

void solve_b(int* left, int* right) {
    int sum = 0;
    for (int i = 0; i < list_length(left); i++) {
        int occurence = count_occurence(left[i], right);
        sum += left[i] * occurence;
    }

    printf("b: %d\n", sum);
}

int main(void) {
    String file;
    if (!file_loadstr(&file, "../res/day01.txt")) {
        return 1;
    }

    ListResult lists = parse_lists(file);
    int* left = lists.left;
    int* right = lists.right;

    assert(list_length(left) == list_length(right));

    qsort(left, list_length(left), sizeof(int), compare);
    qsort(right, list_length(right), sizeof(int), compare);

    solve_a(left, right);
    solve_b(left, right);

    free(file.chars);
    list_free(left);
    list_free(right);
}
