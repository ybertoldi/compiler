# Compilador
Compilador simples escrito em C.

# Build
Certifique-se de possuir git e GNU Make instalados no seu sistema. Para baixar e gerar uma versao de teste, rode:
```bash
git clone https://github.com/ybertoldi/compiler.git
cd compiler
make test
```

# Testando
Após fazer a build, no diretorio `teste`, há um programa simples escrito em `src.lang`, execute os seguintes comandos 
para verificar a funcionalidade do compilador
- `./Compiler src.lang -t` para ver a lista de tokens gerada.
- `./Compiler src.lang -g` para ver a derivacao da gramatica.
- `./Compiler src.lang` para realizar a compilacao normal.





