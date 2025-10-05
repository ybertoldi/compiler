#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

const uint32_t a = 2654435769;

enum EN_ESTADO {
  EN_VAZIO = 0,
  EN_OCUPADO = 1,
  EN_DELETADO = 2
};

struct _entry {
  uint8_t estado; 
  char *nome;
  int id;
};

typedef struct {
  struct _entry *valores;
  long size;
  long capacity;  
} SimbleTable;

/* hasheia um elemento de n bytes               *
 * para uma tabela de 2^exp elementos           *
 *                                              *
 * considerando que o tamanho maximo da tabela  *
 * eh 2^32                                      */
uint32_t _hash1(uint32_t exp, uint8_t *val, uint64_t n) {
  if (val == NULL) return 0; 
  uint32_t ret = 0;
  uint32_t loops = n / 4;
  uint32_t leftovers = n - loops*4;
  uint32_t j, k;

  for (int i = 0; i < loops; i++) {
    j = i*4;
    ret += val[j] << 24 | val[j + 1] << 16 | val[j + 2] << 8 | val[j + 3];
  }

  k = 0;
  for (int i = 0; i < leftovers; i++) 
      k |= val[loops*4 + i] << (8 * (leftovers - 1 - i));
  ret += k;

  return (ret * a) >> (32 - exp);
}

uint32_t _hash2(uint32_t exp, uint8_t *val, uint_fast64_t n){
  int k = 0;
  for (int i = 0; i < n || i < 4; i++)
    k |= val[i] << (8 * (3 - i));
  
  return k % (1 << exp);
}

SimbleTable *stalloc(void){
  SimbleTable *tb = malloc(sizeof(SimbleTable));
  if (tb == NULL){ printf("stalloc: couldnt allocate\n"); exit(1);}

  tb->capacity = 4; // dado em potencias de 2. A capacidade inicial eh 2^4 = 16;
  tb->size = 0;
  tb->valores = calloc((1 << tb->capacity), sizeof(struct _entry));
  return tb;
}

void stadd(SimbleTable *tb, char *val, int num){
  if (tb->size >= 0.66 * tb->capacity){
    // realloc table
    return;
  }
  

  
}



