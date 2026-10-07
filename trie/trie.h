#ifndef TRIE_H
#define TRIE_H

#include <stdbool.h>
#include <stddef.h>

#define LETTER_COUNT 26
#define MAX_RESULTS 100
#define MAX_LEN 128

typedef struct TrieNode TrieNode;
typedef struct Trie Trie;

struct TrieNode {
  TrieNode *children[LETTER_COUNT];
  bool is_word;
};

struct Trie {
  TrieNode *root;
  size_t word_count;
  size_t node_count;
};

Trie *trie_create(void);
void trie_destroy(Trie *trie);

bool trie_insert(Trie *trie, const char *word);
bool trie_search(Trie *trie, const char *word);
bool trie_has_prefix(Trie *trie, const char *prefix);

TrieNode *trie_traverse(Trie *trie, const char *str);
int trie_autocomplete(Trie *trie, const char *prefix, char results[][MAX_LEN],
                      int max_results);

#endif
