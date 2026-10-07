#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define HEAP_CAP 640000
#define CHUNK_LIST_CAP 1024

#define UNIMPLEMENTED                                                          \
  do {                                                                         \
    fprintf(stderr, "%s:%d: TODO: %s is not implemented\n", __FILE__,          \
            __LINE__, __func__);                                               \
    abort();                                                                   \
  } while (0)

typedef struct {
  void *start;
  size_t size;
} Chunk;

typedef struct {
  size_t count;
  Chunk chunks[CHUNK_LIST_CAP];
} Chunk_List;

void chunk_list_dump(const Chunk_List *list) {
  printf("Allocated Chunks (%zu):\n", list->count);

  for (size_t i = 0; i < list->count; ++i) {
    printf("\tstart: %p, size: %zu\n", list->chunks[i].start,
           list->chunks[i].size);
  }
}

int chunk_start_compar(const Chunk *a, const Chunk *b) {
  return a->start - b->start;
}

int chunk_list_find(const Chunk_List *list, void *ptr) {

  Chunk key = {.start = ptr};

  void *result = bsearch(&key, list->chunks, list->count,
                         sizeof(list->chunks[0]), chunk_start_compar);
  if (result == 0) {
    return -1;
  }

  assert(list->chunks <= result);
}

void chunk_list_insert(Chunk_List *list, void *start, size_t size) {
  assert(list->count < CHUNK_LIST_CAP);

  list->chunks[list->count].start = start;
  list->chunks[list->count].size = size;

  for (size_t i = list->count;
       i > 0 && list->chunks[i].start < list->chunks[i - 1].start; --i) {
    Chunk tmp = list->chunks[i];
    list->chunks[i] = list->chunks[i - 1];
    list->chunks[i - 1] = tmp;
  }

  list->count++;
}

void chunk_list_remove(Chunk_List *list, size_t idx) {
  (void)list;
  (void)idx;
  UNIMPLEMENTED;
}

char heap[HEAP_CAP] = {0};
size_t heap_size = 0;

Chunk_List alloced_chunks = {0};
Chunk_List freed_chunks = {0};

void *heap_alloc(size_t size) {
  if (size <= 0) {
    return NULL;
  }

  assert(heap_size + size <= HEAP_CAP);
  void *ptr = heap + heap_size;
  heap_size += size;
  chunk_list_insert(&alloced_chunks, ptr, size);

  return ptr;
}

void heap_free(void *ptr) {
  const int idx = chunk_list_find(&alloced_chunks, ptr);
  assert(idx >= 0);

  chunk_list_insert(&freed_chunks, alloced_chunks.chunks[idx].start,
                    alloced_chunks.chunks[idx].size);
  chunk_list_remove(&alloced_chunks, (size_t)idx);
}

void heap_collect() { UNIMPLEMENTED; }

int main() {
  for (int i = 0; i < 100; ++i) {
    void *p = heap_alloc(i);
    if (i % 2 == 0) {
      heap_free(p);
    }
  }

  chunk_list_dump(&alloced_chunks);

  return 0;
}
