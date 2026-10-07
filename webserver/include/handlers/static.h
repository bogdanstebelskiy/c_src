#ifndef HANDLERS_STATIC_H
#define HANDLERS_STATIC_H

#include "types.h"

int static_file_handler(request_t *req, response_t *res);
void static_handler_set_root(const char *root);

#endif // HANDLERS_STATIC_H
