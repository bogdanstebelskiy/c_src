#include "server.h"
#include "server_internal.h"

#include "parser.h"
#include "response.h"
#include "headers.h"

#include "logger.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

server_t* server_create(int port) {
    server_t *srv = calloc(1, sizeof(server_t));
    if (!srv) return NULL;

    srv->port = port;
    srv->fd = socket(AF_INET, SOCK_STREAM, 0);

    if (srv->fd < 0) {
        log_error("Failed to create socket");
        free(srv);
        return NULL;
    }

    int opt = 1;
    setsockopt(srv->fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    struct sockaddr_in addr = {0};
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(port);

    if (bind(srv->fd, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        log_error("Failed to bind to port %d", port);
        close(srv->fd);
        free(srv);
        return NULL;
    }

    if (listen(srv->fd, 10) < 0) {
        log_error("Failed to listen on port %d", port);
        close(srv->fd);
        free(srv);
        return NULL;
    }

    return srv;
}

void server_destroy(server_t *srv) {
    if (!srv) return;
    close(srv->fd);
    for (int i = 0; i < srv->route_count; i++) {
        free(srv->routes[i].path);
    }
    free(srv);
}

void server_register_handler(server_t *srv, const char *path, handler_fn handler) {
    if (!srv || srv->route_count >= MAX_ROUTES) return;
    srv->routes[srv->route_count].path = strdup(path);
    srv->routes[srv->route_count].handler = handler;
    srv->route_count++;
}

static handler_fn find_handler(server_t *srv, const char *path) {
    for (int i = 0; i < srv->route_count; i++) {
        if (strcmp(srv->routes[i].path, path) == 0 ||
            strcmp(srv->routes[i].path, "*") == 0) {
            return srv->routes[i].handler;
        }
    }
    return NULL;
}

int server_run(server_t *srv) {
    log_info("Server listening on port %d", srv->port);    

    while (1) {
        struct sockaddr_in client_addr;
        socklen_t client_len = sizeof(client_addr);

        int client_fd = accept(srv->fd, (struct sockaddr*)&client_addr, &client_len);

        if (client_fd < 0) continue;

        char buffer[BUFFER_SIZE] = {0};
        ssize_t bytes = recv(client_fd, buffer, sizeof(buffer) - 1, 0);

        if (bytes > 0) {
            request_t *req = http_parse_request(buffer);
            response_t *res = http_response_create();

            handler_fn handler = find_handler(srv, http_request_path(req));
            if (!handler) handler = find_handler(srv, "*");

            if (handler) {
                handler(req, res);
            } else {
                http_response_set_status(res, 404, "Not Found");
                http_response_set_body(res, "404 Not Found", 13);
            }

            log_request(http_request_method(req), http_request_path(req), http_response_status(res));

            size_t res_len;
            char *res_data = http_response_build(res, &res_len);
            send(client_fd, res_data, res_len, 0);

            free(res_data);
            http_request_free(req);
            http_response_free(res);
        }

        close(client_fd);
    }

    return 0;
}
