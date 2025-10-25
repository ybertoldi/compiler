#include <stdint.h>
#include <stddef.h>

#ifndef HASH_MAP_H
#define HASH_MAP_H

typedef struct Map Map; // estrutura opaca

Map *map_init(void);
int map_set(Map *m, void *key, size_t ksize, uintptr_t value);
uintptr_t map_get(Map *m, void *key, size_t ksize);

#endif // HASH_MAP_H

