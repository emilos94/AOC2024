#include "base/file_handling.h"

int main(void) {
    String file;
    if (!file_loadstr(&file, "../res/dayxx-test.txt")) {
        return 1;
    }

    printf(file.chars);

    free(file.chars);
    return 0;
}

