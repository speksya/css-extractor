#include "lexer/lexer_utils.h"

#include <ctype.h>

void lexer_utils_advance(Lexer* lexer) {
    if (POS(lexer) < SIZE(lexer) && !IS_NULL_TERMINATOR(lexer)) {
        ADVANCE(lexer);
    }
}

void lexer_utils_multiadvance(Lexer* lexer, unsigned int steps) {
    for (unsigned int i = 0; i < steps; i++) {
        lexer_utils_advance(lexer);
    }
}

void lexer_utils_skip_whitespace(Lexer* lexer) {
    while (isspace(PEEK(lexer))) {
        lexer_utils_advance(lexer);
    }
}
