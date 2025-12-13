#define TKL_D
#include "../includes/dfa_node.h"

#ifndef TOKENIZER_H
#define TOKENIZER_H 

typedef  enum {
  TKTYPE_INIT = 0,
  TKTYPE_WTSP,

  //SEPARADORES
  TKTYPE_OPPAREN,
  TKTYPE_CLPAREN,
  TKTYPE_OPBRACKET,
  TKTYPE_CLBRACKET,
  TKTYPE_OPSQRBRACK,
  TKTYPE_CLSQRBRACK,
  TKTYPE_COMMA,
  TKTYPE_SMCOLON,
  TKTYPE_SINGLE_QUOTE,
  TKTYPE_DOUBLE_QUOTE,

  //OPERADORES ARITIMETICOS
  TKTYPE_SUM,
  TKTYPE_SUB,
  TKTYPE_DIV,
  TKTYPE_MULT, // mesmo token para derefenciar um ponteiro
  TKTYPE_MOD,
  TKTYPE_BITWISE_AND, // mesmo token para pegar o endereco de memoria de uma variavel (&)
  TKTYPE_BITWISE_OR,
  TKTYPE_BITWISE_SHFTL,
  TKTYPE_BITWISE_SHFTR,

  //OPERADORES LOGICOS
  TKTYPE_EQUALS,
  TKTYPE_NEQUALS,
  TKTYPE_GT,
  TKTYPE_GEQT,
  TKTYPE_LT,
  TKTYPE_LEQT,
  TKTYPE_LOGIC_OR,
  TKTYPE_LOGIC_AND,
  TKTYPE_NEGATION, //negacao em bitwise e negacao logica sao iguais
                   
  //VARIAVEIS E NUMS
  TKTYPE_VAR,
  TKTYPE_ATTRIBUTION,
  TKTYPE_INT,
  TKTYPE_FLOAT,
  TKTYPE_HEXINT,
  TKTYPE_OCTALINT,

  //PALAVRAS RESERVADAS
  TKTYPE_IF,
  TKTYPE_ELSE,
  TKTYPE_FOR,
  TKTYPE_WHILE,
  TKTYPE_SWITCH,
  TKTYPE_CASE,
  TKTYPE_FALSE,
  TKTYPE_TRUE,

  //PALAVRAS RESERVADAS, TIPOS
  TKTYPE_T_INT,
  TKTYPE_T_CHAR,
  TKTYPE_T_BOOL,
  TKTYPE_T_LONG,
  TKTYPE_T_DOUBLE,
  TKTYPE_T_FLOAT,

  // STRING e CHAR
  TKTYPE_STRLIT,

  // sentinela para contar o numero de tokens
  TKTYPE_NUM_TOKENS
}TKTYPE;


// STRUCTS
typedef struct tk{
  char *lexeme;
  TKTYPE type;
  struct tk *next;
} TokenList;


// FUNCOES
TokenList *tokenize(char *eval);
char *type2str(TKTYPE t);

// macros para printar
#ifdef TOKENIZER_H_DEBUG
  #define TKL_FMT "(%s, %s)"
  #define TKL_ARGS(tkl) type2str(tkl->type), tkl->lexeme
#endif // TOKENIZER_H_DEBUG

#endif //TOKENIZER_H
