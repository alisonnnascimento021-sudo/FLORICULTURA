#ifndef PRODUTOS_H
#define PRODUTOS_H

#define MAX_CARRINHO 100

/* Comandos de navegacao */
#define NAV_OK 0
#define NAV_VOLTAR 1
#define NAV_LOGOUT 2
#define NAV_SAIR 3

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

int cadastrarProduto(void);
int alterarProduto(void);
int consultarProdutos(void);
int mostrarCatalogo(void);
int removerProduto(void);

int adicionarAoCarrinho(
    struct ItemCarrinho carrinho[],
    int *totalItens
);

void mostrarCarrinho(
    struct ItemCarrinho carrinho[],
    int totalItens
);

int gerenciarCarrinho(
    struct ItemCarrinho carrinho[],
    int *totalItens
);

float calcularTotal(
    struct ItemCarrinho carrinho[],
    int totalItens
);

/* Atualizar estoque depois da venda */
int atualizarEstoque(
    struct ItemCarrinho carrinho[],
    int totalItens
);

#endif
