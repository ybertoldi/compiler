from dataclasses import dataclass, field
from typing import Mapping
from rich import print as pp

@dataclass
class Prod:
    head:  str
    production: "list[str | Variable]" = field(default_factory=list)

    def __repr__(self) -> str:
        r = f"Prod({self.head} -> "
        for elem in self.production:
            if isinstance(elem, str):
                r += elem + ' '
            else:
                r += f"Variable({elem.head}) "
        return r + ')'

@dataclass
class Variable:
    head: str
    prods: list[Prod] = field(default_factory=list)

    def __repr__(self) -> str:
        r = f"Variable({self.head} -> \n"
        for i, prod in enumerate(self.prods):
            r += '\t '
            if i > 0: 
                r = r[:-1] + '|'
            for elem in prod.production:
                if isinstance(elem, str):
                    r += elem + ' '
                else:
                    r += f"Variable({elem.head}) "
            r += '\n'
        return r + ')'

@dataclass
class Item:
    prod: Prod
    pos:  int = -1

@dataclass
class Node:
    items: list[Item] = field(default_factory=list)
    goto:  "Mapping[str, Node]" = field(default_factory=dict)

bnf = """
<E'>  ::= <E>
<E>   ::= <E> + <T> | <T>
<T>   ::= <T> * <F> | <F>
<F>   ::= ap <E> fp | id
"""

variables_map = {}
variables = []
for ln in bnf.splitlines():
    if not ln: 
        continue

    p, rest = (s.strip() for s in ln.split("::="))
    if p in variables_map:
        cur_var = variables_map[p]
    else:
        cur_var = Variable(p[1:-1])
        variables_map[p] = cur_var

    for derivation in (d.strip() for d in rest.split('|')):
        cur_prod = Prod(cur_var.head)
        for elem in derivation.split():
            if elem.startswith('<'):
                if elem in variables_map:
                    cur_prod.production.append(variables_map[elem])
                else:
                    variables_map[elem] = Variable(elem[1:-1])
                    cur_prod.production.append(variables_map[elem])
            else:
                cur_prod.production.append(elem)

        cur_var.prods.append(cur_prod)    

    variables.append(cur_var)




pp(variables)

initial = Item(variables[0].prods[0])
def closure(start: list[Item]) -> list[Item]:
    ret: list[Item] = []
    
    look: list[Item] = start.copy()
    added = []
    while (look):
        cur = look.pop(0)
        ret.append(cur)

        if cur.pos + 1 >= len(cur.prod.production):
            continue

        follow = cur.prod.production[cur.pos + 1]
        if follow in added:
            continue
        added.append(follow)

        if isinstance(follow, Variable):
            for prod in follow.prods:
                look.append(Item(prod))

    return ret

pp(closure([initial]))

