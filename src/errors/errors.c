#include "errors/errors.h"

#include <stdio.h>

void throw_error(const char* error) {
    printf("%s\n", error);
}

void throw_message_error(const char* error, const char* message) {
    printf("%s: %s\n", error, message);
}
