#include <stddef.h>
#define TOKENIZER_H_DEBUG
#include "../includes/tokenizer.h"
#include "../includes/grammar.h"
#include "../includes/ast.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct Symbol {
  long symbol_type;
  AstNode *node;
} Symbol ;

static void reduce_ast(Production *p);
static inline Symbol default_validation_get(Production *p); static inline void default_validation(Production *p);
static inline AstNode *node_alloc();
static inline void consume_symbol(long expected);
static inline Symbol consume_get_symbol(long expected);
static inline void default_validation(Production *p) ;
static inline void unexpected_symbol_err(long expected) ;

/*--------------------------------------------------------------------------------------*/
/*                                       ARENA                                          */
/*--------------------------------------------------------------------------------------*/
// TODO: quando isso passar por um realloc, todas as referencias aos enderecos de memoria anteriores
// vao apontar para lixo. Salvar informacoes sobre os ponteiros para resolver este problema.
// por enquanto estou pre-alocando uma quantidade grande de memoria para nunca precisar fazer um realloc,
// mas no futuro isso dara problemas
struct {
  AstNode *elems;
  size_t capacity;
  size_t count;
} node_arena;

static void node_arena_init(void){
  node_arena.capacity = 1024;
  node_arena.count = 0;
  node_arena.elems = calloc(node_arena.capacity, sizeof(AstNode));

  assert(node_arena.elems != NULL);
}

void node_arena_free(void) { free(node_arena.elems); }

static inline AstNode *node_alloc(){
  assert(node_arena.count + 1 < node_arena.capacity); // realocacao da arena nao implementada
  return &node_arena.elems[node_arena.count++];
}

/*--------------------------------------------------------------------------------------*/
/*                                      STACKS                                          */
/*--------------------------------------------------------------------------------------*/
#define STACK_SIZE 4096
static long state_stack[STACK_SIZE];
static long stp = 0; // state pointer

static void state_push(long v) { assert(stp < STACK_SIZE); state_stack[stp++] = v; }
static long state_pop(void)    { assert(stp > 0);          return state_stack[--stp]; }
static long state_peek(void)   { assert(stp > 0);          return state_stack[stp - 1]; }

static Symbol symbols_stack[STACK_SIZE];
static long syp = 0; // symbol pointer

static void symbol_push_term(TokenList *t) {
  assert(syp < STACK_SIZE);
  AstNode *n = node_alloc();
  n->type = 0;
  n->lexeme = t->lexeme;
  symbols_stack[syp++] = (Symbol){t->type, n};
}
static void symbol_push_nonterm(Symbol s)  { assert(syp < STACK_SIZE); symbols_stack[syp++] = s; }
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
/*                                   AST GENERATION                                     */
/*--------------------------------------------------------------------------------------*/
/*------------binops------------*/
static AstNode* build_bnop(BINOPTYPE op, long exp1, long exp2, long exp3) {
  AstNode *lhs, *rhs;

  rhs = consume_get_symbol(exp3).node;
  consume_symbol(exp2);
  lhs = consume_get_symbol(exp1).node;

  AstNode *built_bnop = node_alloc();
  built_bnop->type = ASTYPE_BINOP;
  built_bnop->binop_type = op;
  built_bnop->left = lhs;
  built_bnop->right = rhs;

  return built_bnop;
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
/*                                   MAIN FUNCTIONS                                     */
/*--------------------------------------------------------------------------------------*/
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
      AstNode *binop_node = build_bnop(BNOP_OR, VAR_EXPR, TKTYPE_LOGIC_OR, VAR_EXPR1);
      Symbol s = {VAR_LOG_OR, binop_node};
      symbol_push_nonterm(s);
    }break;

    case VAR_LOG_AND:{
      AstNode *binop_node = build_bnop(BNOP_AND, VAR_EXPR1, TKTYPE_LOGIC_AND, VAR_EXPR2);
      Symbol s = {VAR_LOG_AND, binop_node};
      symbol_push_nonterm(s);
    }break;

    case VAR_CMP_EQ:{
      AstNode *binop_node = build_bnop(BNOP_EQ, VAR_EXPR2, TKTYPE_EQUALS, VAR_EXPR3);
      Symbol s = {VAR_CMP_EQ, binop_node};
      symbol_push_nonterm(s);
    }break;

    case VAR_CMP_NEQ:{
      AstNode *binop_node = build_bnop(BNOP_NEQ, VAR_EXPR2, TKTYPE_NEQUALS, VAR_EXPR3);
      Symbol s = {VAR_CMP_NEQ, binop_node};
      symbol_push_nonterm(s);
    }break;

    case VAR_CMP_LT:{
      AstNode *binop_node = build_bnop(BNOP_LT, VAR_EXPR3, TKTYPE_LT, VAR_EXPR4);
      Symbol s = {VAR_CMP_LT, binop_node};
      symbol_push_nonterm(s);
    }break;

    case VAR_CMP_LEQT:{
      AstNode *binop_node = build_bnop(BNOP_LEQT, VAR_EXPR3, TKTYPE_LEQT, VAR_EXPR4);
      Symbol s = {VAR_CMP_LEQT, binop_node};
      symbol_push_nonterm(s);
    }break;

    case VAR_CMP_GT:{
      AstNode *binop_node = build_bnop(BNOP_GT, VAR_EXPR3, TKTYPE_GT, VAR_EXPR4);
      Symbol s = {VAR_CMP_GT, binop_node};
      symbol_push_nonterm(s);
    }break;

    case VAR_CMP_GEQT:{
      AstNode *binop_node = build_bnop(BNOP_GEQT, VAR_EXPR3, TKTYPE_GEQT, VAR_EXPR4);
      Symbol s = {VAR_CMP_GEQT, binop_node};
      symbol_push_nonterm(s);
    }break;

    case VAR_MULT:{
      AstNode *binop_node = build_bnop(BNOP_MULT, VAR_EXPR5, TKTYPE_MULT, VAR_EXPR6);
      Symbol s = {VAR_MULT, binop_node};
      symbol_push_nonterm(s);
    } break;

    case VAR_DIV:{
      AstNode *binop_node = build_bnop(BNOP_DIV, VAR_EXPR5, TKTYPE_DIV, VAR_EXPR6);
      Symbol s = {VAR_DIV, binop_node};
      symbol_push_nonterm(s);
    }break;

    case VAR_SUM:{
      AstNode *binop_node = build_bnop(BNOP_SUM, VAR_EXPR4, TKTYPE_SUM, VAR_EXPR5);
      Symbol s = {VAR_SUM, binop_node};
      symbol_push_nonterm(s);
    } break;

    case VAR_SUB:{
      AstNode *binop_node = build_bnop(BNOP_SUB, VAR_EXPR4, TKTYPE_SUB, VAR_EXPR5);
      Symbol s = {VAR_SUB, binop_node};
      symbol_push_nonterm(s);
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
  char buf[BUFSIZ];
  fread(buf, sizeof(buf), sizeof(char), stdin);

  node_arena_init();
  grammar_test(buf);

  printf("\n--------------------------------------------------\n");
  printf("AST:\n\n");

  AstNode *end = symbol_pop().node;
  print_ast(end);
  puts("");

  node_arena_free();
  return 0;
}
#endif //SLR_DEBUG
