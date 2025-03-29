#ifndef LEXER_TYPES
#define LEXER_TYPES

#include <stddef.h>

typedef struct {
    char* buffer;
    char character;
    size_t buffer_size;
    size_t position;
} Lexer;

#endif
