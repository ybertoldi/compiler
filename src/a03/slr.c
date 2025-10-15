#include "../../includes/tokenizer.h"
#include "../../includes/common.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
  long target;
  long elements[256];
  long num_elems;
} Production;

#define PROD(tgt, ...)                                                         \
  (Production) {                                                               \
    .target = tgt, .elements = {__VA_ARGS__},                                  \
    .num_elems = (sizeof((long[]){__VA_ARGS__}) / sizeof(long))                \
  }

typedef enum {
  VAR_DEC = TKTYPE_NUM_TOKENS,
  VAR_EXPR,
  VAR_EXPR1,
  VAR_EXPR2,
  VAR_EXPR3,
  VAR_EXPR4,
  VAR_EXPR5,
  VAR_TIPO,
  VAR_CONST,
  VAR_NUM_VARS
} VARTYPE;

char * var2str(VARTYPE v){
  switch (v) {
     case VAR_DEC:   return "VAR_DEC";
     case VAR_EXPR:  return "VAR_EXPR";
     case VAR_EXPR1: return "VAR_EXPR1";
     case VAR_EXPR2: return "VAR_EXPR2";
     case VAR_EXPR3: return "VAR_EXPR3";
     case VAR_EXPR4: return "VAR_EXPR4";
     case VAR_EXPR5: return "VAR_EXPR5";
     case VAR_TIPO:  return "VAR_TIPO";
     case VAR_CONST: return "VAR_CONST";
     default: exit(1);
  }
}

#define START_SYMBOL VAR_DEC

long goto_table[39][VAR_NUM_VARS] = {
    [1] = {[VAR_TIPO] = 2, [TKTYPE_T_INT] = 3, [TKTYPE_T_CHAR] = 4, [TKTYPE_T_BOOL] = 5, [TKTYPE_T_LONG] = 6, [TKTYPE_T_FLOAT] = 7, [TKTYPE_T_DOUBLE] = 8},

    [2] = {[TKTYPE_VAR] = 9},

    [3] = {},
    [4] = {},
    [5] = {},
    [6] = {},
    [7] = {},
    [8] = {},
    [9] = {[TKTYPE_ATTRIBUTION] = 10},

    [10] = {[TKTYPE_VAR] = 11, [VAR_EXPR] = 12, [VAR_EXPR1] = 13, [VAR_EXPR2] = 14, [VAR_EXPR3] = 15, [VAR_EXPR4] = 16, [VAR_EXPR5] = 17, [VAR_CONST] = 18, [TKTYPE_OPPAREN] = 19,
            [TKTYPE_INT] = 20, [TKTYPE_FLOAT] = 21, [TKTYPE_HEXINT] = 22, [TKTYPE_OCTALINT] = 23, [TKTYPE_FALSE] = 24, [TKTYPE_TRUE] = 25},

    [11] = {},

    [12] = {[TKTYPE_SMCOLON] = 26, [TKTYPE_GT] = 27},

    [13] = {[TKTYPE_LT] = 28},
    [14] = {[TKTYPE_SUM] = 29},
    [15] = {[TKTYPE_SUB] = 30},
    [16] = {[TKTYPE_MULT] = 31},
    [17] = {},
    [18] = {},

    [19] = {[TKTYPE_VAR] = 11, [VAR_EXPR] = 32, [VAR_EXPR1] = 13, [VAR_EXPR2] = 14, [VAR_EXPR3] = 15, [VAR_EXPR4] = 16, [VAR_EXPR5] = 17, [VAR_CONST] = 18,
            [TKTYPE_OPPAREN] = 19, [TKTYPE_INT] = 20, [TKTYPE_FLOAT] = 21, [TKTYPE_HEXINT] = 22, [TKTYPE_OCTALINT] = 23, [TKTYPE_FALSE] = 24, [TKTYPE_TRUE] = 25},

    [20] = {},
    [21] = {},
    [22] = {},
    [23] = {},
    [24] = {},
    [25] = {},
    [26] = {},

    [27] = {[TKTYPE_VAR] = 11, [VAR_EXPR1] = 33, [VAR_EXPR2] = 14, [VAR_EXPR3] = 15, [VAR_EXPR4] = 16, [VAR_EXPR5] = 17, [VAR_CONST] = 18, [TKTYPE_OPPAREN] = 19, [TKTYPE_INT] = 20,
            [TKTYPE_FLOAT] = 21, [TKTYPE_HEXINT] = 22, [TKTYPE_OCTALINT] = 23, [TKTYPE_FALSE] = 24, [TKTYPE_TRUE] = 25},

    [28] = {[TKTYPE_VAR] = 11, [VAR_EXPR2] = 34, [VAR_EXPR3] = 15, [VAR_EXPR4] = 16, [VAR_EXPR5] = 17, [VAR_CONST] = 18, [TKTYPE_OPPAREN] = 19, [TKTYPE_INT] = 20, [TKTYPE_FLOAT] = 21,
            [TKTYPE_HEXINT] = 22, [TKTYPE_OCTALINT] = 23, [TKTYPE_FALSE] = 24, [TKTYPE_TRUE] = 25},

    [29] = {[TKTYPE_VAR] = 11, [VAR_EXPR3] = 35, [VAR_EXPR4] = 16, [VAR_EXPR5] = 17, [VAR_CONST] = 18, [TKTYPE_OPPAREN] = 19, [TKTYPE_INT] = 20, [TKTYPE_FLOAT] = 21,
            [TKTYPE_HEXINT] = 22, [TKTYPE_OCTALINT] = 23, [TKTYPE_FALSE] = 24, [TKTYPE_TRUE] = 25},

    [30] = {[TKTYPE_VAR] = 11, [VAR_EXPR4] = 36, [VAR_EXPR5] = 17, [VAR_CONST] = 18, [TKTYPE_OPPAREN] = 19, [TKTYPE_INT] = 20, [TKTYPE_FLOAT] = 21, [TKTYPE_HEXINT] = 22,
            [TKTYPE_OCTALINT] = 23, [TKTYPE_FALSE] = 24, [TKTYPE_TRUE] = 25},

    [31] = {[TKTYPE_VAR] = 11, [VAR_EXPR5] = 37, [VAR_CONST] = 18, [TKTYPE_OPPAREN] = 19, [TKTYPE_INT] = 20, [TKTYPE_FLOAT] = 21,
            [TKTYPE_HEXINT] = 22, [TKTYPE_OCTALINT] = 23, [TKTYPE_FALSE] = 24, [TKTYPE_TRUE] = 25},

    [32] = {[TKTYPE_GT] = 27, [TKTYPE_CLPAREN] = 38},
    [33] = {[TKTYPE_LT] = 28},
    [34] = {[TKTYPE_SUM] = 29},
    [35] = {[TKTYPE_SUB] = 30},
    [36] = {[TKTYPE_MULT] = 31},
    [37] = {},
    [38] = {},
};

