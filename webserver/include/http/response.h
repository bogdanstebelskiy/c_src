#ifndef HTTP_RESPONSE_H
#define HTTP_RESPONSE_H

#include "types.h"

response_t* http_response_create(void);
void http_response_free(response_t *res);
void http_response_set_status(response_t *res, int status, const char *text);
void http_response_set_body(response_t *res, const char *body, size_t length);
char* http_response_build(response_t *res, size_t *out_len);
int http_response_status(response_t *res);

#endif // HTTP_RESPONSE_H
