#ifndef STATIC_H
#define STATIC_H

#include "router.h"

#define STATIC_DIR "./public"

route_response_t serve_static_file(const char *path);

#endif
