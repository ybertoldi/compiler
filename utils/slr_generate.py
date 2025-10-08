from collections import defaultdict
from dataclasses import dataclass, field
from rich import print as pprint
# import graphviz -- descomente se quiser gerar a imagem do automato


@dataclass
class Prod:
    head:  str
    production: "list[str | Variable]" = field(default_factory=list)

    def __repr__(self) -> str:
        return self.head

#   def __eq__(self, other):
#       return isinstance(other, Prod) and other.head == self.head

    def __hash__(self) -> int:
        return hash(self.head)

@dataclass
class Variable:
    head: str
    prods: list[Prod] = field(default_factory=list)

    def __repr__(self) -> str:
        return self.head
    

 #   def __eq__(self, other):
 #       return isinstance(other, Variable) and other.head == self.head

    def __hash__(self) -> int:
        return hash(self.head)


@dataclass
class Item:
    prod: Prod
    pos:  int = -1

    def __repr__(self) -> str:
        r = repr(self.prod) + " -> "
        for i, p in enumerate(self.prod.production):
            if self.pos + 1 == i:
                r += '· '
            r += str(p) + " "
        return r

    def __eq__(self, other):
        return  isinstance(other, Item)\
                and other.prod == self.prod\
                and self.pos == other.pos

    def __hash__(self) -> int:
        return hash(self.prod) + hash(self.pos)

@dataclass
class Node:
    items: list[Item]
    goto:  "dict[str | Variable, Node]" = field(default_factory=dict)

    def __eq__(self, other) -> bool:
        return isinstance(other, Node) and set(self.items) == set(other.items)



bnf = """
<E'>  ::= <E>
<E>   ::= <E> + <T> | <T>
<T>   ::= <T> * <F> | <F>
<F>   ::= ap <E> fp | id
"""


variables_cache: dict[str, Variable] = {}
variables: list[Variable] = []
grammar_symbols: list[Variable | str] = []
for ln in bnf.splitlines():
    if not ln: 
        continue

    p, rest = (s.strip() for s in ln.split("::="))
    if p in variables_cache:
        cur_var = variables_cache[p]
    else:
        cur_var = Variable(p[1:-1])
        variables_cache[p] = cur_var

    for derivation in (d.strip() for d in rest.split('|')):
        cur_prod = Prod(cur_var.head)
        for elem in derivation.split():
            if elem.startswith('<'):
                if elem in variables_cache:
                    cur_prod.production.append(variables_cache[elem])
                else:
                    variables_cache[elem] = Variable(elem[1:-1])
                    cur_prod.production.append(variables_cache[elem])
                
                if variables_cache[elem] not in grammar_symbols:
                    grammar_symbols.append(variables_cache[elem])
            else:
                cur_prod.production.append(elem)
                if elem not in grammar_symbols:
                    grammar_symbols.append(elem)

        cur_var.prods.append(cur_prod)    

    variables.append(cur_var)



def closure(start: list[Item]) -> Node:
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

    return Node(ret)

_goto_cache: dict[tuple[Item | str | Variable, ...], Node] = {} 
def goto(items: list[Item], symbol: str | Variable) -> None | Node:
    key = tuple(items + [symbol])
    if key in _goto_cache:
        return _goto_cache[key]

    print(items)

    kernels: list[Item] = []
    for item in items:
        if item.pos + 1 >= len(item.prod.production):
            continue

        if item.prod.production[item.pos + 1] == symbol:
            kernels.append(Item(item.prod, item.pos +1))

    if kernels:
        ret = closure(kernels)
        if ret in _goto_cache.values():
            return [v for v in  _goto_cache.values() if v == ret][0]
        _goto_cache[key] = ret
        return ret
    return None


initial = Item(variables[0].prods[0])
C = closure([initial]) #closure inicial: E' -> ·E
to_visit = [C]
visited = [C] 

while to_visit:
    cur_c = to_visit.pop(0)
    temp_items = defaultdict(list)
    for s in grammar_symbols:
        if (gt := goto(cur_c.items, s)) and gt not in cur_c.goto.values():
            cur_c.goto[s] = gt
            if gt not in visited:
                to_visit.append(gt)
                visited.append(gt)
        
# -- descomente para gerar a imagem do automato
# pprint(C)
# def draw_lr_automaton(start: Node, filename="automaton"):
#     dot = graphviz.Digraph(format="png")
#     visited = {}
#     counter = [0]
# 
#     def node_label(node):
#         lines = [str(it) for it in node.items]
#         return "\n".join(lines)
# 
#     def dfs(node):
#         if id(node) in visited:
#             return
#         idx = counter[0]
#         counter[0] += 1
#         visited[id(node)] = f"I{idx}"
# 
#         label = node_label(node)
#         dot.node(visited[id(node)], label=label, shape="box", fontsize="10", fontname="monospace")
# 
#         for sym, nxt in node.goto.items():
#             dfs(nxt)
#             dot.edge(visited[id(node)], visited[id(nxt)], label=str(sym))
# 
#     dfs(start)
#     dot.render(filename, view=True)
# 
# draw_lr_automaton(C)
