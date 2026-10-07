#include "http_response.h"
#include "ssl.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>

void send_http_response(ssl_connection_t *conn, int status_code,
                        const char *status_text, const char *content_type,
                        const char *body) {
  char response[4096];
  int len =
      snprintf(response, sizeof(response),
               "HTTP/1.1 %d %s\r\n"
               "Content-Type: %s\r\n"
               "Content-Length: %zu\r\n"
               "Connection: close\r\n"
               "\r\n"
               "%s",
               status_code, status_text, content_type, strlen(body), body);

  ssl_write_or_plain(conn, response, len);
}
