#ifndef HANDLERS_H
#define HANDLERS_H

#include "router.h"

route_response_t handle_home(const char *path);
route_response_t handle_about(const char *path);
route_response_t handle_api_status(const char *path);

#endif
