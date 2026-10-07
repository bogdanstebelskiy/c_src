#include "http_parser.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int parse_http_request(const char *buffer, http_request_t *request) {
  memset(request, 0, sizeof(http_request_t));

  if (sscanf(buffer, "%15s %255s %15s", request->method, request->path,
             request->version) != 3) {
    return -1;
  }

  const char *header_start = strstr(buffer, "\r\n");
  if (!header_start) {
    return 0;
  }
  header_start += 2;

  while (*header_start != '\r' && request->header_count < MAX_HEADERS) {
    const char *line_end = strstr(header_start, "\r\n");
    if (!line_end) {
      break;
    }

    const char *colon = strchr(header_start, ':');
    if (!colon || colon > line_end) {
      header_start = line_end + 2;
      continue;
    }

    size_t name_len = colon - header_start;
    request->headers[request->header_count].name =
        strndup(header_start, name_len);

    const char *value_start = colon + 1;
    while (*value_start == ' ')
      value_start++;

    size_t value_len = line_end - value_start;
    request->headers[request->header_count].value =
        strndup(value_start, value_len);

    request->header_count++;
    header_start = line_end + 2;
  }

  return 0;
}

const char *get_header(http_request_t *request, const char *name) {
  for (int i = 0; i < request->header_count; i++) {
    if (strcasecmp(request->headers[i].name, name) == 0) {
      return request->headers[i].value;
    }
  }
  return NULL;
}

void free_http_request(http_request_t *request) {
  for (int i = 0; i < request->header_count; i++) {
    free(request->headers[i].name);
    free(request->headers[i].value);
  }
}
