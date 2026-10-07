#include "response.h"
#include "http_internal.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

response_t* http_response_create(void) {
    response_t *res = calloc(1, sizeof(struct response));

    if (res) {
        struct response *r = (struct response*) res;
        r->status = 200;
        r->status_text = strdup("OK");
    }

    return res;
}

void http_response_free(response_t *res) {
    if (!res) return;

    struct response *r = (struct response*) res;

    free(r->status_text);

    for (int i = 0; i < r->header_count; i++) {
        free(r->headers[i].name);
        free(r->headers[i].value);
    }

    free(r->body);
    free(res);
}

void http_response_set_status(response_t *res, int status, const char *text) {
    if (!res) return;

    struct response *r = (struct response*) res;

    r->status = status;
    free(r->status_text);
    r->status_text = strdup(text);
}

void http_response_set_body(response_t *res, const char *body, size_t length) {
    if (!res) return;

    struct response *r = (struct response*) res;

    free(r->body);
    r->body = malloc(length);
    if (r->body) {
        memcpy(r->body, body, length);
        r->body_length = length;
    }
}

char* http_response_build(response_t *res, size_t *out_len) {
    if (!res) return NULL;
    struct response *r = (struct response*) res;

    char *buffer = malloc(BUFFER_SIZE);
    int offset = 0;

    offset += snprintf(buffer + offset, BUFFER_SIZE - offset, "HTTP/1.1 %d %s\r\n", r->status, r->status_text);

    for (int i = 0; i < r->header_count; i++) {
        offset += snprintf(buffer + offset, BUFFER_SIZE - offset, "%s: %s\r\n", r->headers[i].name, r->headers[i].value);
    }

    if (r->body) {
        offset += snprintf(buffer + offset, BUFFER_SIZE - offset, "Content-Length: %zu\r\n", r->body_length);
    }

    offset += snprintf(buffer + offset, BUFFER_SIZE - offset, "\r\n");

    if (r->body) {
        buffer = realloc(buffer, offset + r->body_length);
        memcpy(buffer + offset, r->body, r->body_length);
        offset += r->body_length;
    }

    *out_len = offset;
    return buffer;
}

int http_response_status(response_t *res) {
    if (!res) return 0;

    struct response *r = (struct response*) res;

    return r->status;
}
