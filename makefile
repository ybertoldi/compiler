.PHONY: test_compiler test_slr

CFLAGS=  -O1 -g -Wall -Wextra 
LIB_COMMON= lib/dfa_node.o lib/hashmap.o lib/tokenizer.o

Compiler: src/main.c $(LIB_COMMON) lib/slr.o lib/slr_tables.o
	gcc src/main.c $(LIB_COMMON) lib/slr.o lib/slr_tables.o -o Compiler $(CFLAGS)

lib/slr.o: lib/slr_tables.o src/slr.c
	gcc -c src/slr.c -o lib/slr.o $(CFLAGS)

lib/slr_tables.o: src/slr_tables.c includes/grammar.h lib/tokenizer.o
	gcc -c src/slr_tables.c -o lib/slr_tables.o $(CFLAGS)

includes/grammar.h src/slr_tables.c: gramatica.bnf utils/slr_generate.py
	python3 utils/slr_generate.py gramatica.bnf -h includes/grammar.h -c src/slr_tables.c


# commons
lib/hashmap.o: src/hashmap.c
	gcc -c $< -o $@ $(CFLAGS)

lib/tokenizer.o: src/tokenizer.c
	gcc -c $< -o $@ $(CFLAGS)

lib/dfa_node.o: src/dfa_node.c
	gcc -c $< -o $@ $(CFLAGS)



test_compiler: Compiler
	mkdir -p teste; mv Compiler teste

test_slr: $(LIB_COMMON) lib/slr_tables.o
	mkdir -p teste/slr_test;
	gcc src/slr.c $(LIB_COMMON) lib/slr_tables.o -o teste/slr_test/slr $(CFLAGS) --define-macro=SLR_DEBUG;
	if [ ! -f teste/slr_test/src.lang ]; then \
		printf "int x = 20;\nbool y = false;\n" > teste/slr_test/src.lang; \
	fi
	echo "./slr < src.lang" > teste/slr_test/run.sh;
	chmod +x teste/slr_test/run.sh;