Production reduce[39] = {
    [3] =  PROD(VAR_TIPO,       TKTYPE_T_INT),
    [4] =  PROD(VAR_TIPO,       TKTYPE_T_CHAR),
    [5] =  PROD(VAR_TIPO,       TKTYPE_T_BOOL),
    [6] =  PROD(VAR_TIPO,       TKTYPE_T_LONG),
    [7] =  PROD(VAR_TIPO,       TKTYPE_T_FLOAT),
    [8] =  PROD(VAR_TIPO,       TKTYPE_T_DOUBLE),
    [11] = PROD(VAR_EXPR5,      TKTYPE_VAR),
    [13] = PROD(VAR_EXPR,       VAR_EXPR1),
    [14] = PROD(VAR_EXPR1,      VAR_EXPR2),
    [15] = PROD(VAR_EXPR2,      VAR_EXPR3),
    [16] = PROD(VAR_EXPR3,      VAR_EXPR4),
    [17] = PROD(VAR_EXPR4,      VAR_EXPR5),
    [18] = PROD(VAR_EXPR5,      VAR_CONST),
    [20] = PROD(VAR_CONST,      TKTYPE_INT),
    [21] = PROD(VAR_CONST,      TKTYPE_FLOAT),
    [22] = PROD(VAR_CONST,      TKTYPE_HEXINT),
    [23] = PROD(VAR_CONST,      TKTYPE_OCTALINT),
    [24] = PROD(VAR_CONST,      TKTYPE_FALSE),
    [25] = PROD(VAR_CONST,      TKTYPE_TRUE),
    [26] = PROD(VAR_DEC,        VAR_TIPO, TKTYPE_VAR, TKTYPE_ATTRIBUTION, VAR_EXPR, TKTYPE_SMCOLON),
    [33] = PROD(VAR_EXPR,       VAR_EXPR, TKTYPE_GT, VAR_EXPR1),
    [34] = PROD(VAR_EXPR1,      VAR_EXPR1, TKTYPE_LT, VAR_EXPR2),
    [35] = PROD(VAR_EXPR2,      VAR_EXPR2, TKTYPE_SUM, VAR_EXPR3),
    [36] = PROD(VAR_EXPR3,      VAR_EXPR3, TKTYPE_SUB, VAR_EXPR4),
    [37] = PROD(VAR_EXPR4,      VAR_EXPR4, TKTYPE_MULT, VAR_EXPR5),
    [38] = PROD(VAR_EXPR5,      TKTYPE_OPPAREN, VAR_EXPR, TKTYPE_CLPAREN),
};

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


void printstate(long t){
  printf("STATES = {");
  for (int i = 0; i < stp; i++) {
    printf("%ld, ", state_stack[i]);
  }
  printf("}\t");

  printf("SYMBOLS = {");
  for (int i = 0; i < syp; i++) {
    printf("%s, ", (symbols_stack[i] >= TKTYPE_NUM_TOKENS) ? var2str(symbols_stack[i]) : type2str(symbols_stack[i]));
  }
  printf("}\t");
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
            fprintf(stderr, "syntax error\n");
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
