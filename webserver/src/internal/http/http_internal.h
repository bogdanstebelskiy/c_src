#ifndef HTTP_INTERNAL_H
#define HTTP_INTERNAL_H

#include "types.h"

struct request {
    char *method;
    char *path;
    char *version;
    struct {
        char *name;
        char *value;
    } headers[MAX_HEADERS];
    int header_count;
    char *body;
    size_t body_length;
};

struct response {
    int status;
    char *status_text;
    struct {
        char *name;
        char *value;
    } headers[MAX_HEADERS];
    int header_count;
    char *body;
    size_t body_length;
};

#endif // HTTP_INTERNAL_H
