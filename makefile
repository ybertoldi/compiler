CFLAGS= -O2 -g -Wall -Wextra 

LIB= lib/dfa_node.o lib/hashmap.o lib/tokenizer.o
objects= dfa_node.o hashmap.o tokenizer.o



Compiler: src/main.c $(objects)
	gcc src/main.c $(LIB) $(CFLAGS) -o Compiler

$(objects): %.o : src/%.c
	gcc -c $^ -o lib/$@ $(CFLAGS)

lib/slr_tables.o: src/slr_tables.c

includes/grammar.h src/slr_tables.c: gramatica.bnf
	python3 utils/slr_generate.py gramatica.bnf -h includes/grammar.h -c src/slr_tables.c

test: Compiler
	mv Compiler teste

lib/slr_tables.o: src/slr_tables.c
	gcc -c src/slr_tables.c -o lib/slr_tables.o $(CFLAGS)

test_slr: $(LIB) lib/slr_tables.o
	gcc src/slr.c $(LIB) lib/slr_tables.o -o teste/srl -g


