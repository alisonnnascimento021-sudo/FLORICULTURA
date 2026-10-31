
#ifndef PRODUTOS_H
#define PRODUTOS_H

#define MAX_CARRINHO 100

struct Produto {
    int codigo;
    char nome[50];
    float preco;
    int quantidade;
    int estoqueMinimo;
    int diaValidade;
    int mesValidade;
    int anoValidade;
};

struct ItemCarrinho {
    struct Produto produto;
    int quantidade;
};

void cadastrarProduto(void);
void consultarProdutos(void);
void mostrarCatalogo(void);

int adicionarAoCarrinho(
    struct ItemCarrinho carrinho[],
    int *totalItens
);

void mostrarCarrinho(
    struct ItemCarrinho carrinho[],
    int totalItens
);

float calcularTotal(
    struct ItemCarrinho carrinho[],
    int totalItens
);

#endif

