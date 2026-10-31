
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "produtos.h"

/* =====================================
   CADASTRAR PRODUTO
   ===================================== */
void cadastrarProduto(void) {
    struct Produto produto;
    struct Produto existente;
    char linha[250];
    int resultado;
    FILE *arquivo;

    printf("\n====== CADASTRO DE PRODUTO ======\n");

    printf("Codigo: ");
    if (scanf("%d", &produto.codigo) != 1) {
        printf("Codigo invalido!\n");
        while (getchar() != '\n') {}
        return;
    }

    /* Verifica se o codigo ja existe */
    arquivo = fopen("produtos.txt", "r");

    if (arquivo != NULL) {
        while (fgets(linha, sizeof(linha), arquivo) != NULL) {
            resultado = sscanf(
                linha,
                "%d;%49[^;];%f;%d;%d;%d/%d/%d",
                &existente.codigo,
                existente.nome,
                &existente.preco,
                &existente.quantidade,
                &existente.estoqueMinimo,
                &existente.diaValidade,
                &existente.mesValidade,
                &existente.anoValidade
            );

            if (resultado == 8 &&
                existente.codigo == produto.codigo) {
                printf("Esse codigo ja esta cadastrado!\n");
                fclose(arquivo);
                return;
            }
        }

        fclose(arquivo);
    }

    printf("Nome do produto: ");
    scanf(" %49[^\n]", produto.nome);

    printf("Preco: ");
    if (scanf("%f", &produto.preco) != 1 ||
        produto.preco < 0) {
        printf("Preco invalido!\n");
        while (getchar() != '\n') {}
        return;
    }

    printf("Quantidade em estoque: ");
    if (scanf("%d", &produto.quantidade) != 1 ||
        produto.quantidade < 0) {
        printf("Quantidade invalida!\n");
        while (getchar() != '\n') {}
        return;
    }

    printf("Estoque minimo: ");
    if (scanf("%d", &produto.estoqueMinimo) != 1 ||
        produto.estoqueMinimo < 0) {
        printf("Estoque minimo invalido!\n");
        while (getchar() != '\n') {}
        return;
    }

    printf("Dia de validade: ");
    scanf("%d", &produto.diaValidade);

    printf("Mes de validade: ");
    scanf("%d", &produto.mesValidade);

    printf("Ano de validade: ");
    scanf("%d", &produto.anoValidade);

    arquivo = fopen("produtos.txt", "a");

    if (arquivo == NULL) {
        printf("Erro ao abrir produtos.txt!\n");
        return;
    }

    fprintf(
        arquivo,
        "%d;%s;%.2f;%d;%d;%02d/%02d/%04d\n",
        produto.codigo,
        produto.nome,
        produto.preco,
        produto.quantidade,
        produto.estoqueMinimo,
        produto.diaValidade,
        produto.mesValidade,
        produto.anoValidade
    );

    fclose(arquivo);

    printf("\nProduto cadastrado com sucesso!\n");
    printf("Codigo: %d\n", produto.codigo);
    printf("Nome: %s\n", produto.nome);
    printf("Preco: R$ %.2f\n", produto.preco);
    printf("Estoque: %d\n", produto.quantidade);

    if (produto.quantidade <= produto.estoqueMinimo) {
        printf("\n*** ALERTA: ESTOQUE BAIXO! ***\n");
    }
}

/* =====================================
   CONSULTAR PRODUTOS E ESTOQUE
   ===================================== */
void consultarProdutos(void) {
    FILE *arquivo;
    char linha[250];
    struct Produto produto;
    int resultado;
    int encontrou = 0;

    arquivo = fopen("produtos.txt", "r");

    if (arquivo == NULL) {
        printf("\nNenhum produto cadastrado ainda.\n");
        return;
    }

    printf("\n====== CONSULTA DE ESTOQUE ======\n");

    while (fgets(linha, sizeof(linha), arquivo) != NULL) {
        resultado = sscanf(
            linha,
            "%d;%49[^;];%f;%d;%d;%d/%d/%d",
            &produto.codigo,
            produto.nome,
            &produto.preco,
            &produto.quantidade,
            &produto.estoqueMinimo,
            &produto.diaValidade,
            &produto.mesValidade,
            &produto.anoValidade
        );

        if (resultado == 8) {
            encontrou = 1;

            printf("\nCodigo: %d\n", produto.codigo);
            printf("Produto: %s\n", produto.nome);
            printf("Preco: R$ %.2f\n", produto.preco);
            printf("Estoque: %d\n", produto.quantidade);
            printf("Estoque minimo: %d\n",
                   produto.estoqueMinimo);

            printf("Validade: %02d/%02d/%04d\n",
                   produto.diaValidade,
                   produto.mesValidade,
                   produto.anoValidade);

            if (produto.quantidade <= produto.estoqueMinimo) {
                printf("*** ALERTA: ESTOQUE BAIXO! ***\n");
            } else {
                printf("Situacao: Estoque normal.\n");
            }

            printf("---------------------------------\n");
        }
    }

    fclose(arquivo);

    if (!encontrou) {
        printf("Nenhum produto encontrado.\n");
    }
}

/* =====================================
   CATALOGO PUBLICO
   ===================================== */
