#include "lexer/lexer.h"

#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "errors/errors.h"
#include "lexer/lexer_utils.h"
#include "token/token.h"

Lexer* lexer_init(char* buffer) {
    Lexer* lexer = malloc(sizeof(Lexer));
    if (lexer == NULL) {
        throw_error(ALLOCATION_ERROR);
        exit(ENOMEM);
    }

    lexer->buffer = buffer;
    lexer->character = buffer[0];
    lexer->buffer_size = strlen(buffer);
    lexer->position = 0;

    return lexer;
}

void lexer_free(Lexer* lexer) {
    if (lexer != NULL) {
        free(lexer->buffer);
        free(lexer);
    }
}
