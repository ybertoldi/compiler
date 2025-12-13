#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define TOKENIZER_H_DEBUG // funcoes para printar
#include "../includes/tokenizer.h"
#include "../includes/ast.h"


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

  size_t read = fread(ret, sizeof(char), *buf_size + 1, src);
  if (read != *buf_size) {
    perror("Could not read entire file\n");
    return false;
  }

  ret[*buf_size] = 0;
  *buf = ret;
  return true;
}

void usage_ext(int ext_code){
    printf("Usage: Compiler <src-file> [-t | -g]\n\n"
           "Flags:\n"
           "   -t      print tokens\n"
           "   -g      print grammar\n");
    exit(ext_code);
}

int main(int argc, char *argv[]){
  enum {
    OPT_DEFAULT,
    OPT_PRINT_TOKENS,
    OPT_PRINT_GRAMMAR
  } opt;

  opt = OPT_DEFAULT;

  if (argc < 2)
    usage_ext(1);

  if (argc == 3){
    if (argv[2][0] == '-'){
      if (argv[2][1] == 't'){
        opt = OPT_PRINT_TOKENS;
      } else if (argv[2][1] == 'g') {
        opt = OPT_PRINT_GRAMMAR;      
      } else {
        printf("Invalid argumet %s\n\n", argv[2]);
        usage_ext(1);
      } 
    } else {
      printf("Invalid Command %s\n", argv[2]);
      usage_ext(1);
    }
  }

  char *buf;
  size_t bufsz;
  if (!read_file(argv[1], &buf, &bufsz)) return 1;

  switch (opt) {
    case OPT_PRINT_TOKENS: {
      TokenList *tkl = tokenize(buf);
      TokenList *cur = tkl;
      while (cur) {
        printf(TKL_FMT "\n", TKL_ARGS(cur));
        cur = cur->next;
      }
    } break;

    case OPT_PRINT_GRAMMAR:{
      grammar_test(buf);
    } break;

    case OPT_DEFAULT:
      printf("DEFAULT: not implemented\n");
      break;
    
  }

  return 0;
}

