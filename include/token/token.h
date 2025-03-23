#ifndef TOKEN
#define TOKEN

typedef enum {
    TOKEN_IDENTIFIER,
    TOKEN_CLASSKEY,
    TOKEN_MODULE_KEY,
    
    TOKEN_ASSIGN,
    TOKEN_LBRACE,
    TOKEN_RBRACE,
    TOKEN_LBRACKET,
    TOKEN_RBRACKET,
    TOKEN_LPAREN,
    TOKEN_RPAREN,
    TOKEN_DOT,
    TOKEN_COMMA,
    TOKEN_COLON,
    TOKEN_SEMICOLON,
    TOKEN_QUOTE,
    TOKEN_DOUBLE_QUOTE,
    TOKEN_BACKTICK,
    TOKEN_DOLLAR,
    TOKEN_QUESTION,
    TOKEN_BANG,

    TOKEN_PLUS,
    TOKEN_MINUS,
    TOKEN_ASTERISK,
    TOKEN_SLASH,
    TOKEN_PERCENT,
    TOKEN_UNDERSCORE,

    TOKEN_LOGICAL_OR,
    TOKEN_LOGICAL_AND,
} TokenType;

typedef struct {
    TokenType type;
    char* value;
} Token;

Token* token_ch_init(TokenType type, char value);

Token* token_str_init(TokenType type, char* value);

void token_free(Token* token);

#endif
