from typing import DefaultDict
from rich import print as pp
from rich.rule import Rule
from copy import deepcopy

# bnf= """
# <DEC>   ::= <TIPO> id  eq <EXPR> pv;
# <EXPR>  ::= <EXPR>  maiorq <EXPR1>  | <EXPR1>;
# <EXPR1> ::= <EXPR1> menorq <EXPR2>  | <EXPR2>;
# <EXPR2> ::= <EXPR2> mais   <EXPR3>  | <EXPR3>;
# <EXPR3> ::= <EXPR3> menos  <EXPR4>  | <EXPR4>;
# <EXPR4> ::= <EXPR4> vezes  <EXPR5>  | <EXPR5>;
# <EXPR5> ::= id  | const | ap <EXPR> fp;
# <TIPO>  ::= int | char | bool;
# """
# 
# cmd1 = ["tipo", "id", "eq", "const", "pv"]; # int numero = 50;
# cmd2 = ["if", "ap", "id", "gt", "const", "fp"] # if (numero > 10)

bnf = """
<E>   ::= <T> <E'>;
<E'>  ::= mais <T> <E'> | epilson;
<T>   ::= <F> <T'>;
<T'>  ::= vezes <F> <T'> | epilson;
<F>   ::= ap <E> fp | id;
"""

bnf_dict = {}
terminais = set()
first_iter = True
start = ''
for ln in bnf.split('\n'):
    if not ln.strip(): 
        continue

    c, prod = [s.strip().replace(';', '') for s in ln.split('::=')]
    if first_iter:
        start = c
        first_iter = False

    bnf_dict[c] = [
            [s1.strip() for s1 in s.split() if s1] 
            for s in prod.split('|')
    ]
    terminais |= set(term for prod in bnf_dict[c] for term in prod if not term.startswith('<'))

terminais = list(terminais)
pp(Rule('dicionario bnf'), bnf_dict)

_ffirstcache = {}
def find_first(bnf_dict: dict[str,list[list[str]]], target:str) -> list[str]:
    global _ffirstcache

    if target in _ffirstcache:
        return _ffirstcache[target]
    ret = []
    for prod in bnf_dict[target]:
        i = 0
        while i < len(prod):
            if prod[i].startswith('<'):
                temp_firsts = find_first(bnf_dict, prod[i])
                ret.extend(temp_firsts)
                if 'epilson' in temp_firsts:
                    i += 1
                else: 
                    break
            else:
                ret.append(prod[i])
                break

    ret = list(set(ret))
    _ffirstcache[target] = ret
    return ret


firsts = {}
for key in bnf_dict:
    pp(key)
    firsts[key] = find_first(bnf_dict, key)

follows = DefaultDict(set)
follows[start].add('$')

for var in bnf_dict: #regra 2 do algoritmo
    for prod in bnf_dict[var]:
        i = 0
        plen = len(prod)
        while i < plen - 1:
            if prod[i].startswith('<'):
                if prod[i + 1].startswith('<'):
                    follows[prod[i]]\
                            .update( set(firsts[prod[i+1]]) - {'epilson'} )
                else:
                    follows[prod[i]].add(prod[i+1])
            i += 1

for var in bnf_dict: #regra 3 do algoritmo. Talvez tenha algo errado aqui
    for prod in bnf_dict[var]:
        j = len(prod) - 1
        while j >= 0:
            if prod[j].startswith('<'):
                follows[prod[j]].update(follows[var])

                if 'epilson' not in firsts[prod[j]]: break
                else:  j -= 1
                    
            else:
                break

pp(Rule('firsts'),firsts)
pp(Rule("follows"),follows)

colunas = dict(zip(terminais + ['$'],[ [] for _ in range(len(terminais)+1) ]))
M = dict((k, deepcopy(colunas)) for k in bnf_dict)

# se X é terminal, então first(X) = {X}
for A, prods_A in bnf_dict.items():
    for alpha in prods_A:
        for p in alpha:
            if p.startswith('<'):
                for a in firsts[p]:
                    M[A][a] += [(A, alpha)]
                if 'epilson' in firsts[p]:
                    for b in follows[A]:
                        M[A][b] += [(A, alpha)]
                else:
                    break
            else:
                if p == 'epilson':
                    for b in follows[A]:
                        M[A][b] += [(A, alpha )]
                    M[A]['$'] = [(A, alpha )]
                else:
                    M[A][p] += [(A, alpha)]
                    break


for col in M.values():
    if 'epilson' in col:
        col.pop('epilson')
pp(Rule('tabela M'), M)



