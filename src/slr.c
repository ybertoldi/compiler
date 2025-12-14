#define TOKENIZER_H_DEBUG
#include "../includes/tokenizer.h"
#include "../includes/grammar.h"
#include "../includes/ast.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct Symbol {
  long symbol_type;
  AstNode node;
} Symbol ;

#define BUILD_BNOP(btype, lhs, rhs)                                            \
  (AstNode) {                                                                  \
    .type = ASTYPE_BINOP, .binop_type = btype, .left = lhs, .right = rhs       \
  }

char *binopstr(long binop){
  switch (binop) {
    case BNOP_SUM:    return "SUM";
    case BNOP_SUB:    return "SUB";
    case BNOP_MULT:   return "MULT";
    case BNOP_DIV:    return "DIV";
    case BNOP_MOD:    return "MOD";
    case BNOP_SHL:    return "SHL";
    case BNOP_SHR:    return "SHR";
    case BNOP_BW_OR:  return "BW_OR";
    case BNOP_BW_AND: return "BW_AND";
    case BNOP_BW_XOR: return "BW_XOR";
    case BNOP_OR:     return "OR";
    case BNOP_AND:    return "AND";
    case BNOP_EQ:     return "EQ";
    case BNOP_NEQ:    return "NEQ";
    case BNOP_GT:     return "GT";
    case BNOP_GEQT:   return "GEQT";
    case BNOP_LT:     return "LT";
    case BNOP_LEQT:   return "LEQT";
  }
  return "UNKOWN_BINOP";
}

void print_ast(AstNode *n){
  switch(n->type){
    case ASTYPE_TERM: {
      fputs(n->lexeme, stdout);
    } break;
    case ASTYPE_BINOP: {
      printf("BNOP(%s,", binopstr(n->binop_type));
      print_ast(n->left);
      printf(",");
      print_ast(n->right);
      printf(")");
    } break;    
  } 
}

/*--------------------------------------------------------------------------------------*/







/*---------------------------------------STACKS-----------------------------------------*/
#define STACK_SIZE 4096
static long state_stack[STACK_SIZE];
static long stp = 0; // state pointer

static void state_push(long v) { assert(stp < STACK_SIZE); state_stack[stp++] = v; }
static long state_pop(void)    { assert(stp > 0);          return state_stack[--stp]; }
static long state_peek(void)   { assert(stp > 0);          return state_stack[stp - 1]; }

static Symbol symbols_stack[STACK_SIZE];
static long syp = 0; // symbol pointer

static void symbol_push_term(TokenList *t) { assert(syp < STACK_SIZE); symbols_stack[syp++] = (Symbol) {.symbol_type = t->type, (AstNode) { .type = 0, .lexeme = t->lexeme }};}
static void symbol_push_nonterm(Symbol s)  { assert(syp < STACK_SIZE); symbols_stack[syp++] = s;}
static Symbol symbol_pop(void)             { assert(syp > 0);          return symbols_stack[--syp]; }
static Symbol symbol_peek(void)            { assert(syp > 0);          return symbols_stack[syp - 1]; }

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
           (symbols_stack[i].symbol_type >= TKTYPE_NUM_TOKENS) ? var2str(symbols_stack[i].symbol_type)
                                                   : type2str(symbols_stack[i].symbol_type),
           (i == syp - 1) ? "" : ", ");
  }
  printf("}\t");

  printf("\e[175G");
  printf("INPUT = %s\n", type2str(t));
}
/*--------------------------------------------------------------------------------------*/

static void reduce_ast(Production *p);
static inline Symbol default_validation_get(Production *p);
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
      symbol_push_term(tklist);
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

#ifdef SLR_DEBUG
      reduce_ast(&p);
#else
      default_validation(&p);
#endif //SLR_DEBUG
      if ((gt_state = goto_table[state_peek()][p.target]) != 0) {
        state_push(gt_state);
      }
    }

    if (tklist == NULL) // terminou de consumir os tokens
      parse_state = PARSING_EOF;
  }

  if (symbol_peek().symbol_type == START_SYMBOL) {
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
  if (expected != symbol_pop().symbol_type) unexpected_symbol_err(expected);
  state_pop();
}

static inline Symbol consume_get_symbol(long expected){
  Symbol s = symbol_pop();
  if (expected != s.symbol_type)
    unexpected_symbol_err(expected);
  state_pop();
  return s;
}

static inline void default_validation(Production *p) {
  for (int i = p->num_elems - 1; i >= 0; i--) consume_symbol(p->elements[i]);
}

// consome toda a producao e retorna o ultimo elemento consumido
static inline Symbol default_validation_get(Production *p) {
  Symbol ret;
  for (int i = p->num_elems - 1; i >= 0; i--) {
    if (i == 0){
      ret = consume_get_symbol(p->elements[i]);
      continue;
    }
    consume_get_symbol(p->elements[i]);
  }
  return ret;
}

// Constroi a arvore de sintaxe abstrata na hora de fazer as reducoes.
static void reduce_ast(Production *p){
  switch (p->target) {
    case VAR_LOG_OR:{
      printf("TODO: VAR_LOGIC_OR\n");
      default_validation(p);
    }break;

    case VAR_LOG_AND:{
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
      AstNode *lhs, *rhs;
      lhs = malloc(sizeof(AstNode));
      rhs = malloc(sizeof(AstNode));

      *rhs = consume_get_symbol(VAR_EXPR6).node;
      consume_symbol(TKTYPE_MULT);
      *lhs = consume_get_symbol(VAR_EXPR5).node;

      Symbol result = {VAR_MULT, BUILD_BNOP(BNOP_MULT, lhs, rhs)};
      symbol_push_nonterm(result);
    }break;

    case VAR_DIV:{
      printf("TODO: VAR_DIV\n");
      default_validation(p);
    }break;

    case VAR_SUM:{
      AstNode *lhs, *rhs;
      lhs = malloc(sizeof(AstNode));
      rhs = malloc(sizeof(AstNode));

      *rhs = consume_get_symbol(VAR_EXPR5).node;
      consume_symbol(TKTYPE_SUM);
      *lhs = consume_get_symbol(VAR_EXPR4).node;

      Symbol result = {VAR_SUM, BUILD_BNOP(BNOP_SUM, lhs, rhs)};
      symbol_push_nonterm(result);
    }break;

    case VAR_SUB:{
      printf("TODO: VAR_SUB\n");
      default_validation(p);
    }break;

    case VAR_TIPO:{
      printf("TODO: VAR_TIPO\n");
      default_validation(p);
    }break;

    case VAR_PAREN_EXPR:{
      consume_symbol(TKTYPE_CLPAREN);
      Symbol s = consume_get_symbol(VAR_EXPR);
      consume_symbol(TKTYPE_OPPAREN);

      symbol_push_nonterm((Symbol) {.symbol_type = p->target, .node = s.node});
    } break;

    default: {
      // gambiarra
      Symbol last = default_validation_get(p);
      symbol_push_nonterm((Symbol) {.symbol_type = p->target, .node = last.node});
    } break;
  }
}


/*--------------------DEBUG ONLY--------------------*/
#ifdef SLR_DEBUG
int main() {
  printf("cur size of ast node: %d\n\n", sizeof(AstNode));

  char buf[BUFSIZ];
  fread(buf, sizeof(buf), sizeof(char), stdin);
  grammar_test(buf);
  AstNode end = symbol_pop().node;

  printf("\n--------------------------------------------------\n");
  printf("AST:\n\n");
  print_ast(&end);
  puts("");
  return 0;
}
#endif //SLR_DEBUG
