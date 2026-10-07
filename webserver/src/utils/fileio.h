#ifndef UTILS_FILEIO_H
#define UTILS_FILEIO_H

#include <stddef.h>

char* read_file(const char *path, size_t *size);

int file_exists(const char *path);

#endif // UTILS_FILEIO_H
