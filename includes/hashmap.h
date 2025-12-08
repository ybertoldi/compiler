#include <stdint.h>
#include <stddef.h>

#ifndef HASH_MAP_H
#define HASH_MAP_H

struct _bucket {
  uint32_t hash;
  void *key;
  size_t ksize;
  uintptr_t value;
  struct _bucket *next;
};

typedef struct {
  struct _bucket *values;
  uint32_t capacity;
  uint32_t count;

  struct _bucket *first;
  struct _bucket *last;
} Map;


Map *map_init(void);
int map_set(Map *m, void *key, size_t ksize, uintptr_t value);
uintptr_t map_get(Map *m, void *key, size_t ksize);

#endif // HASH_MAP_H

