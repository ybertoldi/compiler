#include "../includes/dfa_node.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


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

  //OPERADORES ARITIMETICOS
  TKTYPE_SUM,
  TKTYPE_SUB,
  TKTYPE_DIV,
  TKTYPE_MULT, // mesmo token para derefenciar um ponteiro
  TKTYPE_MOD,
  TKTYPE_BITWISE_AND, // mesmo token para pegar o endereco de memoria de uma variavel
  TKTYPE_BITWISE_OR,
  TKTYPE_BITWISE_SHFTL,
  TKTYPE_BITWISE_SHFTR,

  //OPERADORES LOGICOS
  TKTYPE_EQUALS,
  TKTYPE_GT,
  TKTYPE_GEQT,
  TKTYPE_LT,
  TKTYPE_LEQT,
  TKTYPE_LOGIC_OR,
  TKTYPE_LOGIC_AND,
  TKTYPE_NEGATION, //negacao em bitwise e negacao logica sao iguais
                   //
  //VARIAVEIS E NUMS
  TKTYPE_VAR,
  TKTYPE_ATTRIBUTE,
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
  TKTYPE_T_FLOAT

}TKTYPE;



typedef struct tk{
  char *lexeme;
  TKTYPE type;
  struct tk *next;
} TokenList;

char *print_type(TKTYPE t) {
  switch (t) {
  case TKTYPE_INIT:
    return "TKTYPE_INIT";
  case TKTYPE_WTSP:
    return "TKTYPE_WTSP";
  case TKTYPE_OPPAREN:
    return "TKTYPE_OPPAREN";
  case TKTYPE_CLPAREN:
    return "TKTYPE_CLPAREN";
  case TKTYPE_OPBRACKET:
    return "TKTYPE_OPBRACKET";
  case TKTYPE_CLBRACKET:
    return "TKTYPE_CLBRACKET";
  case TKTYPE_OPSQRBRACK:
    return "TKTYPE_OPSQRBRACK";
  case TKTYPE_CLSQRBRACK:
    return "TKTYPE_CLSQRBRACK";
  case TKTYPE_COMMA:
    return "TKTYPE_COMMA";
  case TKTYPE_SMCOLON:
    return "TKTYPE_SMCOLON";
  case TKTYPE_SUM:
    return "TKTYPE_SUM";
  case TKTYPE_SUB:
    return "TKTYPE_SUB";
  case TKTYPE_DIV:
    return "TKTYPE_DIV";
  case TKTYPE_MULT:
    return "TKTYPE_MULT";
  case TKTYPE_MOD:
    return "TKTYPE_MOD";
  case TKTYPE_BITWISE_AND:
    return "TKTYPE_BITWISE_AND";
  case TKTYPE_BITWISE_OR:
    return "TKTYPE_BITWISE_OR";
  case TKTYPE_BITWISE_SHFTL:
    return "TKTYPE_BITWISE_SHFTL";
  case TKTYPE_BITWISE_SHFTR:
    return "TKTYPE_BITWISE_SHFTR";
  case TKTYPE_EQUALS:
    return "TKTYPE_EQUALS";
  case TKTYPE_GT:
    return "TKTYPE_GT";
  case TKTYPE_GEQT:
    return "TKTYPE_GEQT";
  case TKTYPE_LT:
    return "TKTYPE_LT";
  case TKTYPE_LEQT:
    return "TKTYPE_LEQT";
  case TKTYPE_LOGIC_OR:
    return "TKTYPE_LOGIC_OR";
  case TKTYPE_LOGIC_AND:
    return "TKTYPE_LOGIC_AND";
  case TKTYPE_NEGATION:
    return "TKTYPE_NEGATION";
  case TKTYPE_VAR:
    return "TKTYPE_VAR";
  case TKTYPE_ATTRIBUTE:
    return "TKTYPE_ATTRIBUTE";
  case TKTYPE_INT:
    return "TKTYPE_INT";
  case TKTYPE_FLOAT:
    return "TKTYPE_FLOAT";
  case TKTYPE_HEXINT:
    return "TKTYPE_HEXINT";
  case TKTYPE_OCTALINT:
    return "TKTYPE_OCTALINT";
  case TKTYPE_IF:
    return "TKTYPE_IF";
  case TKTYPE_ELSE:
    return "TKTYPE_ELSE";
  case TKTYPE_FOR:
    return "TKTYPE_FOR";
  case TKTYPE_WHILE:
    return "TKTYPE_WHILE";
  case TKTYPE_SWITCH:
    return "TKTYPE_SWITCH";
  case TKTYPE_CASE:
    return "TKTYPE_CASE";
  case TKTYPE_FALSE:
    return "TKTYPE_FALSE";
  case TKTYPE_TRUE:
    return "TKTYPE_TRUE";
  case TKTYPE_T_INT:
    return "TKTYPE_T_INT";
  case TKTYPE_T_CHAR:
    return "TKTYPE_T_CHAR";
  case TKTYPE_T_BOOL:
    return "TKTYPE_T_BOOL";
  case TKTYPE_T_LONG:
    return "TKTYPE_T_LONG";
  case TKTYPE_T_DOUBLE:
    return "TKTYPE_T_DOUBLE";
  case TKTYPE_T_FLOAT:
    return "TKTYPE_T_FLOAT";
  }
}

