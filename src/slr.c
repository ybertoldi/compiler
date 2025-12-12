#include "../includes/tokenizer.h"
#include "../includes/grammar.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>


#define STACK_SIZE 4096
static long state_stack[STACK_SIZE];
static long stp = 0;
static void state_push(long v) { assert(stp < STACK_SIZE); state_stack[stp++] = v; }
static long state_pop(void)    { assert(stp > 0);          return state_stack[--stp]; }
static long state_peek(void)   { assert(stp > 0);          return state_stack[stp - 1]; }

static long symbols_stack[STACK_SIZE];
static long syp = 0;
static void symbol_push(long v) { assert(syp < STACK_SIZE); symbols_stack[syp++] = v; }
static long symbol_pop(void)    { assert(syp > 0);          return symbols_stack[--syp]; }
static long symbol_peek(void)   { assert(syp > 0);          return symbols_stack[syp - 1]; }

void do_reduction(Production *p);

static void printstate(long t) {
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
  fread(buf, sizeof(buf), sizeof(char), stdin);

  state_push(1);
  TokenList *tklist = tokenize(buf);
  enum {PARSING_TOKENS, PARSING_EOF} parse_state = PARSING_TOKENS;
  long gt_state;

  while (1) {
    if (parse_state == PARSING_TOKENS)
      printstate(tklist->type);
    else
      printstate(0);

    if (parse_state == PARSING_TOKENS && tklist && (gt_state = goto_table[state_peek()][tklist->type]) != 0) {
      symbol_push(tklist->type);
      state_push(gt_state);
      tklist = tklist->next;
    } else {
      Production p = rdc_table[state_peek()];

      if (p.target == 0) {
        if (parse_state == PARSING_EOF){
          break;
        } else {
          fprintf(stderr, "syntax error\n");
          exit(1);
        }
      }

      do_reduction(&p);

      symbol_push(p.target);
      if ((gt_state = goto_table[state_peek()][p.target]) != 0) {
        state_push(gt_state);
      }
    }

    if (tklist == NULL) // terminou de consumir os tokens
      parse_state = PARSING_EOF;
  }

  if (symbol_peek() == START_SYMBOL) {
    printf("ACC\n\n");
  } else {
    printf("ERR\n\n");
  }
  return 0;
}

static inline void unexpected_symbol_err(long expected) {
  char *expected_str;
  if (expected >= TKTYPE_NUM_TOKENS){
    expected_str = var2str(expected);
  } else {
    expected_str = type2str(expected);
  }

  fprintf(stderr, "syntax error, expecting %s\n", expected_str);
  exit(1);
}

void do_reduction(Production *p){
  switch (p->target) {
    default: {
      for (int i = p->num_elems - 1; i >= 0; i--) {
        if (p->elements[i] != symbol_pop()) 
          unexpected_symbol_err(p->elements[i]);

        state_pop();
      }
    } break;

  }
}
