#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>


#define TOKENIZER_H_DEBUG // funcoes para printar
#include "../includes/tokenizer.h"

bool read_file(char* filename, char **buf, size_t *buf_size){
  char *ret = NULL;

  FILE *src = fopen(filename, "r");
  if (src == NULL){
    perror("Could not open file\n");
    return false;
  }

  fseek(src, 0, SEEK_END);   
  *buf_size = ftell(src);
  rewind(src);

  ret = malloc(*buf_size + 1);
  if (ret == NULL){
    perror("Failed to allocate buffer\n");
    return false;
  }

  fread(ret, *buf_size, sizeof(char), src);
  ret[*buf_size] = 0;
  *buf = ret;
  return true;
}

int main(int argc, char *argv[]){
  if (argc != 2){
    printf("Usage: Compiler <src-file>\n");
    return 1;
  }

  char *buf;
  size_t bufsz;
  if (!read_file(argv[1], &buf, &bufsz)) return 1;

  TokenList *tkl = tokenize(buf);
  
  TokenList *cur = tkl;
  while (cur){
    printf(TKL_FMT "\n", TKL_ARGS(cur));
    cur = cur->next;
  }
  

  return 0;
}

