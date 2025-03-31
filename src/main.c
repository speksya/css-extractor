#include <stdlib.h>

#include "configuration/configuration.h"
#include "errors/errors.h"
#include "io/io.h"

int main(int argc, char** argv) {
    if (argc < 2) {
        throw_error(FILE_NOT_SPECIFIED_NAME_ERROR);
        exit(EXIT_FAILURE);
    }

    const char* filename = *++argv;
    char* buffer = io_read(filename);
    Configuration* configuration = configuration_read();

    configuration_free(configuration);
    free(buffer);

    return 0;
}
