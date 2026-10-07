#ifndef MIN_HEAP_H
#define MIN_HEAP_H

typedef struct HeapNode HeapNode;
typedef struct MinHeap MinHeap;

struct HeapNode {
  int node;
  int dist;
};

struct MinHeap {
  HeapNode *arr;
  int size;
  int capacity;
};

MinHeap *min_heap_create(int capacity);
void min_heap_destroy(MinHeap *h);
void min_heap_push(MinHeap *h, const HeapNode value);
HeapNode min_heap_pop(MinHeap *h);

#endif
