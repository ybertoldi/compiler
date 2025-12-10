#ifndef GRAMMAR_H
#define GRAMMAR_H

#include "tokenizer.h"


typedef struct {
  long target;
  long elements[256];
  long num_elems;
} Production;

#define START_SYMBOL VAR_DEC
typedef enum {	VAR_DEC = TKTYPE_NUM_TOKENS,
	VAR_EXPR,
	VAR_EXPR1,
	VAR_EXPR2,
	VAR_EXPR3,
	VAR_EXPR4,
	VAR_EXPR5,
	VAR_EXPR6,
	VAR_COND1,
	VAR_COND2,
	VAR_REL1,
	VAR_REL2,
	VAR_OP1,
	VAR_OP2,
	VAR_CONST,
	VAR_TIPO,
	VAR_NUM_VARS} VARTYPE;



extern char *var2str(VARTYPE t);

extern long goto_table[54][VAR_NUM_VARS];
extern Production reduce[54];
#endif // GRAMMAR_H
