CFLAGS= -O2 -g -Wall -Wextra 
LIB_COMMON= lib/dfa_node.o lib/hashmap.o lib/tokenizer.o

Compiler: src/main.c $(LIB_COMMON)
	gcc src/main.c $(LIB_COMMON) -o Compiler $(CFLAGS)

$(LIB_COMMON): src/hashmap.c src/tokenizer.c src/dfa_node.c
	gcc -c src/hashmap.c -o lib/hashmap.o $(CFLAGS);
	gcc -c src/tokenizer.c -o lib/tokenizer.o $(CFLAGS);
	gcc -c src/dfa_node.c -o lib/dfa_node.o $(CFLAGS)

includes/grammar.h src/slr_tables.c: gramatica.bnf utils/slr_generate.py
	python3 utils/slr_generate.py gramatica.bnf -h includes/grammar.h -c src/slr_tables.c

lib/slr_tables.o: src/slr_tables.c
	gcc -c src/slr_tables.c -o lib/slr_tables.o $(CFLAGS)

test_compiler: Compiler
	mkdir -p teste; mv Compiler teste

test_slr: $(LIB_COMMON) lib/slr_tables.o
	mkdir -p teste/slr_test;
	gcc src/slr.c $(LIB_COMMON) lib/slr_tables.o -o teste/slr_test/slr $(CFLAGS);
	echo "int x = 20;\nbool y = false;" > teste/slr_test/src.lang;
	echo "./slr < src.lang" > teste/slr_test/run.sh

