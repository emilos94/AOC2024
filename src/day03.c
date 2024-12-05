#include "base/file_handling.h"
#include "base/list.h"

enum TokenType {
    TK_LPAR,
    TK_RPAR,
    TK_COMMA,
    TK_INT,
    TK_MUL,
    TK_DO,
    TK_DONT,
    TK_UNKNOWN
};
typedef u32 TokenType;

typedef struct Token {
    TokenType type;
    s32 value;
} Token;

Token* tokenize(String str);
int sum_multiplications(Token* tokens);
int sum_multiplications_do_dont(Token* tokens);

int main(void) {
    String file;
    if (!file_loadstr(&file, "../res/day03.txt")) {
        return 1;
    }

    Token* tokens = tokenize(file);
    int sum = sum_multiplications(tokens);
    printf("a: %d\n", sum);

    int sum2 = sum_multiplications_do_dont(tokens);
    printf("b: %d\n", sum2);

    list_free(tokens);
    free(file.chars);
}

Token* tokenize(String str) {
    Token* tokens = list(Token);

    for (int i = 0; i < str.length;) {
        if (i < str.length - 3 && string_chars_startswith(str.chars + i, STR("mul"))) {
            list_append(tokens, (Token){.type = TK_MUL});
            i += 3;
        }
        else if (i < str.length - 4 && string_chars_startswith(str.chars + i, STR("do()"))) {
            list_append(tokens, (Token){.type = TK_DO});
            i += 4;
        }
        else if (i < str.length - 7 && string_chars_startswith(str.chars + i, STR("don't()"))) {
            list_append(tokens, (Token){.type = TK_DONT});
            i += 7;
        }
        else if (char_is_number(str.chars[i])) {
            u32 marker = i;
            i++;
            while (char_is_number(str.chars[i])) {
                i++;
            }

            int value = atoi(str.chars + marker);
            list_append(tokens, ((Token){.type = TK_INT, .value = value}));
        }
        else {
            switch (str.chars[i])
            {
            case '(':
                list_append(tokens, (Token){.type = TK_LPAR});
                break;
            case ')':
                list_append(tokens, (Token){.type = TK_RPAR});
                break;
            case ',':
                list_append(tokens, (Token){.type = TK_COMMA});
                break;
            default:
                list_append(tokens, (Token){.type = TK_UNKNOWN});
                break;
            }

            i++;
        }
    }

    return tokens;
}


int sum_multiplications(Token* tokens) {
    int sum = 0;
    list_foreach(tokens, Token, token) {
        if (i < list_length(tokens) - 6) {
            if (
                tokens[i].type == TK_MUL &&
                tokens[i+1].type == TK_LPAR &&
                tokens[i+2].type == TK_INT &&
                tokens[i+3].type == TK_COMMA &&
                tokens[i+4].type == TK_INT &&
                tokens[i+5].type == TK_RPAR
            ) {
                sum += tokens[i+2].value * tokens[i+4].value;
            }
        }
    }

    return sum;
}

int sum_multiplications_do_dont(Token* tokens) {
    int sum = 0;
    bool enabled = true;
    list_foreach(tokens, Token, token) {
        if (token->type == TK_DO) {
            enabled = true;
        }
        else if (token->type == TK_DONT) {
            enabled = false;
        }
        else if (token->type == TK_MUL && i < list_length(tokens) - 6) {
            if (
                tokens[i].type == TK_MUL &&
                tokens[i+1].type == TK_LPAR &&
                tokens[i+2].type == TK_INT &&
                tokens[i+3].type == TK_COMMA &&
                tokens[i+4].type == TK_INT &&
                tokens[i+5].type == TK_RPAR &&
                enabled
            ) {
                sum += tokens[i+2].value * tokens[i+4].value;
            }
        }
    }

    return sum;
}