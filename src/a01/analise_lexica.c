#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>

#include "../includes/tokenizer.h"


int main() {
  /*--------------------------ANALISE LEXICA------------------------------*/
//  char *eval = "\nif (cont > 10) {\n" 
//               "\t+-/% & >> <<  > >= < <= == && ||\n"
//               "\tint uma_variavel=0x23fab - 0432;\n"
//               "}\n\n"
//               "double umdouble=4.5;\n"
//               "long x,y;\n"
//               "float z;\n"
//               "x=2; y=5; z = 1.0*x/y;\n"
//               "for (int i = 0; i<10; i+= 1)\n"
//               "\tx |= 0x1 << i;";

  char buf[4098] = {0};
  int bp = 0;
  int c;
  printf("Escreva um programa e aperte CTRL-D para terminar:\n\n");
  while ((c = getchar()) != EOF) 
    buf[bp++] = c; 

  printf("\n\n");
  TokenList *tkns = tokenize(buf);
  TokenList *curlist = tkns;
  for (; curlist; curlist = curlist->next) {
    if (curlist->type == TKTYPE_WTSP) continue;
    printf("TOKEN(\"%s\", %s)%s\n", 
           curlist->lexeme,
           type2str(curlist->type),
           curlist->next ? "," : "");
  }
  printf("\n\ntexto original: \n%s\n", buf);
  return 0;
}
