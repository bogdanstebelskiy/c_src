#ifndef ROUTER_H
#define ROUTER_H

typedef struct {
  int status_code;
  const char *status_text;
  const char *content_type;
  const char *body;
} route_response_t;

typedef route_response_t (*route_handler_t)(const char *path);

void router_init();
void router_add(const char *path, route_handler_t handler);
route_response_t route_handle(const char *path);

#endif
