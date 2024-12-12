#ifndef MYSTRING_H
#define MYSTRING_H

#include "definitions.h"
#include "list.h"
#include "string.h"
#include "stdlib.h"
#include "stdarg.h"
#include "stdio.h"

typedef struct String {
    char* chars;
    u32 length;
} String;

typedef String StringView;

#define STR(x) (String){.chars = x, .length = strlen(x)}
#define TO_STR(x) ((String) {.chars = (char*)&x, .length = sizeof(x)})
#define STRING_VIEW(x, n) (String){.chars = x, .length = n}
#define STRING_EMPTY (String){.chars = 0, .length = 0}

String string_copy_n(char* chars, u32 count);
String string_copy(String str);
String string_fmt_n(char* chars, u32 n, ...);
bool string_equals(String left, String right);
bool string_contains_char(String str, char c);
bool string_startswith(String left, String right);
bool string_chars_startswith(char* chars, String prefix);
bool string_endswith(String left, String right);
StringView* string_split(String str, char delimiter);
StringView* string_split_str(String str, String delimiter);
void stringview_print(StringView string_view);
s32 string_toint(String s);
u32 string_to_arrayf32(String s, f32* destination, char delimiter);
u32 string_to_arrayu32(String s, u32* destination, char delimiter);
bool char_is_number(char c);

#endif // STRING_H