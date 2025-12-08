CFLAGS= -O2 -g -Wall -Wextra 

LIB= lib/dfa_node.o lib/hashmap.o lib/tokenizer.o
objects= dfa_node.o hashmap.o tokenizer.o

Compiler: src/main.c $(objects)
	gcc src/main.c $(LIB) $(CFLAGS) -o Compiler

$(objects): %.o : src/%.c
	gcc -c $^ -o lib/$@









