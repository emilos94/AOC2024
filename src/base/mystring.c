#include "mystring.h"

String string_copy_n(char* chars, u32 count) {
    String result = {
        .chars = malloc(count+1),
        .length = count
    };

    memcpy(result.chars, chars, count);
    result.chars[count] = '\0';
    return result;
}

String string_copy(String str) {
    return string_copy_n(str.chars, str.length);
}

String string_fmt_n(char* chars, u32 n, ...) {
    char* buf = malloc(n + 1);

    va_list args;
    va_start(args, chars);
    vsnprintf(buf, n, chars, args);
    va_end(args);

    buf[n] = '\0';

    return (String){.chars = buf, .length = n};
}

bool string_equals(String left, String right) {
    if (left.length != right.length) return false;
    if (left.chars == right.chars) return true; // same length and ptr, same

    for (int i = 0; i < left.length; i++) {
        if (left.chars[i] != right.chars[i]) {
            return false;
        }
    }

    return true;
}

// Does left start with right ? 
bool string_startswith(String left, String right) {
    if (left.length < right.length) return false;
    if (left.length == right.length && left.chars == right.chars) return true;

    for (int i = 0; i < right.length; i++) {
        if (left.chars[i] != right.chars[i]) {
            return false;
        }
    }

    return true;
}

bool string_chars_startswith(char* source, String literal) {
    for (int i = 0; i < literal.length; i++) {
        if (source[i] != literal.chars[i]) {
            return false;
        }
    }

    return true;
}

bool string_endswith(String left, String right) {
    if (left.length < right.length) return false;
    if (left.length == right.length && left.chars == right.chars) return true;

    u32 offset = left.length - right.length;
    u32 right_offset = 0;
    for (int i = offset; i < left.length; i++) {
        if (left.chars[i] != right.chars[right_offset++]) {
            return false;
        }
    }

    return true;
}

StringView* string_split(String str, char delimiter) {
    StringView* result = list(StringView);

    u32 marker = 0;
    for (int i = 0; i < str.length; i++) {
        if (str.chars[i] == delimiter) {
            StringView* element = list_push(result);
            element->chars = str.chars + marker;
            element->length = i - marker;
            marker = i + 1;
        }
    }

    if (marker != str.length) {
        StringView* element = list_push(result);
        element->chars = str.chars + marker;
        element->length = str.length - marker + 1;
    }

    return result;
}

StringView* string_split_str(String str, String delimiter) {
    StringView* result = list(StringView);

    u32 limit = str.length - delimiter.length;
    u32 marker = 0;
    for (int i = 0; i < str.length; i++) {
        if (i < limit && string_chars_startswith(str.chars + i, delimiter)) {
            StringView* element = list_push(result);
            element->chars = str.chars + marker;
            element->length = i - marker;
            marker = i + delimiter.length;
        }
    }

    if (marker != str.length) {
        StringView* element = list_push(result);
        element->chars = str.chars + marker;
        element->length = str.length - marker + 1;
    }

    return result;
}

void stringview_print(StringView string_view) {
    for (int i = 0; i < string_view.length; i++) {
        printf("%c", string_view.chars[i]);
    }
    printf("\n");
}

s32 string_toint(String s) {
    return atoi(s.chars);
}

u32 string_to_arrayf32(String s, f32* destination, char delimiter) {
    u32 marker = 0;
    u32 count = 0;
    for (int i = 0; i < s.length; i++) {
        if (s.chars[i] == delimiter || i == s.length-1) {
            destination[count++] = atof(s.chars + marker);
            marker = i+1;
        }
    }

    return count;
}

u32 string_to_arrayu32(String s, u32* destination, char delimiter) {
    u32 marker = 0;
    u32 count = 0;
    for (int i = 0; i < s.length; i++) {
        if (s.chars[i] == delimiter || i == s.length-1) {
            destination[count++] = (u32)atoi(s.chars + marker);
            marker = i+1;
        }
    }

    return count;
}

bool char_is_number(char c) {
    return c >= '0' && c <= '9';
}
