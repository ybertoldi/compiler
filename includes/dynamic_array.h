#ifndef DYNAMIC_ARRAY_H
#define DYNAMIC_ARRAY_H
#include <stddef.h>
#include <stdlib.h>

// macros par arrays dinamicos

#define DA_APPEND(arr, value)                                                  \
  do {                                                                         \
    if (arr.count >= arr.capacity) {                                           \
      arr.capacity = (arr.capacity > 0) ? arr.capacity * 2 : 16;              \
      arr.elems = realloc(arr.elems, sizeof(arr.elems[0]));                    \
    }                                                                          \
    arr.elems[arr.count++] = value;                                            \
  } while (0)

#endif // DYNAMIC_ARRAY_H