void mostrarCatalogo(void) {
    FILE *arquivo;
    char linha[250];
    struct Produto produto;
    int resultado;
    int encontrou = 0;

    arquivo = fopen("produtos.txt", "r");

    printf("\n====================================\n");
    printf("       FLORICULTURA VAN GOGH\n");
    printf("          CATALOGO PUBLICO\n");
    printf("====================================\n");

    if (arquivo == NULL) {
        printf("Nenhum produto cadastrado ainda.\n");
        return;
    }

    while (fgets(linha, sizeof(linha), arquivo) != NULL) {
        resultado = sscanf(
            linha,
            "%d;%49[^;];%f;%d;%d;%d/%d/%d",
            &produto.codigo,
            produto.nome,
            &produto.preco,
            &produto.quantidade,
            &produto.estoqueMinimo,
            &produto.diaValidade,
            &produto.mesValidade,
            &produto.anoValidade
        );

        if (resultado == 8) {
            encontrou = 1;

            printf("\nCodigo: %d\n", produto.codigo);
            printf("Flor: %s\n", produto.nome);
            printf("Preco: R$ %.2f\n", produto.preco);

            if (produto.quantidade > 0) {
                printf("Disponibilidade: Disponivel\n");
            } else {
                printf("Disponibilidade: Esgotado\n");
            }

            printf("------------------------------------\n");
        }
    }

    fclose(arquivo);

    if (!encontrou) {
        printf("Nenhum produto encontrado no catalogo.\n");
    }

    printf("\nObrigado por visitar a van Gogh!\n");
}

/* =====================================
   ADICIONAR PRODUTO AO CARRINHO
   ===================================== */
int adicionarAoCarrinho(
    struct ItemCarrinho carrinho[],
    int *totalItens
) {
    FILE *arquivo;
    char linha[250];
    struct Produto produto;
    int codigoEscolhido;
    int quantidadeEscolhida;
    int resultado;
    int encontrado = 0;
    int posicao = -1;
    int quantidadeNoCarrinho = 0;
    int i;

    printf("\n====== ADICIONAR AO CARRINHO ======\n");
    printf("Digite o codigo do produto: ");

    if (scanf("%d", &codigoEscolhido) != 1) {
        printf("Codigo invalido!\n");
        while (getchar() != '\n') {}
        return 0;
    }

    arquivo = fopen("produtos.txt", "r");

    if (arquivo == NULL) {
        printf("Nenhum produto cadastrado.\n");
        return 0;
    }

    while (fgets(linha, sizeof(linha), arquivo) != NULL) {
        resultado = sscanf(
            linha,
            "%d;%49[^;];%f;%d;%d;%d/%d/%d",
            &produto.codigo,
            produto.nome,
            &produto.preco,
            &produto.quantidade,
            &produto.estoqueMinimo,
            &produto.diaValidade,
            &produto.mesValidade,
            &produto.anoValidade
        );

        if (resultado == 8 &&
            produto.codigo == codigoEscolhido) {
            encontrado = 1;
            break;
        }
    }

    fclose(arquivo);

    if (!encontrado) {
        printf("Produto nao encontrado!\n");
        return 0;
    }

    printf("\nProduto: %s\n", produto.nome);
    printf("Preco: R$ %.2f\n", produto.preco);
    printf("Estoque disponivel: %d\n", produto.quantidade);

    if (produto.quantidade <= 0) {
        printf("Produto esgotado!\n");
        return 0;
    }

    printf("Quantidade desejada: ");

    if (scanf("%d", &quantidadeEscolhida) != 1) {
        printf("Quantidade invalida!\n");
        while (getchar() != '\n') {}
        return 0;
    }

    if (quantidadeEscolhida <= 0) {
        printf("Informe uma quantidade maior que zero.\n");
        return 0;
    }

    for (i = 0; i < *totalItens; i++) {
        if (carrinho[i].produto.codigo == codigoEscolhido) {
            posicao = i;
            quantidadeNoCarrinho = carrinho[i].quantidade;
            break;
        }
    }

    if (quantidadeEscolhida + quantidadeNoCarrinho >
        produto.quantidade) {
        printf("Quantidade maior que o estoque disponivel.\n");
        return 0;
    }

    if (posicao >= 0) {
        carrinho[posicao].quantidade += quantidadeEscolhida;
    } else {
        if (*totalItens >= MAX_CARRINHO) {
            printf("Carrinho cheio!\n");
            return 0;
        }

        carrinho[*totalItens].produto = produto;
        carrinho[*totalItens].quantidade = quantidadeEscolhida;

        (*totalItens)++;
    }

    printf("\nProduto adicionado ao carrinho!\n");
    return 1;
}

/* =====================================
   CALCULAR TOTAL DO CARRINHO
   ===================================== */
float calcularTotal(
    struct ItemCarrinho carrinho[],
    int totalItens
) {
    int i;
    float total = 0.0f;

    for (i = 0; i < totalItens; i++) {
        total += carrinho[i].produto.preco *
                 carrinho[i].quantidade;
    }

    return total;
}

/* =====================================
   MOSTRAR CARRINHO
   ===================================== */
void mostrarCarrinho(
    struct ItemCarrinho carrinho[],
    int totalItens
) {
    int i;

    printf("\n====================================\n");
    printf("             SEU CARRINHO\n");
    printf("====================================\n");

    if (totalItens == 0) {
        printf("Seu carrinho esta vazio.\n");
        return;
    }

    for (i = 0; i < totalItens; i++) {
        printf("\nCodigo: %d\n",
               carrinho[i].produto.codigo);

        printf("Produto: %s\n",
               carrinho[i].produto.nome);

        printf("Quantidade: %d\n",
               carrinho[i].quantidade);

        printf("Preco unitario: R$ %.2f\n",
               carrinho[i].produto.preco);

        printf("Subtotal: R$ %.2f\n",
               carrinho[i].produto.preco *
               carrinho[i].quantidade);
    }

    printf("\n------------------------------------\n");
    printf("Total: R$ %.2f\n",
           calcularTotal(carrinho, totalItens));
}

