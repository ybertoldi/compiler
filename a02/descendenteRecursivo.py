"""
COMANDO -> ATR | DEC
ATR -> id atr EXP pv
EXP -> id | const
DEC -> tipo id pv
"""


listatoken = ["ip", ""]
cont  = 0

def term(token):
    global listatoken, cont

    res = listatoken[cont] == token
    cont += 1
    return res

def comando():
    global listatoken, cont

    anterior = listatoken[cont]
    if atr():
        return True
    else:
        cont = anterior
        return dec()

def atr():
    return term("id") and term('atr') and exp() and term("pv")

def exp():
    global cont, listatoken

    if exp1():
        return True
    else:
        cont = anterior
        return exp2()


def dec():
    return term("tipo") and term("id") and term("pv")






