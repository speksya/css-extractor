#ifndef ERRORS
#define ERRORS

#define FILE_NOT_SPECIFIED_NAME_ERROR "Error: Please specify file name"
#define FILE_READ_ERROR               "Error: Failed to open the file"

#define ALLOCATION_ERROR   "Error: Failed to allocate memory"
#define REALLOCATION_ERROR "Error: Failed to reallocate memory"

#define UNTERMINATED_COMMENT_ERROR "Error: Unterminated comment"
#define NESTED_COMMENT_BLOCK_ERROR "Error: Nested block comment"

#define CONFIGURATION_READ_CONFIG_ERROR "Error: Cannot parse configuration file"
#define CONFIGURATION_MISSING_ERROR     "Error: Configuration missing"

#define CLASSNAMES_MISSING_KEY_ERROR  "Error: classnames missing key"
#define CLASSNAMES_MISSING_KEYS_ERROR "Error: please specify classnames keys"
#define CLASSNAMES_WRONG_KEY_ERROR    "Error: classnames incorrect key"

void throw_error(const char* error);

void throw_message_error(const char* error, const char* message);

void throw_unexpected_character_error(char character);

#endif
