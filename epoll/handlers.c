#include "router.h"

route_response_t handle_home(const char *path) {
  route_response_t response = {
      .status_code = 200,
      .status_text = "OK",
      .content_type = "text/html",
      .body = "<html><body><h1>Hello, World!</h1></body></html>"};
  return response;
}

route_response_t handle_about(const char *path) {
  route_response_t response = {
      .status_code = 200,
      .status_text = "OK",
      .content_type = "text/html",
      .body = "<html><body><h1>About Page</h1></body></html>"};
  return response;
}

route_response_t handle_api_status(const char *path) {
  route_response_t response = {.status_code = 200,
                               .status_text = "OK",
                               .content_type = "application/json",
                               .body = "{\"status\":\"ok\",\"uptime\":12345}"};
  return response;
}
