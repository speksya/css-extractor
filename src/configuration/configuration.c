#include "configuration/configuration.h"

#include <stdlib.h>
#include <string.h>

#include "errors/errors.h"
#include "file/file.h"
#include "toml/toml.h"

#define CONFIGURATION_FILE "configuration.toml"
#define CLASS_KEY          "classnames"
#define CONFIGURATION_KEYS "keys"

static void configuration_free_keys(KeysArray* keys) {
    if (keys) {
        for (size_t i = 0; i < keys->length; i++) {
            free(keys->keys[i]);
        }
        free((void*)keys->keys);
        free((void*)keys);
    }
}

void configuration_free(Configuration* configuration) {
    if (configuration) {
        configuration_free_keys(configuration->classKeys);
        configuration_free_keys(configuration->cssModulesKeys);
        free(configuration);
    }
}

static Configuration* configuration_init() {
    Configuration* configuration = malloc(sizeof(Configuration));
    if (configuration == NULL) {
        throw_error(ALLOCATION_ERROR);
        exit(EXIT_FAILURE);
    }

    configuration->classKeys = NULL;
    configuration->cssModulesKeys = NULL;
    configuration->cssModulesEnabled = false;

    return configuration;
}

static KeysArray* configuration_keys_init(size_t length) {
    KeysArray* keys = malloc(sizeof(KeysArray));
    if (keys == NULL) {
        throw_error(ALLOCATION_ERROR);
        exit(EXIT_FAILURE);
    }

    keys->keys = (char**)malloc(sizeof(char*) * length);
    if (keys->keys == NULL) {
        free(keys);
        throw_error(ALLOCATION_ERROR);
        exit(EXIT_FAILURE);
    }

    keys->length = length;

    return keys;
}

static KeysArray* configuration_read_keys(const char* error,
    const toml_table_t* table,
    const char* key_error,
    const char* keys_error) {
    const toml_array_t* table_keys = toml_array_in(table, CONFIGURATION_KEYS);
    if (!table_keys) {
        throw_message_error(error, CONFIGURATION_KEYS);
        exit(EXIT_FAILURE);
    }

    const size_t length = toml_array_nelem(table_keys);
    if (length == 0) {
        throw_error(keys_error);
        exit(EXIT_FAILURE);
    }

    KeysArray* keys = configuration_keys_init(length);

    for (size_t i = 0; i < keys->length; i++) {
        toml_datum_t key = toml_string_at(table_keys, (int)i);
        if (!key.ok) {
            for (size_t j = 0; j < i; j++) {
                free(keys->keys[j]);
            }
            free((void*)keys->keys);
            free((void*)keys);
            throw_error(key_error);
            exit(EXIT_FAILURE);
        }

        keys->keys[i] = key.u.s;
    }

    return keys;
}

static void configuration_read_class_names(Configuration* configuration,
    const toml_table_t* toml_configuration) {
    const toml_table_t* classNames = toml_table_in(toml_configuration, CLASS_KEY);
    if (!classNames) {
        configuration_free(configuration);
        throw_message_error(CONFIGURATION_MISSING_ERROR, CLASS_KEY);
        exit(EXIT_FAILURE);
    }

    configuration->classKeys = configuration_read_keys(CLASSNAMES_MISSING_KEY_ERROR,
        classNames,
        CLASSNAMES_WRONG_KEY_ERROR,
        CLASSNAMES_MISSING_KEYS_ERROR);
}

static char* configuration_get_file_buffer(long file_size) {
    char* buffer = malloc(file_size);
    if (buffer == NULL) {
        throw_error(ALLOCATION_ERROR);
        exit(EXIT_FAILURE);
    }

    return buffer;
}

Configuration* configuration_read() {
    Configuration* configuration = configuration_init();
    if (!configuration) {
        exit(EXIT_FAILURE);
    }

    File* file = file_read(CONFIGURATION_FILE);
    char* buffer = configuration_get_file_buffer(file->size);
    toml_table_t* toml = toml_parse_file(file->file, buffer, (int)file->size);

    free(buffer);
    file_free(file);

    if (!toml) {
        configuration_free(configuration);
        throw_message_error(CONFIGURATION_READ_CONFIG_ERROR, CONFIGURATION_FILE);
        exit(EXIT_FAILURE);
    }

    configuration_read_class_names(configuration, toml);
    toml_free(toml);

    return configuration;
}
