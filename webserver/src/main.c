#include "server.h"
#include "static.h"
#include "logger.h"

#include <stdio.h>
#include <signal.h>
#include <stdlib.h>

server_t *global_server = NULL;

void signal_handler(int sig) {
    (void) sig;
    log_info("Shutting down...");
    if (global_server) {
        server_destroy(global_server);
    }
    exit(0);
}

int test_handler_fn(request_t *req, response_t *res) {
    log_info("Test Handler Fn");
}

int main() {
    signal(SIGINT, signal_handler);

    server_t *srv = server_create(8080);
    if (!srv) {
        log_error("Failed to create server");
        return 1;
    }

    global_server = srv;

    static_handler_set_root("./www");
    server_register_handler(srv, "*", test_handler_fn);

    server_run(srv);

    server_destroy(srv);
    return 0;
}
