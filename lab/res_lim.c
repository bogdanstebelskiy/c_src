#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <unistd.h>

#define MAX_RESOURCES 3
#define NUM_THREADS 6

sem_t sem;

void *access_resource(void *arg) {
  int id = *(int *)arg;

  sem_wait(&sem);
  printf("Thread %d: Using resource\n", id);
  sleep(2);
  printf("Thread %d: Releasing resource\n", id);
  sem_post(&sem);

  return NULL;
}

int main() {
  pthread_t threads[NUM_THREADS];
  int ids[NUM_THREADS];

  sem_init(&sem, 0, MAX_RESOURCES);

  for (int i = 0; i < NUM_THREADS; ++i) {
    ids[i] = i;
    pthread_create(&threads[i], NULL, access_resource, &ids[i]);
  }

  for (int i = 0; i < NUM_THREADS; ++i) {
    pthread_join(threads[i], NULL);
  }

  sem_destroy(&sem);
  return 0;
}
