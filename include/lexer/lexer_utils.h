#ifndef LEXER_UTILS
#define LEXER_UTILS

#include "lexer_types.h"

#define POS(lexer)       ((lexer)->position)
#define SIZE(lexer)      ((lexer)->buffer_size)
#define PEEK(lexer)      ((lexer)->character)
#define PEEK_NEXT(lexer) ((lexer)->buffer[POS(lexer) + 1])
#define PEEK_PREV(lexer) ((lexer)->buffer[POS(lexer) - 1])

#define IS_NULL_TERMINATOR(lexer)     (PEEK(lexer) == '\0')
#define IS_LINE_COMMENT(lexer)        (PEEK(lexer) == '/' && PEEK_NEXT(lexer) == '/')
#define IS_BLOCK_COMMENT_START(lexer) (PEEK(lexer) == '/' && PEEK_NEXT(lexer) == '*')
#define IS_BLOCK_COMMENT_END(lexer)   (PEEK(lexer) == '*' && PEEK_NEXT(lexer) == '/')

#define ADVANCE(lexer)                                                                             \
    (++(lexer)->position, (lexer)->character = (lexer)->buffer[(lexer)->position])
#define RETREAT(lexer)                                                                             \
    (--(lexer)->position, (lexer)->character = (lexer)->buffer[(lexer)->position])

void lexer_utils_advance(Lexer* lexer);

void lexer_utils_multiadvance(Lexer* lexer, unsigned int steps);

void lexer_utils_skip_whitespace(Lexer* lexer);

#endif
