#include "trie.h"

#include <stdio.h>
#include <string.h>

#define MAX_RESULTS 100
#define MAX_LEN 128

#define LINE_LEN 256

int main(void) {
  Trie *trie = trie_create();
  if (!trie) {
    fprintf(stderr, "Failed to create trie\n");
    return 1;
  }

  char line[LINE_LEN];
  char results[MAX_RESULTS][MAX_LEN];

  printf("Commands:\n");
  printf("  add <word>\n");
  printf("  find <word>\n");
  printf("  prefix <prefix>\n");
  printf("  exit\n\n");

  while (1) {
    printf("> ");

    if (!fgets(line, sizeof(line), stdin)) {
      printf("\nEOF received. Exiting.\n");
      break;
    }

    /* Remove trailing newline */
    line[strcspn(line, "\n")] = '\0';

    char *command = strtok(line, " \t");

    if (!command) {
      continue;
    }

    if (strcmp(command, "exit") == 0) {
      printf("Exit.\n");
      break;
    }

    char *argument = strtok(NULL, " \t");

    if (!argument) {
      printf("Missing argument.\n");
      continue;
    }

    if (strcmp(command, "add") == 0) {
      if (trie_insert(trie, argument)) {
        printf("Added: %s\n", argument);
      } else {
        printf("Failed to add word.\n");
      }
    } else if (strcmp(command, "find") == 0) {
      printf("%s\n", trie_search(trie, argument) ? "YES" : "NO");
    } else if (strcmp(command, "prefix") == 0) {
      int count = trie_autocomplete(trie, argument, results, MAX_RESULTS);

      printf("Autocomplete for \"%s\" (%d found):\n", argument, count);

      for (int i = 0; i < count; i++) {
        printf("  %s\n", results[i]);
      }
    } else {
      printf("Unknown command: %s\n", command);
    }
  }

  trie_destroy(trie);
  return 0;
}
