#include "headers.h"
#include "http_internal.h"
#include "response.h"

#include <string.h>
#include <strings.h>

const char* http_request_get_header(request_t *req, const char *name) {
    if (!req) return NULL;

    struct request *r = (struct request*) req;

    for (int i = 0; i < r->header_count; i++) {
        if (strcasecmp(r->headers[i].name, name) == 0) {
            return r->headers[i].value;
        }
    }

    return NULL;
}

void http_response_set_header(response_t *res, const char *name, const char *value) {
    if (!res) return;

    struct response *r = (struct response*) res;

    if (r->header_count >= MAX_HEADERS) return;

    r->headers[r->header_count].name = strdup(name);
    r->headers[r->header_count].value = strdup(value);
    r->header_count++;
}

int http_request_header_count(request_t *req) {
    if (!req) return 0;

    struct request *r = (struct request*) req;
    
    return r->header_count;
}

int http_response_header_count(response_t *res) {
    if (!res) return 0;

    struct response *r = (struct response*) res;
    
    return r->header_count;
}
