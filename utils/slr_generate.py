from collections import defaultdict
from dataclasses import dataclass, field
from os import getcwd
from os.path import isfile, relpath
from rich import print
import sys

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



TARGET_FILE = False
HEADER_N_SRC = False
src_filename = ""
header_filename = ""

if (len(sys.argv) < 2):
    print(f"usage: {sys.argv[0]} <bnf-file> [-f <dest.c>] [-h <header-file.h> -c <c-file.c>]")
    exit(1)
if not isfile(sys.argv[1]):
    print(f"usage: {sys.argv[0]} <bnf-file>. Could not identify file '{sys.argv[1]}'")
    exit(1)

with open(sys.argv[1], 'r') as f:
    bnf = f.read()

header_fd = sys.stdout
src_fd = sys.stdout
if "-f" in sys.argv:
    TARGET_FILE = True

    tgt_name = sys.argv[sys.argv.index("-f")+1]
    header_fd = open(tgt_name, "w")
    src_fd = header_fd
elif "-h" in sys.argv and "-c" in sys.argv:
    HEADER_N_SRC = True

    header_filename = sys.argv[sys.argv.index("-h")+1]
    src_filename = sys.argv[sys.argv.index("-c")+1]
    src_fd = open(src_filename, "w")
    header_fd = open(header_filename, "w")



variables_cache: dict[str, Variable] = {}
variables: list[Variable] = []
grammar_symbols: list[Variable | str] = []

for ln in bnf.split(";"):
    if not ln.strip(): 
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
to_visit: list[Node] = [C]
visited = [C] 
nodes: list[Node] = []

while to_visit:
    cur_c = to_visit.pop(0)
    nodes.append(cur_c)
    temp_items = defaultdict(list)
    for s in grammar_symbols:
        if (gt := goto(cur_c.items, s)) and gt not in cur_c.goto.values():
            cur_c.goto[s] = gt
            if gt not in visited:
                to_visit.append(gt)
                visited.append(gt)
        



######################GERANDO O CODIGO###########################



# struct para as derivacoes

if HEADER_N_SRC:
    header_fd.write("#ifndef GRAMMAR_H\n#define GRAMMAR_H\n\n")

header_fd.write("#include \"tokenizer.h\"\n\n")
header_fd.write(
"""
typedef struct {
  long target;
  long elements[256];
  long num_elems;
} Production;\n
""")

# enum com as variaveis da gramatica
enumlines = []
start_symbol = variables[0]
header_fd.write(f"#define START_SYMBOL VAR_{start_symbol.head.replace('\'', '_')}\n")

header_fd.write("typedef enum {")
for i, var in enumerate(variables):
    enumlines.append(
            f"VAR_{var.head.replace('\'', '_')}" 
            + (" = TKTYPE_NUM_TOKENS" if i == 0 else "")
    )
enumlines.append("VAR_NUM_VARS")
header_fd.write("\t" + ",\n\t".join(enumlines))
header_fd.write("} VARTYPE;\n\n")

header_fd.write("\n\n")

if HEADER_N_SRC:
    header_fd.write("char *var2str(VARTYPE t);\n\n")
    header_fd.write(f"extern long goto_table[{len(nodes)+1}][VAR_NUM_VARS];\n")
    header_fd.write(f"extern Production rdc_table[{len(nodes)+1}];\n")
    
    header_fd.write("#endif // GRAMMAR_H")

if HEADER_N_SRC:
    header_relpath = relpath(header_filename, src_filename).removeprefix("../");
    src_fd.write(f"#include \"{header_relpath}\"\n")
    src_fd.write(f"#include <assert.h>\n")

# tabela goto
src_fd.write(f"long goto_table[{len(nodes)+1}][VAR_NUM_VARS] = {{\n")
for i, node in enumerate(nodes):
    p = []
    for entry, dest in node.goto.items():
        entry_str = "TKTYPE_" + entry if isinstance(entry, str)\
                else f"VAR_{entry.head.replace('\'', '_')}"
        p.append(f"[{entry_str}] = {nodes.index(dest) + 1}\n")
    src_fd.write(f"\t[{i+1}] = {{ {', '.join(p)} }},\n")
src_fd.write("};\n\n")

# tabela das producoes
src_fd.write(
"""#define PROD(tgt, ...) (Production) \\
                {.target = tgt, .elements = {__VA_ARGS__}, .num_elems = (sizeof((long[]){__VA_ARGS__})/sizeof(long))}
""")

src_fd.write(f"Production rdc_table[{len(nodes)+1}] = {{\n")
for i, node in enumerate(nodes):
    p = []
    kernel = node.items[0]
    if kernel.pos + 1 == len(kernel.prod.production):
        fmt_head = "VAR_" + kernel.prod.head.replace('\'', '_')
        fmt_prods = [
                "TKTYPE_" + val if isinstance(val,str) \
                        else "VAR_" + val.head.replace('\'', '_')\
                for val in kernel.prod.production
        ]
        src_fd.write(f"\t[{i+1}] = PROD({fmt_head},   {', '.join(fmt_prods)}),\n")
src_fd.write("};\n")

src_fd.write("char *var2str(VARTYPE t){\n")
src_fd.write("  switch(t){\n")

for var in variables:
    varname = "VAR_" + var.head.replace('\'', '_')
    src_fd.write(f"    case {varname}: return \"{varname}\";\n")
src_fd.write("    default: assert(0 && \"type2str: invalid value\");\n")

src_fd.write("  }\n")
src_fd.write("}\n")

