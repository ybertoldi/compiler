#ifndef GRAMMAR_H
#define GRAMMAR_H

#include "tokenizer.h"


typedef struct {
  long target;
  long elements[256];
  long num_elems;
} Production;

#define START_SYMBOL VAR_FUNC_DEF
typedef enum {	VAR_FUNC_DEF = TKTYPE_NUM_TOKENS,
	VAR_FUNC_DEC,
	VAR_FUNC_DEC_ARGS,
	VAR_STMTS,
	VAR_STMT,
	VAR_WHILE,
	VAR_IF,
	VAR_IF_ELSE,
	VAR_ELSE,
	VAR_DEC,
	VAR_ASSIGN,
	VAR_FUNC_CALL,
	VAR_FUNC_CALL_ARGS,
	VAR_EXPR,
	VAR_LOG_OR,
	VAR_EXPR1,
	VAR_LOG_AND,
	VAR_EXPR2,
	VAR_CMP_EQ,
	VAR_CMP_NEQ,
	VAR_EXPR3,
	VAR_CMP_LT,
	VAR_CMP_LEQT,
	VAR_CMP_GT,
	VAR_CMP_GEQT,
	VAR_EXPR4,
	VAR_SUM,
	VAR_SUB,
	VAR_EXPR5,
	VAR_MULT,
	VAR_DIV,
	VAR_EXPR6,
	VAR_PAREN_EXPR,
	VAR_CONST,
	VAR_TIPO,
	VAR_NUM_VARS} VARTYPE;



char *var2str(VARTYPE t);

extern long goto_table[124][VAR_NUM_VARS];
extern Production rdc_table[124];
#endif // GRAMMAR_H