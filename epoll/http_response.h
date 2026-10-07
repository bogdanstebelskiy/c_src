#ifndef HTTP_RESPONSE_H
#define HTTP_RESPONSE_H

#include "ssl.h"
#include <stddef.h>

void send_http_response(ssl_connection_t *conn, int status_code,
                        const char *status_text, const char *content_type,
                        const char *body);

#endif
