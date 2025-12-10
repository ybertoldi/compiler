#include "../includes/tokenizer.h"
#include "../includes/common.h"
#include "../includes/grammar.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#define STACK_SIZE 4096
long state_stack[STACK_SIZE];
long stp = 0;
void state_push(long v) { assert(stp < STACK_SIZE); state_stack[stp++] = v; }
long state_pop(void)    { assert(stp > 0);          return state_stack[--stp]; }
long state_peek(void)   { assert(stp > 0);          return state_stack[stp - 1]; }

long symbols_stack[STACK_SIZE];
long syp = 0;
void symbol_push(long v) { assert(syp < STACK_SIZE); symbols_stack[syp++] = v; }
long symbol_pop(void)    { assert(syp > 0);          return symbols_stack[--syp]; }
long symbol_peek(void)   { assert(syp > 0);          return symbols_stack[syp - 1]; }

void printstate(long t) {
  printf("STATES = {");
  for (int i = 0; i < stp; i++) {
    printf("%ld%s", state_stack[i], (i == stp - 1) ? "" : ", ");
  }
  printf("}");
  printf("\e[75G");
  

  printf("SYMBOLS = {");
  for (int i = 0; i < syp; i++) {
    printf("%s%s",
           (symbols_stack[i] >= TKTYPE_NUM_TOKENS) ? var2str(symbols_stack[i])
                                                   : type2str(symbols_stack[i]),
           (i == syp - 1) ? "" : ", ");
  }
  printf("}\t");

  printf("\e[175G");
  printf("INPUT = %s\n", type2str(t));
}

int main() {
  char buf[BUFSIZ];

  printf("input > ");
  while (readline(buf, BUFSIZ)) {
    state_push(1);
    TokenList *tklist = tokenize(buf); // TODO: funcao para limpar os tokens
    int parse = 2;
    long gt_state;
    while (parse) {
      if (parse == 2) printstate(tklist->type);

      if (tklist && (gt_state = goto_table[state_peek()][tklist->type]) != 0) {
        symbol_push(tklist->type);
        state_push(gt_state);
        tklist = tklist->next;
      } else {
        Production p = reduce[state_peek()];
        if (p.target == 0) {
          fprintf(stderr, "syntax error\n");
          exit(1);
        }

        for (int i = p.num_elems - 1; i >= 0; i--) {
          if (p.elements[i] != symbol_pop()) { 
            fprintf(stderr, "syntax error, expecting %s\n", 
                (p.elements[i] > TKTYPE_NUM_TOKENS)? var2str(p.elements[i]) : type2str(p.elements[i]));
            exit(1);
          }

          state_pop();
        }

        symbol_push(p.target);
        if ((gt_state = goto_table[state_peek()][p.target]) != 0) {
          state_push(gt_state);
        }
      }

      parse -= (tklist == NULL)? 1 : 0;
    }

    printstate(0);
    if (symbol_peek() == START_SYMBOL){
      printf("ACC\n\n");
    } else {
      printf("ERR\n\n");
    }
    printf("final state: %ld\n", state_pop());
    printf("input > ");
    syp = 0;
    stp = 0;
  }
  return 0;
}
