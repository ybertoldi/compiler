#include "../includes/tokenizer.h"
#include <stdbool.h>
#include <stdio.h>

/*
 *  COMANDO -> ATR | DEC 
 *  ATR -> id atr EXP pv
 *  EXP -> id | const
 *  DEC -> tipo id pv
 */

bool comando(TokenList **tkn);

int main(){
  char buf[4096] = {0};
  int c, bp = 0;
  printf("escreva algum comando para ser validado (CTRL-D para finalizar)\n");
  while ((c = getchar()) != EOF)
    buf[bp++] = c;
  
  TokenList *tkns = tokenize(buf);
  bool valido = comando(&tkns);
  printf("%s\n", valido ? "comando valido" : "comando invalido");

  return 0;
}
/*-----------------------CONSUMIR TOKENS---------------------*/
bool term(TokenList **tkn,  TKTYPE t){
  if (*tkn){
    bool ret = (*tkn)->type == t;  
    *(tkn) = (*tkn)->next;
    return ret;
  }
  return false;
}

bool term_tipo(TokenList **tkn) {
  if (!(*tkn)) return false;

  TKTYPE t = (*tkn)->type;
  *tkn = (*tkn)->next;

  switch (t) {
  case TKTYPE_T_INT:
  case TKTYPE_T_CHAR:
  case TKTYPE_T_BOOL:
  case TKTYPE_T_LONG:
  case TKTYPE_T_DOUBLE:
  case TKTYPE_T_FLOAT:
    return true;
  default:
    return false;
  }
}

bool term_const(TokenList **tkn){
  if (!(*tkn)) return false;

  TKTYPE t = (*tkn)->type;
  *tkn = (*tkn)->next;
  switch (t) {
    //TODO: adicionar string literal
    case TKTYPE_INT:
    case TKTYPE_FLOAT:
    case TKTYPE_HEXINT:
    case TKTYPE_OCTALINT:
      return true;
    default:
      return false;
  }
}

bool term_op(TokenList **tkn){
  if (!(*tkn)) return false;

  TKTYPE t = (*tkn)->type;
  *tkn = (*tkn)->next;
  switch (t) {
  case TKTYPE_SUM:
  case TKTYPE_SUB:
  case TKTYPE_DIV:
  case TKTYPE_MULT: 
  case TKTYPE_MOD:
  case TKTYPE_BITWISE_AND:
  case TKTYPE_BITWISE_OR:
  case TKTYPE_BITWISE_SHFTL:
  case TKTYPE_BITWISE_SHFTR:
  case TKTYPE_EQUALS:
  case TKTYPE_GT:
  case TKTYPE_GEQT:
  case TKTYPE_LT:
  case TKTYPE_LEQT:
  case TKTYPE_LOGIC_OR:
  case TKTYPE_LOGIC_AND:
  case TKTYPE_NEGATION: 
      return true;
  default:
      return false;
  }
}
/*-----------------------VALIDAR GRAMATICA---------------------*/
/*
 * COMANDO -> ATR | DEC
 * ATR -> id atr EXP pv | tipo id atr EXP pv
 * DEC -> tipo id pv
 *
 *
 * inicial: 
 *    EXP -> id | const | id op EXP | const op EXP
 */


bool atr(TokenList **tkn);
bool expr(TokenList **tkn);
bool dec(TokenList **tkn);
bool expr1(TokenList **tkn);
bool expr2(TokenList **tkn);
bool expr3(TokenList **tkn);

bool comando(TokenList **tkn){
  TokenList *eval = *tkn;
  if (atr(&eval)) {
    *tkn = eval;
    return true;
  } else return dec(tkn);
}

bool atr(TokenList **tkn) {
  TokenList *eval = *tkn;
  if (term_tipo(&eval)) *tkn = eval; // uma attribuicao pode ter definicao de tipo. TODO: verificar se a variavel ja foi declarada.

  return term(tkn, TKTYPE_VAR) 
    && term(tkn, TKTYPE_ATTRIBUTION) 
    && expr(tkn) 
    && term(tkn, TKTYPE_SMCOLON);
}

bool expr(TokenList **tkn){
  TokenList *eval;

// TODO: expandir a gramatica para expressoes. os trechos comentados geram loop de recursao
//  eval = *tkn;
//  if (expr3(&eval)) {
//    *tkn = eval;
//    return true;
//  }

  eval = *tkn;
  if (expr1(&eval)){
    *tkn = eval;
    return true;
  } 

  eval = *tkn;
  if (expr2(&eval)) {
    *tkn = eval;
    return true;
  }

  return false;
}

bool expr1(TokenList **tkn){
  return term(tkn, TKTYPE_ATTRIBUTION);
}

bool expr2(TokenList **tkn){
  return term_const(tkn);
}


// bool expr3(TokenList **tkn){
//   return expr(tkn) 
//     && term_op(tkn) 
//     && expr(tkn);
// }

bool dec(TokenList **tkn){
  return term_tipo(tkn) 
    && term(tkn, TKTYPE_VAR) 
    && term(tkn, TKTYPE_SMCOLON);
}



