#include "ssl.h"
#include <openssl/err.h>
#include <openssl/ssl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

SSL_CTX *ssl_ctx = NULL;

SSL_CTX *ssl_init(const char *cert_file, const char *key_file) {
  SSL_load_error_strings();
  OpenSSL_add_ssl_algorithms();

  const SSL_METHOD *method = TLS_server_method();
  SSL_CTX *ctx = SSL_CTX_new(method);

  if (!ctx) {
    ERR_print_errors_fp(stderr);
    return NULL;
  }

  if (SSL_CTX_use_certificate_file(ctx, cert_file, SSL_FILETYPE_PEM) <= 0) {
    ERR_print_errors_fp(stderr);
    SSL_CTX_free(ctx);
    return NULL;
  }

  if (SSL_CTX_use_PrivateKey_file(ctx, key_file, SSL_FILETYPE_PEM) <= 0) {
    ERR_print_errors_fp(stderr);
    SSL_CTX_free(ctx);
    return NULL;
  }

  return ctx;
}

void ssl_cleanup(SSL_CTX *ctx) {
  if (ctx) {
    SSL_CTX_free(ctx);
  }
  EVP_cleanup();
}

ssl_connection_t *ssl_accept_connection(SSL_CTX *ctx, int client_fd) {
  ssl_connection_t *conn = malloc(sizeof(ssl_connection_t));
  if (!conn) {
    return NULL;
  }

  conn->ctx = ctx;
  conn->fd = client_fd;
  conn->ssl = NULL;

  if (ctx) {
    conn->ssl = SSL_new(ctx);
    if (!conn->ssl) {
      free(conn);
      return NULL;
    }

    SSL_set_fd(conn->ssl, client_fd);

    if (SSL_accept(conn->ssl) <= 0) {
      ERR_print_errors_fp(stderr);
      SSL_free(conn->ssl);
      free(conn);
      return NULL;
    }
  }

  return conn;
}

void ssl_close_connection(ssl_connection_t *conn) {
  if (!conn) {
    return;
  }

  if (conn->ssl) {
    SSL_shutdown(conn->ssl);
    SSL_free(conn->ssl);
  }

  close(conn->fd);
  free(conn);
}

ssize_t ssl_read_or_plain(ssl_connection_t *conn, void *buffer, size_t size) {
  if (conn->ssl) {
    return SSL_read(conn->ssl, buffer, size);
  }

  return read(conn->fd, buffer, size);
}

ssize_t ssl_write_or_plain(ssl_connection_t *conn, const void *buffer,
                           size_t size) {
  if (conn->ssl) {
    return SSL_write(conn->ssl, buffer, size);
  }

  return write(conn->fd, buffer, size);
}