TokenList* tklalloc(char *lexeme, TKTYPE type){
  TokenList *new = malloc(sizeof(TokenList));
  new->lexeme = lexeme;
  new->type = type;
  new->next = NULL;
  return new;
}

void tklappend(TokenList *head, TokenList *val){
  TokenList *ant;
  while (head) {
    ant = head;
    head = head->next;
  }
  ant->next = val;
}

TokenList *tokenize(char *eval, DfaNode *start){
  int i = 0;
  TokenList *curlist = NULL, *head;
  DfaNode *curnode = start;

  while (eval[i]) {
    int tknstart = i, n = 0;
    while (dfa_node_move(&curnode, eval[i])) {
      i++;
      n++;
    }

    if (curlist) {
      tklappend(curlist, tklalloc(strndup(&eval[tknstart], n), curnode->type));
    } else {
      curlist =
          tklalloc(strndup(&eval[tknstart], n), curnode->type);
      head = curlist;
    }

    curnode = start;
  }

  return head;
}

void dfa_add_delta_orelse(DfaNode *src, char c, DfaNode *target, DfaNode *other){
  char otherexpr[16];
  dfa_add_delta(src, c, target);
  sprintf(otherexpr, "_[A-Za-z0-9]^%c", c);
  dfa_add_delta_expr(src, otherexpr, other);
}


