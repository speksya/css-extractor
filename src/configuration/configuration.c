#include "configuration/configuration.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "errors/errors.h"
#include "file/file.h"
#include "toml/toml.h"

#define CONFIGURATION_FILE                    "configuration.toml"
#define CONFIGURATION_KEY                     "configuration"
#define CONFIGURATION_KEYS                    "keys"
#define CONFIGURATION_CSS_MODULES_ENABLED_KEY "cssModulesEnabled"
#define CLASS_KEY                             "classnames"
#define CSSMODULES                            "cssmodules"

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
        exit(ENOMEM);
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
        exit(ENOMEM);
    }

    keys->keys = (char**)malloc(sizeof(char*) * length);
    if (keys->keys == NULL) {
        free(keys);
        throw_error(ALLOCATION_ERROR);
        exit(ENOMEM);
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
        exit(ENOKEY);
    }

    const size_t length = toml_array_nelem(table_keys);
    if (length == 0) {
        throw_error(keys_error);
        exit(EINVAL);
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
            exit(EINVAL);
        }

        keys->keys[i] = key.u.s;
    }

    return keys;
}

static void configuration_read_css_modules(Configuration* configuration,
    const toml_table_t* toml_configuration) {
    const toml_table_t* css_modules = toml_table_in(toml_configuration, CSSMODULES);
    if (!css_modules && configuration->cssModulesEnabled) {
        configuration_free(configuration);
        throw_message_error(CONFIGURATION_MISSING_ERROR, CSSMODULES);
        exit(ENOKEY);
    }

    configuration->cssModulesKeys = configuration_read_keys(CSSMODULES_MISSING_KEY_ERROR,
        css_modules,
        CSSMODULES_WRONG_KEY_ERROR,
        CSSMODULES_MISSING_KEYS_ERROR);
}

static void configuration_read_class_names(Configuration* configuration,
    const toml_table_t* toml_configuration) {
    const toml_table_t* classNames = toml_table_in(toml_configuration, CLASS_KEY);
    if (!classNames) {
        configuration_free(configuration);
        throw_message_error(CONFIGURATION_MISSING_ERROR, CLASS_KEY);
        exit(ENOKEY);
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
        exit(ENOMEM);
    }

    return buffer;
}

Configuration* configuration_read() {
    Configuration* configuration = configuration_init();
    if (!configuration) {
        exit(ENOMEM);
    }

    File* file = file_read(CONFIGURATION_FILE);
    char* buffer = configuration_get_file_buffer(file->size);
    toml_table_t* toml = toml_parse_file(file->file, buffer, (int)file->size);

    free(buffer);
    file_free(file);

    if (!toml) {
        configuration_free(configuration);
        throw_message_error(CONFIGURATION_READ_CONFIG_ERROR, CONFIGURATION_FILE);
        exit(EIO);
    }

    const toml_table_t* toml_configuration = toml_table_in(toml, CONFIGURATION_KEY);
    if (!toml_configuration) {
        toml_free(toml);
        configuration_free(configuration);
        throw_message_error(CONFIGURATION_MISSING_ERROR, CONFIGURATION_KEY);
        exit(ENOKEY);
    }

    toml_datum_t toml_cssModulesEnabled =
        toml_bool_in(toml_configuration, CONFIGURATION_CSS_MODULES_ENABLED_KEY);
    if (!toml_cssModulesEnabled.ok) {
        toml_free(toml);
        configuration_free(configuration);
        throw_message_error(CONFIGURATION_MISSING_KEY_ERROR, CONFIGURATION_CSS_MODULES_ENABLED_KEY);
        exit(ENOKEY);
    }

    configuration->cssModulesEnabled = toml_cssModulesEnabled.u.b;

    configuration_read_class_names(configuration, toml);

    if (configuration->cssModulesEnabled) {
        configuration_read_css_modules(configuration, toml);
    }

    toml_free(toml);

    return configuration;
}
