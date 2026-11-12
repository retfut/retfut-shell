#ifndef HASHMAP_H
#define HASHMAP_H

#include <stdlib.h>

typedef struct Entry {
  char *key;
  int value;
  struct Entry *next;
} Entry;

typedef struct {
  Entry **buckets;
  size_t count;
  size_t capacity;
} HashMap;

HashMap* hashmap_create(void);
void hashmap_free(HashMap *map);
void hashmap_put(HashMap *map, const char *key, int value);
int hashmap_get(HashMap *map, const char *key, int *value); //return to *value
void hashmap_remove(HashMap *map, const char *key);

#endif

