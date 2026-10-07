#ifndef HTTP_HEADERS_H
#define HTTP_HEADERS_H

#include "types.h"

const char* http_request_get_header(request_t *req, const char *name);
void http_response_set_header(response_t *res, const char *name, const char *value);
int http_request_header_count(request_t *req);
int http_response_header_count(response_t *res);

#endif // HTTP_HEADERS_H
