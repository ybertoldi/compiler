CC=gcc
CFLAGS=-Wall -Wextra -ggdb
DEPS= ../includes/dfa_node.c ../includes/tokenizer.c

teste_sintatico: descendente_recursivo.c $(DEPS)
	$(CC) -o teste_sintatico descendente_recursivo.c $(DEPS) $(CFLAGS)

