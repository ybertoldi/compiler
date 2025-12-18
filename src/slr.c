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
static inline void default_reduction(Production *p);

/*--------------------------------------------------------------------------------------*/
/*                                       ARENA                                          */
/*--------------------------------------------------------------------------------------*/
typedef struct NodeArena {
  AstNode *elems;
  size_t capacity;
  size_t count;
  struct NodeArena *next;
} NodeArena;

NodeArena *arena_head, *arena_cur;

static void node_arena_init(void){
  arena_head = malloc(sizeof(struct NodeArena));

  arena_head->capacity = 1024;
  arena_head->count = 0;
  arena_head->elems = calloc(arena_head->capacity, sizeof(AstNode));
  arena_head->next = NULL;
  
  arena_cur = arena_head;
  assert(arena_cur->elems != NULL);
}

void node_arena_free(void) {
  NodeArena *cur = arena_head;
  while (cur){
    NodeArena *tmp = cur;
    free(cur->elems);
    cur = cur->next;
    free(tmp);
  }
}

static inline AstNode *node_alloc(){
  if (arena_cur->count + 1 >= arena_cur->capacity){
    arena_cur->next = malloc(sizeof(struct NodeArena));
    arena_cur->next->capacity = 1024;
    arena_cur->next->count = 0;
    arena_cur->next->elems = calloc(1024, sizeof(struct AstNode));

    arena_cur = arena_cur->next;
    arena_cur->next = NULL;
  }
  return &arena_cur->elems[arena_cur->count++];
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
static bool symbol_peekable(void)          { return syp > 0;}

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

char *primtypestr(long primtype){
  switch (primtype) {
    case PTYPE_INT:    return "INT";
    case PTYPE_CHAR:   return "CHAR";
    case PTYPE_BOOL:   return "BOOL";
    case PTYPE_LONG:   return "LONG";
    case PTYPE_DOUBLE: return "DOUBLE";
    case PTYPE_FLOAT:  return "FLOAT";
  }
  return "UNKOWN_TYPE";
}

void print_ast_aux(AstNode *n, int stms_depth){
#define ident() for (int tmp = 0; tmp < stms_depth; tmp++) printf("  ")
#define ident_stmt() for (int tmp = 0; tmp < stms_depth + 1; tmp++) printf("  ")

  switch(n->type){
    case ASTYPE_TERMINAL: {
      fputs(n->lexeme, stdout);
    } break;

    case ASTYPE_BINOP: {
      printf("%s(", binopstr(n->binop_type));
      print_ast_aux(n->left, stms_depth);
      printf(",");
      print_ast_aux(n->right, stms_depth);
      printf(")");
    } break;    

    case ASTYPE_DECLARATION: {
      printf("DECLARATION(var_type=%s,varname=\"%s\",init_expr=", primtypestr(n->var_type), n->varname);
      print_ast_aux(n->init_expr, stms_depth);
      printf(")");
    } break;

    case ASTYPE_STMTS: {
      printf("STMTS(\n");
      for (int i = 0; i < n->stmt_arr.count; i++){
        ident_stmt();
        print_ast_aux(n->stmt_arr.elems[i], stms_depth+1);
        printf("%s\n", (i + 1 == n->stmt_arr.count)? "" : ",");
      }
      ident();
      printf(")\n");
    } break;

    case ASTYPE_WHILE: {
      printf("WHILE(\n");
      ident();
      printf("w_cond=");
      print_ast_aux(n->w_cond, stms_depth);
      printf(",\n");
      ident();
      printf("w_stmts=\n");
      ident_stmt();
      print_ast_aux(n->w_stmts, stms_depth+1);
      ident();
      printf(")");
    } break;

    case ASTYPE_ASSIGN:{
      printf("ASSIGN(assign_tgt=");
      print_ast_aux(n->assign_tgt, stms_depth);
      printf(",assign_expr=");
      print_ast_aux(n->assign_expr, stms_depth);
      printf(")");
    } break;

    case ASTYPE_IF: {
      printf("IF(if_cond=");
      print_ast_aux(n->if_cond, stms_depth);
      printf(",if_stmts=\n");
      ident_stmt();
      print_ast_aux(n->if_stmts,stms_depth + 1);
      if (n->else_stmts != NULL){
        printf(",else_stmts=\n");
        ident_stmt();
        print_ast_aux(n->else_stmts, stms_depth+1);
      }
      
      ident();
      printf(")");
    } break;

    case ASTYPE_FUNCALL: {
      printf("CALL(func_name=\"%s\",", n->func_name);
      for (int i = 0; i < n->args.count; i++){
        printf("arg%d=", i);
        print_ast_aux(n->args.elems[i], stms_depth);
        if (i + 1 < n->args.count) printf(",");
      }
      printf(")");
    } break;

    case ASTTYPE_LITERAL_TYPE: {
      assert(0 && "Unreachable");
    } break;

  } 
}

void print_ast(AstNode *n){
  print_ast_aux(n, 0);
}


/*--------------------------------------------------------------------------------------*/
/*                                   MAIN FUNCTIONS                                     */
/*--------------------------------------------------------------------------------------*/
void grammar_test(char *buf){
  node_arena_init();
  state_push(1);
  TokenList *tklist = tokenize(buf);
  enum {PARSING_TOKENS, PARSING_EOF} parse_state = PARSING_TOKENS;
  long gt_state;

  while (1) {
    if (parse_state == PARSING_TOKENS)
      printstate(tklist->type);
    else
      printstate(0);

    if (parse_state == PARSING_TOKENS && (gt_state = goto_table[state_peek()][tklist->type]) != 0) {
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
      default_reduction(&p);
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

static AstNode* consume_get_bnop(BinopType op, long expct1, long expct2, long expct3) {
  AstNode *lhs, *rhs, *ret;

  rhs = consume_get_symbol(expct3).node;
  consume_symbol(expct2);
  lhs = consume_get_symbol(expct1).node;

  ret = node_alloc();
  ret->type = ASTYPE_BINOP;
  ret->binop_type = op;
  ret->left = lhs;
  ret->right = rhs;

  return ret;
}

static inline void default_validation(Production *p) {
  for (int i = p->num_elems - 1; i >= 0; i--) consume_symbol(p->elements[i]);
}

static inline void default_reduction(Production *p) {
  for (int i = p->num_elems - 1; i >= 0; i--) consume_symbol(p->elements[i]);
  Symbol s = {p->target, NULL};
  symbol_push_nonterm(s);
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
      AstNode *binop_node = consume_get_bnop(BNOP_OR, VAR_EXPR, TKTYPE_LOGIC_OR, VAR_EXPR1);
      Symbol s = {VAR_LOG_OR, binop_node};
      symbol_push_nonterm(s);
    }break;

    case VAR_LOG_AND:{
      AstNode *binop_node = consume_get_bnop(BNOP_AND, VAR_EXPR1, TKTYPE_LOGIC_AND, VAR_EXPR2);
      Symbol s = {VAR_LOG_AND, binop_node};
      symbol_push_nonterm(s);
    }break;

    case VAR_CMP_EQ:{
      AstNode *binop_node = consume_get_bnop(BNOP_EQ, VAR_EXPR2, TKTYPE_EQUALS, VAR_EXPR3);
      Symbol s = {VAR_CMP_EQ, binop_node};
      symbol_push_nonterm(s);
    }break;

    case VAR_CMP_NEQ:{
      AstNode *binop_node = consume_get_bnop(BNOP_NEQ, VAR_EXPR2, TKTYPE_NEQUALS, VAR_EXPR3);
      Symbol s = {VAR_CMP_NEQ, binop_node};
      symbol_push_nonterm(s);
    }break;

    case VAR_CMP_LT:{
      AstNode *binop_node = consume_get_bnop(BNOP_LT, VAR_EXPR3, TKTYPE_LT, VAR_EXPR4);
      Symbol s = {VAR_CMP_LT, binop_node};
      symbol_push_nonterm(s);
    }break;

    case VAR_CMP_LEQT:{
      AstNode *binop_node = consume_get_bnop(BNOP_LEQT, VAR_EXPR3, TKTYPE_LEQT, VAR_EXPR4);
      Symbol s = {VAR_CMP_LEQT, binop_node};
      symbol_push_nonterm(s);
    }break;

    case VAR_CMP_GT:{
      AstNode *binop_node = consume_get_bnop(BNOP_GT, VAR_EXPR3, TKTYPE_GT, VAR_EXPR4);
      Symbol s = {VAR_CMP_GT, binop_node};
      symbol_push_nonterm(s);
    }break;

    case VAR_CMP_GEQT:{
      AstNode *binop_node = consume_get_bnop(BNOP_GEQT, VAR_EXPR3, TKTYPE_GEQT, VAR_EXPR4);
      Symbol s = {VAR_CMP_GEQT, binop_node};
      symbol_push_nonterm(s);
    }break;

    case VAR_MULT:{
      AstNode *binop_node = consume_get_bnop(BNOP_MULT, VAR_EXPR5, TKTYPE_MULT, VAR_EXPR6);
      Symbol s = {VAR_MULT, binop_node};
      symbol_push_nonterm(s);
    } break;

    case VAR_DIV:{
      AstNode *binop_node = consume_get_bnop(BNOP_DIV, VAR_EXPR5, TKTYPE_DIV, VAR_EXPR6);
      Symbol s = {VAR_DIV, binop_node};
      symbol_push_nonterm(s);
    }break;

    case VAR_SUM:{
      AstNode *binop_node = consume_get_bnop(BNOP_SUM, VAR_EXPR4, TKTYPE_SUM, VAR_EXPR5);
      Symbol s = {VAR_SUM, binop_node};
      symbol_push_nonterm(s);
    } break;

    case VAR_SUB:{
      AstNode *binop_node = consume_get_bnop(BNOP_SUB, VAR_EXPR4, TKTYPE_SUB, VAR_EXPR5);
      Symbol s = {VAR_SUB, binop_node};
      symbol_push_nonterm(s);
    }break;

    case VAR_PAREN_EXPR:{
      Symbol s;
      consume_symbol(TKTYPE_CLPAREN);
      s = consume_get_symbol(VAR_EXPR);
      consume_symbol(TKTYPE_OPPAREN);

      s.symbol_type = VAR_PAREN_EXPR;
      symbol_push_nonterm(s);
    } break;

    case VAR_TIPO:{
      Symbol s = consume_get_symbol(p->elements[0]);

      s.node->type = ASTTYPE_LITERAL_TYPE;
      s.node->literal_type = s.symbol_type;
      s.symbol_type = VAR_TIPO;

      symbol_push_nonterm(s);
    }break;

    case VAR_DEC: {
      AstNode *dec = node_alloc();
      dec->type = ASTYPE_DECLARATION;

      consume_symbol(TKTYPE_SMCOLON);
      dec->init_expr = consume_get_symbol(VAR_EXPR).node;
      consume_symbol(TKTYPE_ATTRIBUTION); 
      dec->varname = consume_get_symbol(TKTYPE_VAR).node->lexeme; 
      dec->var_type = consume_get_symbol(VAR_TIPO).node->literal_type;
      
      Symbol s = {VAR_DEC, dec};
      symbol_push_nonterm(s);
    } break;

    case VAR_STMTS: {
      Symbol s = consume_get_symbol(VAR_STMT);
      if (symbol_peekable() && symbol_peek().symbol_type == VAR_STMTS){
        Symbol prev = symbol_pop();
        DA_APPEND(prev.node->stmt_arr, s.node);
        symbol_push_nonterm(prev);
      } else {
        AstNode *n = node_alloc();
        n->type = ASTYPE_STMTS;

        n->stmt_arr.capacity = 16;
        n->stmt_arr.elems = calloc(16, sizeof(void *));
        n->stmt_arr.count = 0;
        DA_APPEND(n->stmt_arr, s.node);

        s.node = n;
        s.symbol_type = VAR_STMTS;
        symbol_push_nonterm(s);
      }
    } break;
    
    case VAR_WHILE: {
      AstNode *n = node_alloc();
      n->type = ASTYPE_WHILE;

      consume_symbol(TKTYPE_CLBRACKET);
      n->w_stmts = consume_get_symbol(VAR_STMTS).node;
      consume_symbol(TKTYPE_OPBRACKET);
      consume_symbol(TKTYPE_CLPAREN);
      n->w_cond = consume_get_symbol(VAR_EXPR).node;
      consume_symbol(TKTYPE_OPPAREN);
      consume_symbol(TKTYPE_KW_WHILE);

      Symbol s = {VAR_WHILE, n};
      symbol_push_nonterm(s);
    } break;


    case VAR_ASSIGN: {
      AstNode *n = node_alloc();
      n->type = ASTYPE_ASSIGN;
      
      consume_symbol(TKTYPE_SMCOLON);
      n->assign_expr = consume_get_symbol(VAR_EXPR).node;
      consume_symbol(TKTYPE_ATTRIBUTION);
      n->assign_tgt = consume_get_symbol(TKTYPE_VAR).node;

      Symbol s = {VAR_ASSIGN, n};
      symbol_push_nonterm(s);
    } break;

    case VAR_IF: {
      AstNode *n = node_alloc();
      n->type = ASTYPE_IF;
      consume_symbol(TKTYPE_CLBRACKET);
      n->if_stmts = consume_get_symbol(VAR_STMTS).node;
      consume_symbol(TKTYPE_OPBRACKET);
      consume_symbol(TKTYPE_CLPAREN);
      n->if_cond = consume_get_symbol(VAR_EXPR).node;
      consume_symbol(TKTYPE_OPPAREN);
      consume_symbol(TKTYPE_KW_IF);
      n->else_stmts = NULL;

      Symbol s = {VAR_IF, n};
      symbol_push_nonterm(s);
    } break;

    case VAR_FUNC_CALL: {
      AstNode *n;
      consume_symbol(TKTYPE_CLPAREN);
      n = consume_get_symbol(VAR_ARGS).node;
      consume_symbol(TKTYPE_OPPAREN);
      n->func_name = consume_get_symbol(TKTYPE_VAR).node->lexeme;

      n->type = ASTYPE_FUNCALL;
      Symbol s = {VAR_FUNC_CALL, n};
      symbol_push_nonterm(s);
    } break;
  
    case VAR_ARGS: {
      // <ARGS> ::= <EXPR>;
      if (p->num_elems == 1){ 
        Symbol s = consume_get_symbol(VAR_EXPR);

        AstNode *n = node_alloc();        
        n->type = ASTYPE_FUNCARGS;
        n->args.elems = calloc(16, sizeof(AstNode*));
        n->args.count = 1;
        n->args.capacity = 16;
        n->args.elems[0] = s.node;

        s.node = n;
        s.symbol_type = VAR_ARGS;
        symbol_push_nonterm(s);
      } 
      // <ARGS> ::= <ARGS> TKTYPE_COMMA <EXPR>;
      else {
        AstNode *expr = consume_get_symbol(VAR_EXPR).node;
        consume_symbol(TKTYPE_COMMA);
        Symbol s = consume_get_symbol(VAR_ARGS);

        DA_APPEND(s.node->args, expr);
        symbol_push_nonterm(s);
      }
    } break;
    
    default: {
      // gambiarra
      Symbol s = default_validation_get(p);
      s.symbol_type = p->target;
      symbol_push_nonterm(s);
    } break;
  }
}


/*--------------------DEBUG ONLY--------------------*/
#ifdef SLR_DEBUG
int main() {
  char buf[BUFSIZ];
  fread(buf, sizeof(buf), sizeof(char), stdin);

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
