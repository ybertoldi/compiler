#ifndef AST_H
#define AST_H
#include "tokenizer.h"
#include "dynamic_array.h"
#include <stddef.h>
#include <stdint.h>

typedef enum {
  ASTYPE_TERMINAL, 
  ASTYPE_BINOP,
  ASTTYPE_LITERAL_TYPE,
  ASTYPE_DECLARATION,
  ASTYPE_STMTS,
  ASTYPE_WHILE,
  ASTYPE_ASSIGN,
  ASTYPE_IF,
  ASTYPE_FUNCALL1,
  ASTYPE_FUNCALL2
} AstNodeType;


typedef enum{
  PTYPE_INT = TKTYPE_T_INT,
  PTYPE_CHAR,
  PTYPE_BOOL,
  PTYPE_LONG,
  PTYPE_DOUBLE,
  PTYPE_FLOAT,
} PrimType;

typedef enum {
  BNOP_SUM,
  BNOP_SUB,
  BNOP_MULT,
  BNOP_DIV,
  BNOP_MOD,

  BNOP_SHL,
  BNOP_SHR,
  BNOP_BW_OR,
  BNOP_BW_AND,
  BNOP_BW_XOR,
  
  BNOP_OR,
  BNOP_AND,

  BNOP_EQ,
  BNOP_NEQ,
  BNOP_GT,
  BNOP_GEQT,
  BNOP_LT,
  BNOP_LEQT,
} BinopType ;


typedef struct AstNode {
  AstNodeType type;

  union {
/*---nós primitivos---*/
    char *lexeme; // terminal
    char *sval;   // string
    long ival;    // inteiro
    struct {
      double fval;
      char *frepr;
    };  // float ou double
    
/*---nós derivados---*/
    PrimType literal_type; // tipo literal(char,double,long, etc.)
    
    struct {
      BinopType binop_type;
      struct AstNode *left;
      struct AstNode *right;
    }; // operacao binaria

    struct {
      PrimType var_type;
      char *varname;
      struct AstNode *init_expr;
    }; // declaracao
  
    struct{
      struct AstNode **elems;
      size_t           count;
      size_t           capacity;
    } stmt_arr; // array de stmts

    struct {
      struct AstNode *w_cond;
      struct AstNode *w_stmts;
    }; // while
    
    struct {
      struct AstNode *assign_tgt;
      struct AstNode *assign_expr;
    }; // atribuicao

    struct {
      struct AstNode *if_cond;
      struct AstNode *if_stmts;
      struct AstNode *else_stmts;
    }; // if-else
    
    struct{
      char *func_name;
      struct AstNode* arg1;
      struct AstNode* arg2;
    }; // func-call
    
  };
} AstNode ;

extern void grammar_test(char *);
#endif // AST_H
