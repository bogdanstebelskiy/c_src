#include "parser.h"
#include "http_internal.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

request_t* http_parse_request(const char *raw) {
    request_t *req = calloc(1, sizeof(request_t));

    if (!req) return NULL;

    char *buffer = strdup(raw);
    char *line = strtok(buffer, "\r\n");

    if (line) {
        char *method = strtok(line, " ");
        char *path = strtok(NULL, " ");
        char *version = strtok(NULL, " ");

        req->method = method ? strdup(method) : strdup("GET");
        req->path = path ? strdup(path) : strdup("/");
        req->version = version ? strdup(version) : strdup("HTTP/1.1");
    }

    while ((line = strtok(NULL, "\r\n")) && *line) {
        char *colon = strchr(line, ':');

        if (colon && req->header_count < MAX_HEADERS) {
            *colon == '\0';
            char *name = line;
            char *value = colon + 1;

            while (*value == ' ') value++;

            req->headers[req->header_count].name = strdup(name);
            req->headers[req->header_count].value = strdup(value);
            req->header_count++;
        }
    }

    free(buffer);
    return req;
}

void http_request_free(request_t *req) {
    if (!req) return;
    free(req->method);
    free(req->path);
    free(req->version);
    for (int i = 0; i < req->header_count; i++) {
        free(req->headers[i].name);
        free(req->headers[i].value);
    }
    free(req->body);
    free(req);
}

const char* http_request_method(request_t *req) {
    return req ? req->method : NULL;
}

const char* http_request_path(request_t *req) {
    return req ? req->path : NULL;
}

const char* http_request_version(request_t *req) {
    return req ? req->version : NULL;
}
