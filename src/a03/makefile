LIB= ../../lib
slr_ex: slr.c $(LIB)/dfa_node.o $(LIB)/tokenizer.o
	gcc -o slr_ex slr.c $(LIB)/dfa_node.o $(LIB)/tokenizer.o -ggdb -Wall -Wextra -O3


debug: slr.c $(LIB)/dfa_node.o $(LIB)/tokenizer.o
	gcc -o slr_ex slr.c $(LIB)/dfa_node.o $(LIB)/tokenizer.o -ggdb -Wall -Wextra -ggdb -O3

run: slr.c
	gcc -o slr_ex slr.c ../../lib/* && ./slr_ex

