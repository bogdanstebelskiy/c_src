#ifndef HTTP_PARSER_H
#define HTTP_PARSER_H

#include "types.h"

request_t* http_parse_request(const char *raw);
void http_request_free(request_t *req);
const char* http_request_method(request_t *req);
const char* http_request_path(request_t *req);
const char* http_request_version(request_t *req);

#endif // HTTP_PARSER_H
