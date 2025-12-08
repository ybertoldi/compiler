#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../includes/hashmap.h"

#define MAP_INITIAL_CAPACITY 32
Map *map_init(void){
  Map *m = malloc(sizeof(Map));
  if (m == NULL)
    return NULL;

  m->capacity = MAP_INITIAL_CAPACITY;
  m->count = 0;
  m->values = calloc(m->capacity, sizeof(struct _bucket));

  if (m->values == NULL){
    free(m);
    return NULL;
  }

  m->first = NULL;
  m->last = (struct bucket*)&m->first;
  return m;
}

// uma vez mudado o tamanho do mapa, todos os buckets passam por aqui
static struct _bucket* fit_entry_to_new_array(Map *m, struct _bucket *entry){
  uint32_t index = entry->hash % m->capacity;
  for (;;) {
    struct _bucket *cur = &m->values[index];
    if (cur->key == NULL && cur->value == 0){
      *cur = *entry;
      return cur;
    }  
    index = (index + 1) % m->capacity;
  }
}

static int map_resize(Map *m){
  uint32_t old_capacity = m->capacity;
  struct _bucket *old_values;

  m->capacity *= 2;
  m->values = calloc(m->capacity, sizeof(struct _bucket));
  if (m->values == NULL){
    m->capacity = old_capacity;
    m->values = old_values;
    return 0;
  }

  struct _bucket *old_iter, *new, *new_paren;
  old_iter = m->first;
  new_paren = NULL;
  m->first = NULL;
  while (old_iter) {
    new = fit_entry_to_new_array(m, old_iter);    

    if (new_paren)
      new_paren->next = new;
    else if (m->first == NULL)
      m->first = new;

    new_paren = new;
    old_iter = old_iter->next;  
  }
  new->next = NULL;
  m->last = new;
  
  free(old_values);
  return 1;
}

// H(k) = k * (k + 3) % m
static uint32_t hash_data(unsigned char *data, size_t data_size){
  int chunks = data_size / 4;
  int leftover = data_size % 4;  
  uint32_t hash = 0;
  uint32_t u32data = 0;
  while (chunks--) {
    memcpy(&u32data, data, 4); // espero que o compilador deixe isso inline
    hash += u32data * (u32data + 3);
    data += 4;
  }

  if (leftover){
    memcpy(&u32data, data, leftover);
  } else {
    u32data = 0;
  }

  hash += u32data;
  return hash;
}

struct _bucket *find_entry(Map *m, void *key, size_t ksize, uint32_t hash) {
  uint32_t index = hash % m->capacity;
  for (;;) {
    struct _bucket *cur = &m->values[index];

    if (cur->key == NULL)
      return cur;

    
    if (cur->ksize == ksize && cur->hash == hash &&
        memcmp(cur->key, key, ksize) == 0)
      return cur;

    index = (index + 1) % m->capacity;
  }
}

int map_set(Map *m, void *key, size_t ksize, uintptr_t value){
  if (m->count + 1 > 0.6 * m->capacity)
    if (!map_resize(m))
      return 0;

  uint32_t hash = hash_data(key, ksize);
  struct _bucket *entry = find_entry(m, key, ksize, hash);
  if (entry->key == NULL){

    if (m->first == NULL)
      m->first = entry;

    m->last->next = entry;
    m->last = entry;
    m->count += 1;

    entry->hash = hash;
    entry->key = key;
    entry->ksize = ksize;
    entry->next = NULL;
  } 

  entry->value = value;
  return 1;
}

uintptr_t map_get(Map *m, void *key, size_t ksize){
  uint32_t hash = hash_data(key, ksize);
  struct _bucket *entry = find_entry(m, key, ksize, hash);
  if (entry->key == NULL)
    return 0;

  return entry->value;
}

/* 
 * #define table_set(m, k, v) map_set(m, k, strlen(k) - 1, (uintptr_t) v)
 * #define table_get(m, k) map_get(m, k, strlen(k) - 1)
 * 
 * int main(){
 *   Map *map = map_init();
 *   table_set(map, "hello", 10);
 *   table_set(map, "world", 50);
 *   table_set(map, "mensagem", "hello, world!");
 * 
 *   printf("%d\n", (int) table_get(map, "hello"));
 *   printf("%d\n", (int) table_get(map, "world"));
 *   printf("%s\n", (char *) table_get(map, "mensagem"));
 * 
 *   return 0;
 * }
*/

