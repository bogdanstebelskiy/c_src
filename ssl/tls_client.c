#include <arpa/inet.h>
#include <openssl/err.h>
#include <openssl/ssl.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

int main() {
  SSL_library_init();
  SSL_load_error_strings();

  SSL_CTX *ctx = SSL_CTX_new(TLS_client_method());
  if (ctx == NULL) {
    ERR_print_errors_fp(stderr);
    return 1;
  }

  int sock = socket(AF_INET, SOCK_STREAM, 0);

  struct sockaddr_in addr;
  memset(&addr, 0, sizeof(addr));
  addr.sin_family = AF_INET;
  addr.sin_port = htons(443);

  inet_pton(AF_INET, "142.251.98.139", &addr.sin_addr);

  if (connect(sock, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
    perror("Connections error!");
    return 1;
  }

  SSL *ssl = SSL_new(ctx);
  SSL_set_fd(ssl, sock);

  if (SSL_connect(ssl) <= 0) {
    ERR_print_errors_fp(stderr);
    return 1;
  }

  printf("TLS connected using: %s\n", SSL_get_cipher(ssl));

  const char *req = "GET / HTTP/1.1\r\n"
                    "Host: example.com\r\n"
                    "Connection: close\r\n\r\n";

  SSL_write(ssl, req, strlen(req));

  char buf[4096];
  int n;

  while ((n = SSL_read(ssl, buf, sizeof(buf) - 1)) > 0) {
    buf[n] = '\0';
    printf("%s", buf);
  }

  SSL_shutdown(ssl);
  SSL_free(ssl);
  close(sock);
  SSL_CTX_free(ctx);
}
