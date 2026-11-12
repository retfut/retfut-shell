#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

Entry* create_entry(const char *key, int value){
  Entry *a = malloc(sizeof(Entry));
  a->key = strdup(key);
  a->value = value;
  a->next = NULL;
  return a;
}

void free_entry(Entry *a){
  free(a->key);
  free(a);
}

void free_list(Entry *a){
  while(a != NULL){
    Entry *next = a->next;
    free_entry(a);
    a = next;
  }
}

static unsigned long hash_string(const char *str){
  unsigned long hash = 5381;
  int c;
  while((c = *str++)) hash = ((hash << 5) + hash) + c;
  return hash;
}

static void hashmap_resize(HashMap *map){
  size_t new_capacity;
  if(map->count == 0) new_capacity = 16;
  else new_capacity = map->capacity * 2;
  Entry **new_buckets = calloc(new_capacity, sizeof(Entry*));
  
  for(int i = 0; i<map->capacity; i++){
    Entry *curr = map->buckets[i];
    while(curr != NULL){
      Entry *next = curr->next;

      unsigned long hash = hash_string(curr->key);
      size_t index = hash % new_capacity;

      curr->next = new_buckets[index];
      new_buckets[index] = curr;
      curr = next;
    }
  }
  free(map->buckets);
  map->buckets = new_buckets;
  map->capacity = new_capacity;
}

HashMap* hashmap_create(void){
  HashMap *map = calloc(1, sizeof(HashMap));
  return map;
}

void hashmap_free(HashMap *map){
  for(int i = 0; i<map->capacity; ++i)
    if(map->buckets[i] != NULL) free_list(map->buckets[i]);
  
}

void hashmap_put(HashMap *map, const char *key, int value){
  if(map->count == map->capacity) hashmap_resize(map);
  unsigned long hash = hash_string(key);
  size_t index = hash % map->capacity;
  
  Entry *curr = map->buckets[index];
  while(curr != NULL && strcmp(curr->key, key) != 0){
    curr = curr->next;
  }
  if(curr != NULL){
    curr->value = value;
    return;
  }
  Entry *head = create_entry(key, value);
  head->next = map->buckets[index];
  map->buckets[index] = head;
  map->count++;
}

int hashmap_get(HashMap *map, const char *key, int *value){
  unsigned long hash = hash_string(key);
  size_t index = hash % map->capacity;
  
  Entry *curr = map->buckets[index];
  while(curr != NULL){
    if(strcmp(curr->key, key) == 0){
      *value = curr->value;
      return 1;
    }
    curr = curr->next;
  }
  *value = -9999;
  return 0;
}

void hashmap_remove(HashMap *map, const char *key){
  unsigned long hash = hash_string(key);
  size_t index = hash % map->capacity;
  
  Entry *curr = map->buckets[index];
  Entry *prev = NULL;
  while(curr != NULL && strcmp(curr->key, key) != 0){
    prev = curr;
    curr = curr->next;
  }
  
  if(curr == NULL) return;
  if(prev == NULL)
    map->buckets[index] = curr->next;
  else
    prev->next = curr->next;
  free_entry(curr);
  map->count--;
}



