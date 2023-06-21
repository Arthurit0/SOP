# Nomes: Arthur José e Ferdinando Galera


class Requisicao:
    def __init__(self, bloco_inicial, num_blocos, operacao):
        self.bloco_inicial = bloco_inicial
        self.num_blocos = num_blocos
        self.operacao = operacao


def verify_requisicoes(requisicao, listCurrent):
    for i in range(len(listCurrent)):
        req_prox = listCurrent[i]
        if req_prox == requisicao:
            continue

        if (
            req_prox.operacao == requisicao.operacao
            and requisicao.num_blocos + req_prox.num_blocos <= 64
            and (
                req_prox.operacao == "r"
                and (
                    req_prox.bloco_inicial
                    <= requisicao.bloco_inicial
                    <= requisicao.bloco_inicial + req_prox.num_blocos - 1
                )
                or req_prox.operacao == "w"
                and (
                    req_prox.bloco_inicial + req_prox.num_blocos - 1
                    == requisicao.bloco_inicial - 1
                )
            )
        ):
            return i

    return -1


def fundir(req_1, req_2):
    if req_1.bloco_inicial <= req_2.bloco_inicial:
        return Requisicao(
            req_1.bloco_inicial, req_1.num_blocos + req_2.num_blocos, req_1.operacao
        )
    else:
        return Requisicao(
            req_2.bloco_inicial, req_1.num_blocos + req_2.num_blocos, req_2.operacao
        )


def fundir_requisicoes(requisicoes):
    if len(requisicoes) <= 1:
        return requisicoes

    requisicoes_fundidas = []

    for i in range(0, len(requisicoes)):
        req_prox = requisicoes[i]

        index = verify_requisicoes(req_prox, requisicoes_fundidas)
        if index == -1:
            requisicoes_fundidas.append(req_prox)
        else:
            while index != -1:
                req_prox = fundir(req_prox, requisicoes_fundidas[index])
                requisicoes_fundidas.append(req_prox)
                requisicoes_fundidas.remove(requisicoes_fundidas[index])
                index = verify_requisicoes(
                    req_prox,
                    requisicoes_fundidas,
                )

    requisicoes_ordenadas = sorted(
        requisicoes_fundidas, key=lambda req: req.bloco_inicial
    )
    return requisicoes_ordenadas


def imprimir_fila(requisicoes):
    print("Fila:")
    for req in requisicoes:
        print(f"{req.bloco_inicial} {req.num_blocos} {req.operacao}")


def ler_requisicoes():
    requisicoes = []
    while True:
        try:
            linha = input()
            valores = linha.split()
            bloco_inicial = int(valores[0])
            num_blocos = int(valores[1])
            operacao = valores[2]
            requisicoes.append(Requisicao(bloco_inicial, num_blocos, operacao))
        except EOFError:
            break
    return requisicoes


requisicoes = ler_requisicoes()
requisicoes_fundidas = fundir_requisicoes(requisicoes)
imprimir_fila(requisicoes_fundidas)
