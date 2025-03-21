#ifndef FILE_H
#define FILE_H

#include <stdio.h>

typedef struct {
    FILE* file;
    long size;
} File;

void file_free(File* file);

File* file_read(const char* name);

#endif
