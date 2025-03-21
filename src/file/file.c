#include "file/file.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

#include "errors/errors.h"

void file_free(File* file) {
    if (file != NULL) {
        (void)fclose(file->file);
        free(file);
    }
}

File* file_read(const char* name) {
    File* file_struct = malloc(sizeof(File));
    if (file_struct == NULL) {
        throw_error(ALLOCATION_ERROR);
        exit(ENOMEM);
    }

    FILE* file = fopen(name, "rb");
    if (file == NULL) {
        free(file_struct);
        throw_error(FILE_READ_ERROR);
        exit(EIO);
    }

    if (fseek(file, 0, SEEK_END) != 0) {
        free(file_struct);
        (void)fclose(file);
        throw_error(FILE_READ_ERROR);
        exit(EIO);
    }

    long file_size = ftell(file);
    if (file_size == -1) {
        free(file_struct);
        (void)fclose(file);
        throw_error(FILE_READ_ERROR);
        exit(EIO);
    }

    if (fseek(file, 0, SEEK_SET) != 0) {
        free(file_struct);
        (void)fclose(file);
        throw_error(FILE_READ_ERROR);
        exit(EIO);
    }

    file_struct->file = file;
    file_struct->size = file_size;

    return file_struct;
}
