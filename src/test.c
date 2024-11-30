
#include "base/file_handling.h"

int main(void) {
    String file;
    if (!file_loadstr(&file, "../res/test.txt")) {
        printf("Failed to load file!\n");
        return 1;
    }

    StringView* split = string_split(file, '\n');
    assert(list_length(split) == 4);

    list_foreach(split, StringView, line) {
        for (int j = 0; j < line->length; j++)
            printf("%c", line->chars[j]);
        printf("\n");
    }
    
    return 0;
}
