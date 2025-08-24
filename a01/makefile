CC=gcc
CFLAGS=-Wall -Wextra -ggdb
DEPS= ../includes/dfa_node.c tokenizer.c

teste_tokens: analise_lexica.c $(DEPS)
	$(CC) -o teste_tokens analise_lexica.c $(DEPS) $(CFLAGS)

