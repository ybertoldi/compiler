from rich import print as pp
from rich.rule import Rule

bnf= """
<DEC>   ::= <TIPO> id  eq <EXPR> pv;
<EXPR>  ::= <EXPR>  maiorq <EXPR1>  | <EXPR1>;
<EXPR1> ::= <EXPR1> menorq <EXPR2>  | <EXPR2>;
<EXPR2> ::= <EXPR2> mais   <EXPR3>  | <EXPR3>;
<EXPR3> ::= <EXPR3> menos  <EXPR4>  | <EXPR4>;
<EXPR4> ::= <EXPR4> vezes  <EXPR5>  | <EXPR5>;
<EXPR5> ::= id  | const | ap <EXPR> fp;
<TIPO>  ::= int | char | bool;
"""
cmd1 = ["tipo", "id", "eq", "const", "pv"]; # int numero = 50;
cmd2 = ["if", "ap", "id", "gt", "const", "fp"] # if (numero > 10)
bnf_dict = {}
terminais = []
for ln in bnf.split('\n'):
    if not ln.strip(): 
        continue

    c, prod = [s.strip().replace(';', '') for s in ln.split('::=')]
    bnf_dict[c] = [
            [s1.strip() for s1 in s.split() if s1] 
            for s in prod.split('|')
    ]
    terminais = []

_ffirstcache = {}
def find_first(bnf_dict: dict[str,list[list[str]]], target:str) -> list[str]:
    global _ffirstcache

    if target in _ffirstcache:
        return _ffirstcache[target]
    ret = []
    for first_prod in [x[0] for x in bnf_dict[target]]:
        if first_prod == target: continue
        if first_prod.startswith('<'):
            ret.extend(find_first(bnf_dict, first_prod))
        else:
            ret.append(first_prod)
    _ffirstcache[target] = ret
    return ret


_ffollowcache = {}
def find_follow(
        bnf_dict: dict[str, list[list[str]]],
        firsts: dict[str, list[str]],
        target: str
        ) -> set[str]:
    global _ffollowcache

    if target in _ffollowcache:
        return _ffollowcache[target]

    ret = set()
    for var, prods in bnf_dict.items():
        for prod in prods:
            if var == target:
                if prod[-1].startswith('<') and prod[-1] != target:
                    ret |= find_follow(bnf_dict, firsts, prod[-1])
            for i in range(len(prod)):
                if prod[i] == target and i + 1 < len(prod):
                    if prod[i+1].startswith('<'):
                        ret.add(firsts[prod[i+1]])
                    else:
                        ret.add(prod[i+1])
    _ffollowcache[target] = ret
    return ret

firsts = {}
for key in bnf_dict:
    firsts[key] = find_first(bnf_dict, key)

follows = {}
for key in bnf_dict:
    follows[key] = list(find_follow(bnf_dict, firsts, key))



M = dict((k, {}) for k in bnf_dict)

pp(Rule('dicionario bnf'), bnf_dict)
pp(Rule('firsts'),firsts)
pp(Rule("follows"),follows)