int main(){
/*-----------------------DEFINICAO DOS ESTADOS-----------------------*/
  DfaNode q0 = BUILD_DFA_NODE(TKTYPE_INIT);
  DfaNode q1 = BUILD_DFA_NODE_FINAL(TKTYPE_WTSP);

  DfaNode q2 = BUILD_DFA_NODE_FINAL(TKTYPE_OPPAREN);
  DfaNode q3 = BUILD_DFA_NODE_FINAL(TKTYPE_CLPAREN);
  DfaNode q4 = BUILD_DFA_NODE_FINAL(TKTYPE_OPBRACKET);
  DfaNode q5 = BUILD_DFA_NODE_FINAL(TKTYPE_CLBRACKET);
  DfaNode q6 = BUILD_DFA_NODE_FINAL(TKTYPE_OPSQRBRACK);
  DfaNode q7 = BUILD_DFA_NODE_FINAL(TKTYPE_CLSQRBRACK);
  DfaNode q8 = BUILD_DFA_NODE_FINAL(TKTYPE_COMMA);
  DfaNode q9 = BUILD_DFA_NODE_FINAL(TKTYPE_SMCOLON);
  
  DfaNode q10 = BUILD_DFA_NODE_FINAL(TKTYPE_SUM);
  DfaNode q11 = BUILD_DFA_NODE_FINAL(TKTYPE_SUB);
  DfaNode q12 = BUILD_DFA_NODE_FINAL(TKTYPE_DIV);
  DfaNode q13 = BUILD_DFA_NODE_FINAL(TKTYPE_MULT);
  DfaNode q14 = BUILD_DFA_NODE_FINAL(TKTYPE_MOD);
  DfaNode q15 = BUILD_DFA_NODE_FINAL(TKTYPE_BITWISE_AND);
  DfaNode q16 = BUILD_DFA_NODE_FINAL(TKTYPE_BITWISE_OR);
  DfaNode q17 = BUILD_DFA_NODE_FINAL(TKTYPE_BITWISE_SHFTL);
  DfaNode q18 = BUILD_DFA_NODE_FINAL(TKTYPE_BITWISE_SHFTR);

  DfaNode q19 = BUILD_DFA_NODE_FINAL(TKTYPE_EQUALS);
  DfaNode q20 = BUILD_DFA_NODE_FINAL(TKTYPE_GT);
  DfaNode q21 = BUILD_DFA_NODE_FINAL(TKTYPE_GEQT);
  DfaNode q22 = BUILD_DFA_NODE_FINAL(TKTYPE_LT);
  DfaNode q23 = BUILD_DFA_NODE_FINAL(TKTYPE_LEQT);
  DfaNode q24 = BUILD_DFA_NODE_FINAL(TKTYPE_LOGIC_OR);
  DfaNode q25 = BUILD_DFA_NODE_FINAL(TKTYPE_LOGIC_AND);

  DfaNode q26 = BUILD_DFA_NODE_FINAL(TKTYPE_VAR);
  DfaNode q27 = BUILD_DFA_NODE_FINAL(TKTYPE_ATTRIBUTE);

  DfaNode q28 = BUILD_DFA_NODE_FINAL(TKTYPE_INT); //0 (pode comecar um octal o hexa)
  DfaNode q29 = BUILD_DFA_NODE_FINAL(TKTYPE_INT);
  DfaNode q30 = BUILD_DFA_NODE_FINAL(TKTYPE_FLOAT);
  DfaNode q31 = BUILD_DFA_NODE_FINAL(TKTYPE_HEXINT);
  DfaNode q32 = BUILD_DFA_NODE_FINAL(TKTYPE_OCTALINT);

  
  DfaNode q33 = BUILD_DFA_NODE_FINAL(TKTYPE_VAR); //i
  DfaNode q34 = BUILD_DFA_NODE_FINAL(TKTYPE_IF);  //if

  DfaNode q35 = BUILD_DFA_NODE_FINAL(TKTYPE_VAR);  //e
  DfaNode q36 = BUILD_DFA_NODE_FINAL(TKTYPE_VAR);  //el
  DfaNode q37 = BUILD_DFA_NODE_FINAL(TKTYPE_VAR);  //els
  DfaNode q38 = BUILD_DFA_NODE_FINAL(TKTYPE_ELSE); //else

  DfaNode q39 = BUILD_DFA_NODE_FINAL(TKTYPE_VAR);  //f
  DfaNode q40 = BUILD_DFA_NODE_FINAL(TKTYPE_VAR);  //fo
  DfaNode q41 = BUILD_DFA_NODE_FINAL(TKTYPE_FOR);  //for

  DfaNode q42 = BUILD_DFA_NODE_FINAL(TKTYPE_VAR);  //w
  DfaNode q43 = BUILD_DFA_NODE_FINAL(TKTYPE_VAR);  //wh
  DfaNode q44 = BUILD_DFA_NODE_FINAL(TKTYPE_VAR);  //whi
  DfaNode q45 = BUILD_DFA_NODE_FINAL(TKTYPE_VAR);  //whil
  DfaNode q46 = BUILD_DFA_NODE_FINAL(TKTYPE_VAR);  //while

  DfaNode q47 = BUILD_DFA_NODE_FINAL(TKTYPE_VAR);     //s
  DfaNode q48 = BUILD_DFA_NODE_FINAL(TKTYPE_VAR);     //sw
  DfaNode q49 = BUILD_DFA_NODE_FINAL(TKTYPE_VAR);     //swi
  DfaNode q50 = BUILD_DFA_NODE_FINAL(TKTYPE_VAR);     //swit
  DfaNode q51 = BUILD_DFA_NODE_FINAL(TKTYPE_VAR);     //switc
  DfaNode q52 = BUILD_DFA_NODE_FINAL(TKTYPE_SWITCH);  //switch

  DfaNode q53 = BUILD_DFA_NODE_FINAL(TKTYPE_VAR);   //c
  DfaNode q54 = BUILD_DFA_NODE_FINAL(TKTYPE_VAR);   //ca
  DfaNode q55 = BUILD_DFA_NODE_FINAL(TKTYPE_VAR);   //cas
  DfaNode q56 = BUILD_DFA_NODE_FINAL(TKTYPE_CASE);  //case

  DfaNode q57 = BUILD_DFA_NODE_FINAL(TKTYPE_VAR);     //fa (f do for)
  DfaNode q58 = BUILD_DFA_NODE_FINAL(TKTYPE_VAR);     //fal
  DfaNode q59 = BUILD_DFA_NODE_FINAL(TKTYPE_VAR);     //fals
  DfaNode q60 = BUILD_DFA_NODE_FINAL(TKTYPE_FALSE);   //false

  DfaNode q61 = BUILD_DFA_NODE_FINAL(TKTYPE_VAR);     //t
  DfaNode q62 = BUILD_DFA_NODE_FINAL(TKTYPE_VAR);     //tr
  DfaNode q63 = BUILD_DFA_NODE_FINAL(TKTYPE_VAR);     //tru
  DfaNode q64 = BUILD_DFA_NODE_FINAL(TKTYPE_TRUE);    //true
  
  DfaNode q65 = BUILD_DFA_NODE_FINAL(TKTYPE_VAR);       //i
  DfaNode q66 = BUILD_DFA_NODE_FINAL(TKTYPE_VAR);       //in
  DfaNode q67 = BUILD_DFA_NODE_FINAL(TKTYPE_T_INT);     //int

  DfaNode q68 = BUILD_DFA_NODE_FINAL(TKTYPE_VAR);       //c
  DfaNode q69 = BUILD_DFA_NODE_FINAL(TKTYPE_VAR);       //ch
  DfaNode q70 = BUILD_DFA_NODE_FINAL(TKTYPE_VAR);       //cha
  DfaNode q71 = BUILD_DFA_NODE_FINAL(TKTYPE_T_CHAR);    //char

  DfaNode q72 = BUILD_DFA_NODE_FINAL(TKTYPE_VAR);       //b
  DfaNode q73 = BUILD_DFA_NODE_FINAL(TKTYPE_VAR);       //bo
  DfaNode q74 = BUILD_DFA_NODE_FINAL(TKTYPE_VAR);       //boo
  DfaNode q75 = BUILD_DFA_NODE_FINAL(TKTYPE_T_BOOL);    //bool

  DfaNode q76 = BUILD_DFA_NODE_FINAL(TKTYPE_VAR);       //l
  DfaNode q77 = BUILD_DFA_NODE_FINAL(TKTYPE_VAR);       //lo
  DfaNode q78 = BUILD_DFA_NODE_FINAL(TKTYPE_VAR);       //lon
  DfaNode q79 = BUILD_DFA_NODE_FINAL(TKTYPE_T_LONG);    //long

  DfaNode q80 = BUILD_DFA_NODE_FINAL(TKTYPE_VAR);       //d
  DfaNode q81 = BUILD_DFA_NODE_FINAL(TKTYPE_VAR);       //do
  DfaNode q82 = BUILD_DFA_NODE_FINAL(TKTYPE_VAR);       //dou
  DfaNode q83 = BUILD_DFA_NODE_FINAL(TKTYPE_VAR);       //doub
  DfaNode q84 = BUILD_DFA_NODE_FINAL(TKTYPE_VAR);       //doubl
  DfaNode q85 = BUILD_DFA_NODE_FINAL(TKTYPE_T_DOUBLE);  //double

  DfaNode q86 = BUILD_DFA_NODE_FINAL(TKTYPE_VAR);       //fl (f do for)
  DfaNode q87 = BUILD_DFA_NODE_FINAL(TKTYPE_VAR);       //flo
  DfaNode q88 = BUILD_DFA_NODE_FINAL(TKTYPE_VAR);       //floa
  DfaNode q89 = BUILD_DFA_NODE_FINAL(TKTYPE_T_FLOAT);   //float

  /*-----WTPS------*/
  dfa_add_delta_expr(&q0, " \t\n", &q1);
  dfa_add_delta_expr(&q1, " \t\n", &q1);

  /*-----SEPARADORES/DEMARCADORES------*/
  dfa_add_delta(&q0, '(', &q2);
  dfa_add_delta(&q0, ')', &q3);
  dfa_add_delta(&q0, '{', &q4);
  dfa_add_delta(&q0, '}', &q5);
  dfa_add_delta(&q0, '[', &q6);
  dfa_add_delta(&q0, ']', &q7);
  dfa_add_delta(&q0, ',', &q8);
  dfa_add_delta(&q0, ';', &q9);

  /*-----OPERADORES ARITIMETICOS------*/
  dfa_add_delta(&q0, '+', &q10);
  dfa_add_delta(&q0, '-', &q11);
  dfa_add_delta(&q0, '/', &q12);
  dfa_add_delta(&q0, '*', &q13);
  dfa_add_delta(&q0, '%', &q14);

  //bitwise
  dfa_add_delta(&q0, '&', &q15);
  dfa_add_delta(&q0, '|', &q16);
  dfa_add_delta(&q22, '<', &q17);
  dfa_add_delta(&q20, '>', &q18);

  /*-----OPERADORES LOGICOS------*/
  dfa_add_delta(&q27, '=', &q19);

  dfa_add_delta(&q0, '>', &q20);
  dfa_add_delta(&q20, '=', &q21);

  dfa_add_delta(&q0, '<', &q22);
  dfa_add_delta(&q22, '=', &q23);

  dfa_add_delta(&q16, '|', &q24);
  dfa_add_delta(&q15, '&', &q25);
  
  /*-----VARIAVEIS------*/
  dfa_add_delta_expr(&q0, "_[A-Za-z]", &q26); //^ieftscw
  dfa_add_delta_expr(&q26, "_[A-Za-z]", &q26);

  dfa_add_delta(&q0, '=', &q27); //atribuicao
  /*-----NUMEROS------*/
  //int comum
  dfa_add_delta_expr(&q0, "[0-9]^0", &q29);
  dfa_add_delta_expr(&q29, "[0-9]", &q29);

  //float
  dfa_add_delta(&q29, '.', &q30);
  dfa_add_delta_expr(&q30, "[0-9]", &q30);

  //int em hexa ou octal. (ou simplesmente o numero 0)
  dfa_add_delta(&q0, '0', &q28);
  dfa_add_delta_expr(&q28, "[0-9]", &q32);
  dfa_add_delta_expr(&q32, "[0-9]", &q32);

  dfa_add_delta(&q28, 'x', &q31);
  dfa_add_delta_expr(&q31, "ABCDEFabcdef[0-9]", &q31);

  /*----------------------PALAVRAS RESERVADAS-----------------------*/
  //TODO


/*--------------------------ANALISE LEXICA------------------------------*/
  char *eval = "if (cont > 10) { +-/% & >> <<  > >= < <= == && || sou_uma_variavel = 0x23fab - 0432;}";
  TokenList *tkns = tokenize(eval, &q0);
  TokenList *curlist = tkns;
  for (; curlist; curlist = curlist->next){
    printf("TOKEN(lexeme=\"%s\", type=%s)\n", curlist->lexeme, print_type(curlist->type));
  }

/*------------------------ANALISE SINTATICA----------------------------*/
  //TODO...

  return 0;
}




