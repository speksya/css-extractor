#include "token/token.h"

#include <errno.h>
#include <stdlib.h>

#include "errors/errors.h"

void token_free(Token* token) {
    if (token != NULL) {
        free(token->value);
        free(token);
    }
}

Token* token_str_init(TokenType type, char* value) {
    Token* token = malloc(sizeof(Token));
    if (token == NULL) {
        throw_error(ALLOCATION_ERROR);
        exit(ENOMEM);
    }

    token->type = type;
    token->value = value;

    return token;
}

Token* token_ch_init(TokenType type, const char value) {
    Token* token = malloc(sizeof(Token));
    if (token == NULL) {
        throw_error(ALLOCATION_ERROR);
        exit(ENOMEM);
    }

    char* buffer = malloc(sizeof(char) * 2);
    if (buffer == NULL) {
        throw_error(ALLOCATION_ERROR);
        free(token);
        exit(ENOMEM);
    }

    buffer[0] = value;
    buffer[1] = '\0';

    token->type = type;
    token->value = buffer;

    return token;
}
