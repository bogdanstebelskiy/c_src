#include <assert.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

struct block_data {
  size_t size;
  struct block_data *next;
  int free;
  int magic;
};

#define DATA_BLOCK_SIZE sizeof(struct block_data)

void *global_base = NULL;

struct block_data *get_block_ptr(void *ptr) {
  return (struct block_data *)ptr - 1;
}

struct block_data *find_free_block(struct block_data **last, size_t size) {
  struct block_data *current = global_base;

  while (current && !(current->free && current->size >= size)) {
    *last = current;
    current = current->next;
  }

  return current;
}

struct block_data *request_space(struct block_data *last, size_t size) {
  struct block_data *block;
  block = sbrk(0);
  void *request = sbrk(size + DATA_BLOCK_SIZE);
  assert((void *)block == request);
  if (request == (void *)-1) {
    return NULL; // Failed to allocate memory
  }

  if (last) {
    last->next = block;
  }
  block->size = size;
  block->next = NULL;
  block->free = 0;
  block->magic = 0x12345678;
  return block;
}

void *malloc(size_t size) {
  struct block_data *block;

  if (size <= 0) {
    return NULL;
  }

  if (global_base == NULL) {
    block = request_space(NULL, size);
    if (!block) {
      return NULL;
    }

    global_base = block;
  } else {
    struct block_data *last = global_base;
    block = find_free_block(&last, size);
    if (!block) {
      block = request_space(last, size);
      if (!block) {
        return NULL;
      }
    } else {
      block->free = 0;
      block->magic = 0x77777777;
    }
  }

  return (block + 1);
}

void free(void *ptr) {
  if (ptr == NULL) {
    return;
  }

  struct block_data *block_ptr = get_block_ptr(ptr);
  assert(block_ptr->free == 0);
  assert(block_ptr->magic == 0x77777777 || block_ptr->magic == 0x12345678);
  block_ptr->free = 1;
  block_ptr->magic = 0x55555555;
}

void *realloc(void *ptr, size_t size) {
  if (ptr == NULL) {
    return malloc(size);
  }

  struct block_data *block_ptr = get_block_ptr(ptr);
  if (block_ptr->size >= size) {
    // Free after split implementation
    return ptr;
  }

  void *new_ptr;
  new_ptr = malloc(size);
  if (new_ptr == NULL) {
    // Should return errno on failure
    return NULL;
  }

  memcpy(new_ptr, ptr, block_ptr->size);
  free(ptr);
  return new_ptr;
}

void *calloc(size_t nelem, size_t elsize) {
  size_t size = nelem * elsize; // should check for overflow
  void *ptr = malloc(size);
  memset(ptr, 0, size);
  return ptr;
}
