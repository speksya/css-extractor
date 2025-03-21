#ifndef CONFIGURATION
#define CONFIGURATION

#include <stdbool.h>
#include <stddef.h>

typedef struct {
    char** keys;
    size_t length;
} KeysArray;

typedef struct {
    bool cssModulesEnabled;
    KeysArray* classKeys;
    KeysArray* cssModulesKeys;
} Configuration;

void configuration_free(Configuration* configuration);

Configuration* configuration_read();

#endif
