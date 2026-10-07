#ifndef SSL_H
#define SSL_H

#include <openssl/ssl.h>
#include <stddef.h>

typedef struct {
  SSL_CTX *ctx;
  SSL *ssl;
  int fd;
} ssl_connection_t;

SSL_CTX *ssl_init(const char *cert_file, const char *key_file);
void ssl_cleanup(SSL_CTX *ctx);

ssl_connection_t *ssl_accept_connection(SSL_CTX *ctx, int client_fd);
void ssl_close_connection(ssl_connection_t *conn);

ssize_t ssl_read_or_plain(ssl_connection_t *conn, void *buffer, size_t size);
ssize_t ssl_write_or_plain(ssl_connection_t *conn, const void *buffer,
                           size_t size);

#endif
