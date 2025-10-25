#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
  size_t size;
  size_t capacity;  
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

void stadd(SimbleTable *tb, char *val, int num) {
  int exp = 31 - __builtin_clz(tb->size);

  if (tb->size >= 0.66 * tb->capacity) {
    tb->capacity *= 2;
    struct _entry *novo_val = calloc(tb->capacity, sizeof(tb->valores[0]));

    for (int i = 0; i < tb->capacity; i++) {
      if (tb->valores[i].estado != EN_OCUPADO)
        continue;

      char *t_key = tb->valores[i].nome;
      int t_id = tb->valores[i].id;
      int j = 0;
      int n = strlen(val);
      uint32_t pos = _hash1(exp, (uint8_t *)t_key, n);
      if (tb->valores[pos].estado != EN_VAZIO) {

        uint32_t h2 = _hash2(exp, (uint8_t *)t_key, n);
        while (tb->valores[pos].estado != EN_VAZIO)
          pos = (pos + h2) % tb->size;
      }
      novo_val[pos] = (struct _entry){EN_OCUPADO, t_key, t_id};
    }
  }

  int i = 0;
  int n = strlen(val);
  uint32_t pos = _hash1(exp, (uint8_t *)val, n);
  if (tb->valores[pos].estado != EN_VAZIO) {

    uint32_t h2 = _hash2(exp, (uint8_t *)val, n);
    while (tb->valores[pos].estado != EN_VAZIO) {
      pos = (pos + h2) % tb->size;
    }
  }
  tb->valores[pos] = (struct _entry){EN_OCUPADO, val, num};
}

int stget(SimbleTable *tb, char *val) {
  int exp = 31 - __builtin_clz(tb->size);
  int i = 0;
  int n = strlen(val);
  uint32_t pos = _hash1(exp, (uint8_t *)val, n);

  if (tb->valores[pos].estado == EN_OCUPADO){
    if (strcmp(val, tb->valores[pos].nome) == 0)
      return tb->valores[pos].id;

    uint32_t h2 = _hash2(exp, (uint8_t *)val, n);
    while (tb->valores[pos].estado != EN_VAZIO) {
      pos = (pos + h2) % tb->size;         
      if (strcmp(val, tb->valores[pos].nome) == 0)
        return tb->valores[pos].id;
    }
  }
  
  return -1;
}



int main(){
  SimbleTable *tb = stalloc();
  stadd(tb, "ola, mundo", 10);
  printf("%d\n", stget(tb, "ola, mundo"));
  return 0;
}
