#ifndef LEXER
#define LEXER

#include <stddef.h>

#include "lexer_types.h"
#include "token/token.h"

Lexer* lexer_init(char* buffer);

Token* lexer_tokenize(Lexer* lexer);

void lexer_free(Lexer* lexer);

#endif
