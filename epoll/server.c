#include <asm-generic/errno.h>
#include <asm-generic/socket.h>
#include <errno.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <unistd.h>

#include "queue.h"

#include "http_parser.h"
#include "http_response.h"

#include "handlers.h"
#include "router.h"

#include "ssl.h"

extern SSL_CTX *ssl_ctx;

#define PORT 5678
#define THREAD_POOL_SIZE 4
#define MAX_EVENTS 64
#define BUFFER_SIZE 1024

pthread_mutex_t queue_mutex = PTHREAD_MUTEX_INITIALIZER;
sem_t work_available;

int make_non_blocking(int fd) {
  int flags = fcntl(fd, F_GETFL, 0);
  if (flags == -1) {
    return -1;
  }
  return fcntl(fd, F_SETFL, flags | O_NONBLOCK);
}

void route_http_request(char *buffer, ssl_connection_t *conn, int thread_id) {
  http_request_t request;
  if (parse_http_request(buffer, &request) == 0) {
    printf("[Thread %d] %s %s %s\n", thread_id, request.method, request.path,
           request.version);

    route_response_t response = route_handle(request.path);

    send_http_response(conn, response.status_code, response.status_text,
                       response.content_type, response.body);
  } else {
    send_http_response(conn, 400, "Bad Request", "text/html",
                       "<html><body><h1>400 Bad Request</h1></body></html>");
  }
}

void *worker_thread(void *arg) {
  int thread_id = *(int *)arg;
  printf("[Thread %d] Started\n", thread_id);

  while (1) {
    sem_wait(&work_available);

    pthread_mutex_lock(&queue_mutex);
    int *client_socket = dequeue();
    pthread_mutex_unlock(&queue_mutex);

    if (client_socket == NULL) {
      continue;
    }

    printf("[Thread %d] Processing client (fd=%d)\n", thread_id,
           *client_socket);

    ssl_connection_t *conn = ssl_accept_connection(ssl_ctx, *client_socket);
    if (!conn) {
      close(*client_socket);
      free(client_socket);
      continue;
    }

    char buffer[BUFFER_SIZE];
    ssize_t bytes_read = ssl_read_or_plain(conn, buffer, BUFFER_SIZE - 1);

    if (bytes_read > 0) {
      buffer[bytes_read] = '\0';
      printf("[Thread %d] Received: %s", thread_id, buffer);
      // write(*client_socket, buffer, bytes_read);
      route_http_request(buffer, conn, thread_id);
    }

    ssl_close_connection(conn);
    free(client_socket);
    printf("[Thread %d] Client closed\n", thread_id);
  }

  return NULL;
}

int create_listening_socket() {
  int listen_fd = socket(AF_INET, SOCK_STREAM, 0);
  if (listen_fd == -1) {
    perror("socket");
    return -1;
  }

  int opt = 1;
  setsockopt(listen_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

  struct sockaddr_in addr;
  memset(&addr, 0, sizeof(addr));
  addr.sin_family = AF_INET;
  addr.sin_addr.s_addr = INADDR_ANY;
  addr.sin_port = htons(PORT);

  if (bind(listen_fd, (struct sockaddr *)&addr, sizeof(addr)) == -1) {
    perror("bind");
    close(listen_fd);
    return -1;
  }

  if (listen(listen_fd, SOMAXCONN) == -1) {
    perror("listen");
    close(listen_fd);
    return -1;
  }

  return listen_fd;
}

int main() {
  router_init();

  router_add("/", handle_home);
  router_add("/about", handle_about);
  router_add("/api/status", handle_api_status);

  printf("Thread pool size: %d\n", THREAD_POOL_SIZE);

  ssl_ctx = ssl_init("server.crt", "server.key");
  if (ssl_ctx) {
    printf("SSL enabled\n");
  } else {
    printf("SSL disabled\n");
  }

  if (sem_init(&work_available, 0, 0) == -1) {
    perror("sem_init");
    exit(EXIT_FAILURE);
  }

  int listen_fd = create_listening_socket();
  if (listen_fd == -1) {
    exit(EXIT_FAILURE);
  }

  if (make_non_blocking(listen_fd) == -1) {
    perror("make_non_blocking");
    exit(EXIT_FAILURE);
  }

  printf("Listening on port: %d\n\n", PORT);

  pthread_t threads[THREAD_POOL_SIZE];
  int thread_ids[THREAD_POOL_SIZE];

  for (int i = 0; i < THREAD_POOL_SIZE; ++i) {
    thread_ids[i] = i;
    if (pthread_create(&threads[i], NULL, worker_thread, &thread_ids[i]) != 0) {
      perror("pthread_create");
      exit(EXIT_FAILURE);
    }
  }

  printf("[Main] Created %d worker threads\n\n", THREAD_POOL_SIZE);

  int epoll_fd = epoll_create1(0);
  if (epoll_fd == -1) {
    perror("epoll_create1");
    exit(EXIT_FAILURE);
  }

  struct epoll_event event;
  event.events = EPOLLIN | EPOLLET;
  event.data.fd = listen_fd;

  if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, listen_fd, &event) == -1) {
    perror("epoll_ctl");
    exit(EXIT_FAILURE);
  }

  printf("[Main] epoll monitoring started\n");

  struct epoll_event events[MAX_EVENTS];

  while (1) {
    int nfds = epoll_wait(epoll_fd, events, MAX_EVENTS, -1);

    if (nfds == -1) {
      perror("epoll_wait");
      break;
    }

    for (int i = 0; i < nfds; ++i) {
      if (events[i].data.fd == listen_fd) {
        while (1) {
          struct sockaddr_in client_addr;
          socklen_t client_len = sizeof(client_addr);

          int client_fd =
              accept(listen_fd, (struct sockaddr *)&client_addr, &client_len);

          if (client_fd == -1) {
            if (errno == EAGAIN || errno == EWOULDBLOCK) {
              break;
            } else {
              perror("accept");
              break;
            }
          }

          printf("[Main] New connection (fd=%d)\n", client_fd);

          int *client_socket = malloc(sizeof(int));
          *client_socket = client_fd;

          pthread_mutex_lock(&queue_mutex);
          enqueue(client_socket);
          pthread_mutex_unlock(&queue_mutex);

          sem_post(&work_available);

          printf("[Main] Queued client for processing\n");
        }
      }
    }
  }

  close(listen_fd);
  close(epoll_fd);
  sem_destroy(&work_available);

  ssl_cleanup(ssl_ctx);

  return 0;
}
