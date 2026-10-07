#include "trie.h"
#include <inttypes.h>
#include <linux/limits.h>
#include <stdlib.h>

static TrieNode *trie_create_node(void) {
  TrieNode *node = malloc(sizeof(*node));
  if (!node)
    return NULL;

  node->is_word = false;

  for (int i = 0; i < LETTER_COUNT; i++)
    node->children[i] = NULL;

  return node;
}

TrieNode *trie_traverse(Trie *trie, const char *str) {
  if (!trie || !str)
    return NULL;

  TrieNode *curr = trie->root;

  while (*str) {
    int i = *str - 'a';

    if (i < 0 || i >= LETTER_COUNT)
      return NULL;

    if (!curr->children[i])
      return NULL;

    curr = curr->children[i];
    str++;
  }

  return curr;
}

Trie *trie_create(void) {
  Trie *trie = malloc(sizeof(*trie));
  if (!trie)
    return NULL;

  trie->root = trie_create_node();
  if (!trie->root) {
    free(trie);
    return NULL;
  }

  trie->word_count = 0;
  trie->node_count = 1;

  return trie;
}

static void trie_destroy_node(TrieNode *node) {
  if (!node)
    return;

  for (int i = 0; i < LETTER_COUNT; i++)
    trie_destroy_node(node->children[i]);

  free(node);
}

void trie_destroy(Trie *trie) {
  if (!trie)
    return;

  trie_destroy_node(trie->root);
  free(trie);
}

bool trie_insert(Trie *trie, const char *word) {
  if (!trie || !word)
    return false;

  TrieNode *curr = trie->root;

  while (*word) {
    int i = *word - 'a';

    if (i < 0 || i >= LETTER_COUNT)
      return false;

    if (!curr->children[i]) {
      TrieNode *node = trie_create_node();
      if (!node)
        return false;

      curr->children[i] = node;
      trie->node_count++;
    }

    curr = curr->children[i];
    word++;
  }

  if (!curr->is_word) {
    curr->is_word = true;
    trie->word_count++;
  }

  return true;
}

bool trie_search(Trie *trie, const char *word) {
  TrieNode *node = trie_traverse(trie, word);
  return node && node->is_word;
}

bool trie_has_prefix(Trie *trie, const char *prefix) {
  return trie_traverse(trie, prefix) != NULL;
}

static void dfs_collect_words(TrieNode *node, char *buffer, int depth,
                              char results[][MAX_LEN], int *count,
                              int max_results) {
  if (!node || *count >= max_results) {
    return;
  }

  if (node->is_word) {
    buffer[depth] = '\0';

    int i = 0;
    while (buffer[i]) {
      results[*count][i] = buffer[i];
      ++i;
    }
    results[*count][i] = '\0';

    (*count)++;
  }

  for (int i = 0; i < LETTER_COUNT; ++i) {
    if (node->children[i]) {
      buffer[depth] = 'a' + i;

      dfs_collect_words(node->children[i], buffer, depth + 1, results, count,
                        max_results);
    }
  }
}

int trie_autocomplete(Trie *trie, const char *prefix, char results[][MAX_LEN],
                      int max_results) {
  if (!trie || !prefix || max_results <= 0) {
    return 0;
  }

  TrieNode *node = trie_traverse(trie, prefix);
  if (!node) {
    return 0;
  }

  char buffer[MAX_LEN];
  int depth = 0;

  while (prefix[depth] && depth < MAX_LEN - 1) {
    buffer[depth] = prefix[depth];
    ++depth;
  }

  int count = 0;

  dfs_collect_words(node, buffer, depth, results, &count, max_results);

  return count;
}
