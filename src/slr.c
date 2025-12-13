#define TOKENIZER_H_DEBUG
#include "../includes/tokenizer.h"
#include "../includes/grammar.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

// value stack entry
typedef struct Value {
  TKTYPE type;
  char *lexeme;
} Value ;

/*---------------------------------------STACKS-----------------------------------------*/
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

static Value values_stack[STACK_SIZE];
static long svp = 0;
static void values_push(TokenList *t) { assert(svp < STACK_SIZE); values_stack[svp++] = (Value){t->type, t->lexeme};}
static Value values_pop(void)         { assert(svp > 0);          return values_stack[--svp]; }
static Value values_peek(void)        { assert(svp > 0);          return values_stack[svp - 1]; }

// debug stacks
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
/*--------------------------------------------------------------------------------------*/

static void do_reduction(Production *p);
static inline void default_validation(Production *p);

void grammar_test(char *buf){
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
      values_push(tklist);
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

#ifdef SLR_DEBUG
      do_reduction(&p);
#else
      default_validation(&p);
#endif //SLR_DEBUG
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

static inline void consume_symbol(long expected){
      if (expected != symbol_pop()) 
        unexpected_symbol_err(expected);
      state_pop();
}

static inline void default_validation(Production *p){
      for (int i = p->num_elems - 1; i >= 0; i--) consume_symbol(p->elements[i]);
}

// Constroi a arvore de sintaxe abstrata na hora de fazer as reducoes.
static void do_reduction(Production *p){
  switch (p->target) {
    case VAR_LOGIC_OR:{
      printf("TODO: VAR_LOGIC_OR\n");
      default_validation(p);
    }break;

    case VAR_LOGIC_AND:{
      printf("TODO: VAR_LOGIC_AND\n");
      default_validation(p);
    }break;

    case VAR_CMP_EQ:{
      printf("TODO: VAR_CMP_EQ\n");
      default_validation(p);
    }break;

    case VAR_CMP_NEQ:{
      printf("TODO: VAR_CMP_NEQ\n");
      default_validation(p);
    }break;

    case VAR_CMP_LEQT:{
      printf("TODO: VAR_CMP_LEQT\n");
      default_validation(p);
    }break;

    case VAR_CMP_GT:{
      printf("TODO: VAR_CMP_GT\n");
      default_validation(p);
    }break;

    case VAR_CMP_GEQT:{
      printf("TODO: VAR_CMP_GEQT\n");
      default_validation(p);
    }break;

    case VAR_MULT:{
      printf("TODO: VAR_MULT\n");
      default_validation(p);
    }break;

    case VAR_DIV:{
      printf("TODO: VAR_DIV\n");
      default_validation(p);
    }break;

    case VAR_SUM:{
      printf("TODO: VAR_SUM\n");
      default_validation(p);
    }break;

    case VAR_SUB:{
      printf("TODO: VAR_SUB\n");
      default_validation(p);
    }break;

    case VAR_CONST:{
      printf("TODO: VAR_CONST\n");
      printf("Current value: (%s, %s)\n", type2str(values_peek().type), values_peek().lexeme);
      values_pop();
      default_validation(p);
    }break;

    case VAR_TIPO:{
      printf("TODO: VAR_TIPO\n");
      default_validation(p);
    }break;

    default: {
      default_validation(p);
    } break;
  }
}





/*--------------------DEBUG ONLY--------------------*/
#ifdef SLR_DEBUG
int main() {
  char buf[BUFSIZ];
  fread(buf, sizeof(buf), sizeof(char), stdin);
  grammar_test(buf);
  return 0;
}
#endif //SLR_DEBUG
