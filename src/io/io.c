#include "io/io.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "errors/errors.h"
#include "file/file.h"

char* io_read(const char* name) {
    File* file = file_read(name);

    char* buffer = malloc(file->size + 1);
    if (buffer == NULL) {
        file_free(file);
        throw_error(ALLOCATION_ERROR);
        exit(ENOMEM);
    }

    size_t bytes_read = fread(buffer, sizeof(char), file->size, file->file);
    if (bytes_read < (size_t)file->size) {
        file_free(file);
        throw_error(FILE_READ_ERROR);
        exit(EIO);
    }

    file_free(file);

    buffer[bytes_read] = '\0';

    return buffer;
}
