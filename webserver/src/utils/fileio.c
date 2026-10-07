#include "fileio.h"
#include <stdio.h>
#include <stdlib.h>

char* read_file(const char *path, size_t *size) {
    FILE *f = fopen(path, "rb");
    if (!f) return NULL;

    fseek(f, 0, SEEK_END);
    *size = ftell(f);
    fseek(f, 0, SEEK_SET);

    char *content = malloc(*size);
    if (content) {
        fread(content, 1, *size, f);
    }

    fclose(f);
    return content;
}

int file_exists(const char *path) {
    FILE *f = fopen(path, "r");
    if (f) {
        fclose(f);
        return 1;
    }
    return 0;
}
