#include "../includes/dfa_node.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct tk{
  char *lexeme;
  char *type;
  struct tk *next;
} TokenList;

TokenList* tklalloc(char *lexeme, char *type){
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

TokenList *tokenize(char *eval, DfaNode *start, char **map){
  int i = 0;
  TokenList *curlist = NULL, *head;
  DfaNode *curnode = start;

  while (eval[i]) {
    int tknstart = i, n = 0;
    while (dfa_node_move(&curnode, eval[i])) {
      i++;
      n++;
    }

    // TODO: fatorar essa bagunça
    if (curlist) {
      tklappend(curlist, tklalloc(strndup(&eval[tknstart], n),
                                  map[atoi((curnode->name + 1))]));
    } else {
      curlist =
          tklalloc(strndup(&eval[tknstart], n), map[atoi((curnode->name + 1))]);
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
/*-----------------------CRIAÇÃO DOS AUTOMATOS-----------------------*/
  DfaNode q0 = BUILD_DFA_NODE("q0");        // est. inicial
  DfaNode q1 = BUILD_DFA_NODE_FINAL("q1");  // i
  DfaNode q2 = BUILD_DFA_NODE_FINAL("q2");  // if
  DfaNode q3 = BUILD_DFA_NODE_FINAL("q3");  // INT
  DfaNode q4 = BUILD_DFA_NODE_FINAL("q4");  // variavel
  DfaNode q5 = BUILD_DFA_NODE_FINAL("q5");  // espaco
  DfaNode q6 = BUILD_DFA_NODE_FINAL("q6");  // (
  DfaNode q7 = BUILD_DFA_NODE_FINAL("q7");  // )
  DfaNode q8 = BUILD_DFA_NODE_FINAL("q8");  // >
  DfaNode q9 = BUILD_DFA_NODE_FINAL("q9");  // {

  DfaNode q10 = BUILD_DFA_NODE_FINAL("q10");  // FLOAT

  DfaNode q11 = BUILD_DFA_NODE_FINAL("q11");  // f
  DfaNode q12 = BUILD_DFA_NODE_FINAL("q12");  // fa
  DfaNode q13 = BUILD_DFA_NODE_FINAL("q13");  // fal
  DfaNode q14 = BUILD_DFA_NODE_FINAL("q14");  // fals
  DfaNode q15 = BUILD_DFA_NODE_FINAL("q15");  // false
  
  DfaNode q16 = BUILD_DFA_NODE_FINAL("q16");  // t
  DfaNode q17 = BUILD_DFA_NODE_FINAL("q17");  // tr
  DfaNode q18 = BUILD_DFA_NODE_FINAL("q18");  // tru
  DfaNode q19 = BUILD_DFA_NODE_FINAL("q19");  // true

  char *mapa[] = {
       "EST.INIT",
       "VAR",
       "IF",
       "INT",
       "VAR",
       "WTSP",
       "OPPAREN",
       "CLPAREN",
       "GT",
       "OPBRACKET",
       "FLOAT",
       "VAR",
       "VAR",
       "VAR",
       "VAR",
       "BOOL",
       "VAR",
       "VAR",
       "VAR",
       "BOOL",
  };


  /*-----SEPARADORES/DEMARCADORES------*/
  dfa_add_delta(&q0, ' ', &q5);
  dfa_add_delta(&q0, '(', &q6);
  dfa_add_delta(&q0, ')', &q7);
  dfa_add_delta(&q0, '>', &q8);
  dfa_add_delta(&q0, '{', &q9);

  /*-----NOMES------*/
  dfa_add_delta_expr(&q0, "_[a-zA-Z]^ift", &q4);
  dfa_add_delta_expr(&q4, "_[a-zA-Z0-9]", &q4);

  /*-----IF------*/
  dfa_add_delta(&q0, 'i', &q1);
  dfa_add_delta_orelse(&q1, 'f', &q2, &q4);
  dfa_add_delta_expr(&q2, "_[a-zA-Z0-9]", &q4);

  /*-----INT------*/
  dfa_add_delta_expr(&q0, "[0-9]", &q3);
  dfa_add_delta_expr(&q3, "[0-9]", &q3);

  /*-----FLOAT------*/
  dfa_add_delta_expr(&q0, "[0-9]", &q3);
  dfa_add_delta(&q3, '.', &q10);
  dfa_add_delta_expr(&q10, "[0-9]", &q10);
  
  /*-----FALSE------*/
  dfa_add_delta(&q0, 'f', &q11);
  dfa_add_delta_orelse(&q11, 'a', &q12, &q4);
  dfa_add_delta_orelse(&q12, 'l', &q13, &q4);
  dfa_add_delta_orelse(&q13, 's', &q14, &q4);
  dfa_add_delta_orelse(&q14, 'e', &q15, &q4);
  dfa_add_delta_expr(&q15, "_[A-Za-z0-9]", &q4);

  /*-----TRUE------*/
  dfa_add_delta(&q0, 't', &q16);
  dfa_add_delta_orelse(&q16, 'r', &q17, &q4);
  dfa_add_delta_orelse(&q17, 'u', &q18, &q4);
  dfa_add_delta_orelse(&q18, 'e', &q19, &q4);
  dfa_add_delta_expr(&q19, "_[A-Za-z0-9]", &q4);


/*--------------------------ANALISE LEXICA------------------------------*/
  char *eval = "if (cont > 10) {";
  TokenList *tkns = tokenize(eval, &q0, mapa);
  TokenList *curlist = tkns;

/*------------------------ANALISE SINTATICA----------------------------*/
  //TODO...

  return 0;
}




