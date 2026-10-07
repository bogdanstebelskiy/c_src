#include "static.h"

#include "parser.h"
#include "response.h"
#include "headers.h"
#include "mime.h"

#include "fileio.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

static char root_dir[256] = "./www";

void static_handler_set_root(const char *root) {
    strncpy(root_dir, root, sizeof(root_dir) - 1);
}

int static_file_handler(request_t *req, response_t *res) {
    char filepath[512];
    const char *path = http_request_path(req);

    snprintf(filepath, sizeof(filepath), "%s%s", root_dir, path);

    if (filepath[strlen(filepath) - 1] == '/') {
        strcat(filepath, "index.html");
    }

    size_t size;
    char *content = read_file(filepath, &size);

    if (content) {
        http_response_set_header(res, "Content-Type", mime_type_from_path(filepath));
        http_response_set_body(res, content, size);
        free(content);
        return 0;
    }

    http_response_set_status(res, 404, "Not Found");
    const char *msg = "<h1>404 Not Found</h1>";
    http_response_set_body(res, msg, strlen(msg));
    http_response_set_header(res, "Content-Type", "text/html");
    return 0;
}
