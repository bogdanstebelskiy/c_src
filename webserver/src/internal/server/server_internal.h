#ifndef SERVER_INTERNAL_H
#define SERVER_INTERNAL_H

#define MAX_ROUTES 64

typedef struct {
    char *path;
    handler_fn handler;
} route_t;

struct server {
    int fd;
    int port;
    route_t routes[MAX_ROUTES];
    int route_count;
};

#endif // SERVER_INTERNAL_H
