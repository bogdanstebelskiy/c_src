#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

void *thread_function(void *id) {
  long *myid = (long *)id;

  printf("Running thread function with id %ld...\n", *myid);

  return NULL;
}

int main(int argc, char **argv) {
  int nthreads;
  pthread_t *thread_array;
  long *thread_ids;

  if (argc != 2) {
    fprintf(stderr, "usage: %s <n>\n", argv[0]);
    fprintf(stderr, "where <n> is the number of threads\n");
    return 1;
  }

  nthreads = strtol(argv[1], NULL, 10);

  thread_array = malloc(nthreads * sizeof(pthread_t));
  thread_ids = malloc(nthreads * sizeof(long));

  for (int i = 0; i < nthreads; i++) {
    thread_ids[i] = i;
    pthread_create(&thread_array[i], NULL, thread_function, &thread_ids[i]);
  }

  for (int i = 0; i < nthreads; i++) {
    pthread_join(thread_array[i], NULL);
  }

  free(thread_array);
  free(thread_ids);

  return 0;
}
