# Compilador
Compilador simples escrito em C.

# Build
Certifique-se de possuir git e GNU Make instalados no seu sistema. Para baixar e gerar uma versao de teste, rode:
```bash
git clone https://github.com/ybertoldi/compiler.git
cd compiler
make test_compiler
```

# Testando
### Compilador
Após fazer a build, no diretorio `teste`, há um programa simples escrito em `src.lang`, execute os seguintes comandos 
para verificar a funcionalidade do compilador
- `./Compiler src.lang -t` para ver a lista de tokens gerada.
- `./Compiler src.lang -g` para ver a derivacao da gramatica. 
- `./Compiler src.lang` para realizar a compilacao normal. (ainda nao implementado)

### Gramatica, Compilador de Gramatica e Parser de SLR
Os arquivos [grammar.h](includes/grammar.h) e [slr_tables.c](src/slr_tables.c) são gerados automaticamente por um script em python, baseando-se na gramatica
escrita em [gramatica.bnf](gramatica.bnf). Rode 
```bash
make test_slr
```
Para gerar um programa que testa a gramatica gerada e sua derivacao. Basta modificar o arquivo [gramatica.bnf](gramatica.bnf) como quiser (desde que siga as regras, é claro), compilar com o makefile e testar
o resultado com os arquivos gerados no diretorio `teste/slr_test`. \
Dentro de `teste/slr_test`, modifique o arquivo com o programa fonte em `src.lang` e rode `bash run.sh` para ver a derivacao.
