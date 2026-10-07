#ifndef HTTP_PARSER_H
#define HTTP_PARSER_H

#define MAX_HEADERS 32

typedef struct {
  char *name;
  char *value;
} http_header_t;

typedef struct {
  char method[16];
  char path[256];
  char version[16];
  http_header_t headers[MAX_HEADERS];
  int header_count;
} http_request_t;

int parse_http_request(const char *buffer, http_request_t *request);
const char *get_header(http_request_t *request, const char *name);
void free_http_request(http_request_t *request);

#endif
