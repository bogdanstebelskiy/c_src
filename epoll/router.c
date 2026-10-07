#include "router.h"
#include <stdio.h>
#include <string.h>

#define MAX_ROUTES 32

typedef struct {
  char path[256];
  route_handler_t handler;
} route_t;

static route_t routes[MAX_ROUTES];
static int route_count = 0;

void router_init() { route_count = 0; }

void router_add(const char *path, route_handler_t handler) {
  if (route_count < MAX_ROUTES) {
    strncpy(routes[route_count].path, path,
            sizeof(routes[route_count].path) - 1);
    routes[route_count].handler = handler;
    route_count++;
  }
}

route_response_t route_handle(const char *path) {
  for (int i = 0; i < route_count; ++i) {
    if (strcmp(routes[i].path, path) == 0) {
      return routes[i].handler(path);
    }
  }

  route_response_t response = {
      .status_code = 404,
      .status_text = "Not Found",
      .content_type = "text/html",
      .body = "<html><body><h1>404 Not Found</h1></body></html>"};

  return response;
}
