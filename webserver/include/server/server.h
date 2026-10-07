#ifndef SERVER_H
#define SERVER_H

#include "types.h"

typedef int (*handler_fn)(request_t *req, response_t *res);

server_t* server_create(int port);
void server_destroy(server_t *srv);
int server_run(server_t *srv);
void server_register_handler(server_t *srv, const char *path, handler_fn handler);

#endif // SERVER_H
