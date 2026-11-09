
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <ctype.h>

#include "produtos.h"

/* =====================================
   CORES DO CATALOGO VAN GOGH
   ===================================== */

#define CAT_VERDE (FOREGROUND_GREEN | FOREGROUND_INTENSITY)
#define CAT_DOURADO (FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY)
#define CAT_BRANCO (FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY)
#define CAT_VERMELHO (FOREGROUND_RED | FOREGROUND_INTENSITY)

void corCatalogo(WORD cor) {
    HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(console, cor);
}

/* =====================================
   LER CAMPO E IDENTIFICAR COMANDOS
   ===================================== */

int lerCampo(char texto[], int tamanho, const char mensagem[]) {

    printf("%s", mensagem);

    if (fgets(texto, tamanho, stdin) == NULL) {
        return NAV_VOLTAR;
    }

    texto[strcspn(texto, "\n")] = '\0';

    if (strcmp(texto, "0") == 0) {
        printf("\nOperacao cancelada.\n");
        return NAV_VOLTAR;
    }

    if (strcmp(texto, "9") == 0) {
        return NAV_LOGOUT;
    }

    if (strcmp(texto, "8") == 0) {
        return NAV_SAIR;
    }

    return NAV_OK;
}

/* =====================================
   LER NUMERO INTEIRO
   ===================================== */

int lerInteiro(int *numero, const char mensagem[]) {

    char texto[100];
    char extra;
    int resultado;

    resultado = lerCampo(texto, sizeof(texto), mensagem);

    if (resultado != NAV_OK) {
        return resultado;
    }

    if (sscanf(texto, "%d %c", numero, &extra) != 1) {
        printf("\nDigite um numero valido!\n");
        return NAV_VOLTAR;
    }

    return NAV_OK;
}

/* =====================================
   LER PRECO
   ===================================== */

int lerPreco(float *preco, const char mensagem[]) {

    char texto[100];
    char extra;
    int resultado;
    int i;

    resultado = lerCampo(texto, sizeof(texto), mensagem);

    if (resultado != NAV_OK) {
        return resultado;
    }

    for (i = 0; texto[i] != '\0'; i++) {

        if (texto[i] == ',') {
            texto[i] = '.';
        }
    }

    if (sscanf(texto, "%f %c", preco, &extra) != 1) {
        printf("\nDigite um preco valido!\n");
        return NAV_VOLTAR;
    }

    return NAV_OK;
}

/* =====================================
   LER PRODUTO DO ARQUIVO
   ===================================== */

int lerProduto(FILE *arquivo, struct Produto *produto) {

    char linha[300];

    while (fgets(linha, sizeof(linha), arquivo) != NULL) {

        if (sscanf(
            linha,
            "%d;%49[^;];%f;%d;%d;%d/%d/%d",
            &produto->codigo,
            produto->nome,
            &produto->preco,
            &produto->quantidade,
            &produto->estoqueMinimo,
            &produto->diaValidade,
            &produto->mesValidade,
            &produto->anoValidade
        ) == 8) {

            return 1;
        }
    }

    return 0;
}

/* =====================================
   COMPARAR NOMES DAS FLORES
   ===================================== */

int nomesIguais(const char nome1[], const char nome2[]) {

    int i = 0;

    while (nome1[i] != '\0' && nome2[i] != '\0') {

        if (tolower((unsigned char)nome1[i]) !=
            tolower((unsigned char)nome2[i])) {

            return 0;
        }

        i++;
    }

    return nome1[i] == '\0' && nome2[i] == '\0';
}

/* =====================================
   CADASTRAR PRODUTO
   ===================================== */

int cadastrarProduto(void) {

    FILE *arquivo;
    struct Produto produto;
    struct Produto existente;

    int resultado;
    int encontrado = 0;

    arquivo = fopen("produtos.txt", "a+");

    if (arquivo == NULL) {
        printf("\nErro ao abrir produtos.txt!\n");
        return NAV_VOLTAR;
    }

    resultado = lerInteiro(
        &produto.codigo,
        "\nCodigo do produto (0 voltar, 9 sair da conta, 8 encerrar): "
    );

    if (resultado != NAV_OK) {
        fclose(arquivo);
        return resultado;
    }

    if (produto.codigo <= 0) {
        printf("\nCodigo invalido!\n");
        fclose(arquivo);
        return NAV_VOLTAR;
    }

    rewind(arquivo);

    while (lerProduto(arquivo, &existente)) {

        if (existente.codigo == produto.codigo) {
            encontrado = 1;
            break;
        }
    }

    if (encontrado) {
        printf("\nEsse codigo ja esta cadastrado!\n");
        fclose(arquivo);
        return NAV_VOLTAR;
    }

    resultado = lerCampo(
        produto.nome,
        sizeof(produto.nome),
        "Nome da flor: "
    );

    if (resultado != NAV_OK) {
        fclose(arquivo);
        return resultado;
    }

    resultado = lerPreco(&produto.preco, "Preco: R$ ");

    if (resultado != NAV_OK) {
        fclose(arquivo);
        return resultado;
    }

    resultado = lerInteiro(
        &produto.quantidade,
        "Quantidade em estoque: "
    );

    if (resultado != NAV_OK) {
        fclose(arquivo);
        return resultado;
    }

    resultado = lerInteiro(
        &produto.estoqueMinimo,
        "Estoque minimo: "
    );

    if (resultado != NAV_OK) {
        fclose(arquivo);
        return resultado;
    }

    resultado = lerInteiro(
        &produto.diaValidade,
        "Dia de validade: "
    );

    if (resultado != NAV_OK) {
        fclose(arquivo);
        return resultado;
    }

    resultado = lerInteiro(
        &produto.mesValidade,
        "Mes de validade: "
    );

    if (resultado != NAV_OK) {
        fclose(arquivo);
        return resultado;
    }

    resultado = lerInteiro(
        &produto.anoValidade,
        "Ano de validade: "
    );

    if (resultado != NAV_OK) {
        fclose(arquivo);
        return resultado;
    }

    if (strlen(produto.nome) == 0 ||
        produto.preco < 0 ||
        produto.quantidade < 0 ||
        produto.estoqueMinimo < 0 ||
        produto.diaValidade < 1 ||
        produto.diaValidade > 31 ||
        produto.mesValidade < 1 ||
        produto.mesValidade > 12 ||
        produto.anoValidade < 1) {

        printf("\nDados invalidos!\n");
        fclose(arquivo);
        return NAV_VOLTAR;
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

    return NAV_OK;
}

/* =====================================
   ALTERAR PRODUTO
   ===================================== */

int alterarProduto(void) {

    FILE *arquivo;
    FILE *temporario;

    struct Produto produto;

    int codigo;
    int encontrado = 0;
    int resultado;

    char novoNome[50];
    float novoPreco;

    arquivo = fopen("produtos.txt", "r");

    if (arquivo == NULL) {
        printf("\nNenhum produto cadastrado!\n");
        return NAV_VOLTAR;
    }

    temporario = fopen("produtos_temp.txt", "w");

    if (temporario == NULL) {
        fclose(arquivo);
        return NAV_VOLTAR;
    }

    resultado = lerInteiro(
        &codigo,
        "\nCodigo do produto que deseja alterar: "
    );

    if (resultado != NAV_OK) {
        fclose(arquivo);
        fclose(temporario);
        remove("produtos_temp.txt");
        return resultado;
    }

    while (lerProduto(arquivo, &produto)) {

        if (produto.codigo == codigo) {

            encontrado = 1;

            printf("\nProduto encontrado: %s\n", produto.nome);

            resultado = lerCampo(
                novoNome,
                sizeof(novoNome),
                "Novo nome: "
            );

            if (resultado != NAV_OK) {
                fclose(arquivo);
                fclose(temporario);
                remove("produtos_temp.txt");
                return resultado;
            }

            resultado = lerPreco(&novoPreco, "Novo preco: R$ ");

            if (resultado != NAV_OK) {
                fclose(arquivo);
                fclose(temporario);
                remove("produtos_temp.txt");
                return resultado;
            }

            if (strlen(novoNome) == 0 || novoPreco < 0) {

                printf("\nDados invalidos!\n");

                fclose(arquivo);
                fclose(temporario);

                remove("produtos_temp.txt");

                return NAV_VOLTAR;
            }

            strcpy(produto.nome, novoNome);
            produto.preco = novoPreco;
        }

        fprintf(
            temporario,
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
    }

    fclose(arquivo);
    fclose(temporario);

    if (!encontrado) {

        printf("\nProduto nao encontrado!\n");

        remove("produtos_temp.txt");

        return NAV_VOLTAR;
    }

    remove("produtos_backup.txt");

    if (rename("produtos.txt", "produtos_backup.txt") != 0) {

        remove("produtos_temp.txt");

        printf("\nErro ao criar backup!\n");

        return NAV_VOLTAR;
    }

    if (rename("produtos_temp.txt", "produtos.txt") != 0) {

        rename("produtos_backup.txt", "produtos.txt");

        printf("\nErro ao atualizar produto!\n");

        return NAV_VOLTAR;
    }

    printf("\nProduto alterado com sucesso!\n");

    return NAV_OK;
}

/* =====================================
   REMOVER PRODUTO DO CATALOGO
   ===================================== */

int removerProduto(void) {

    FILE *arquivo;
    FILE *temporario;

    char linha[300];
    char nomeProduto[50];
    char nomeEncontrado[50];

    int codigo;
    int codigoLinha;
    int encontrado = 0;
    int resultado;
    int confirmacao;

    /* LOCALIZAR PRODUTO */

    arquivo = fopen("produtos.txt", "r");

    if (arquivo == NULL) {

        printf("\nNenhum produto cadastrado!\n");

        return NAV_VOLTAR;
    }

    resultado = lerInteiro(
        &codigo,
        "\nDigite o codigo da flor que deseja remover: "
    );

    if (resultado != NAV_OK) {

        fclose(arquivo);

        return resultado;
    }

    if (codigo <= 0) {

        printf("\nCodigo invalido!\n");

        fclose(arquivo);

        return NAV_VOLTAR;
    }

    while (fgets(linha, sizeof(linha), arquivo) != NULL) {

        if (sscanf(
            linha,
            "%d;%49[^;]",
            &codigoLinha,
            nomeProduto
        ) == 2) {

            if (codigoLinha == codigo) {

                encontrado = 1;

                strcpy(nomeEncontrado, nomeProduto);

                break;
            }
        }
    }

    fclose(arquivo);

    if (!encontrado) {

        printf("\nFlor nao encontrada no catalogo!\n");

        return NAV_VOLTAR;
    }

    /* CONFIRMAR EXCLUSAO */

    printf("\nFlor encontrada: %s\n", nomeEncontrado);
    printf("Codigo: %d\n", codigo);

    printf("\nDeseja realmente remover essa flor?\n");
    printf("1 - Sim, remover\n");
    printf("2 - Nao, cancelar\n");

    resultado = lerInteiro(
        &confirmacao,
        "\nEscolha uma opcao: "
    );

    if (resultado != NAV_OK) {
        return resultado;
    }

    if (confirmacao == 2) {

        printf("\nRemocao cancelada!\n");

        return NAV_VOLTAR;
    }

    if (confirmacao != 1) {

        printf("\nOpcao invalida. Remocao cancelada!\n");

        return NAV_VOLTAR;
    }

    /* CRIAR ARQUIVO TEMPORARIO */

    arquivo = fopen("produtos.txt", "r");

    if (arquivo == NULL) {

        printf("\nErro ao abrir o catalogo!\n");

        return NAV_VOLTAR;
    }

    temporario = fopen("produtos_temp.txt", "w");

    if (temporario == NULL) {

        fclose(arquivo);

        printf("\nErro ao criar arquivo temporario!\n");

        return NAV_VOLTAR;
    }

    /* COPIAR TODOS OS PRODUTOS, EXCETO O ESCOLHIDO */

    while (fgets(linha, sizeof(linha), arquivo) != NULL) {

        if (sscanf(linha, "%d;", &codigoLinha) == 1 &&
            codigoLinha == codigo) {

            continue;
        }

        fputs(linha, temporario);
    }

    fclose(arquivo);
    fclose(temporario);

    /* CRIAR BACKUP */

    remove("produtos_backup.txt");

    if (rename("produtos.txt", "produtos_backup.txt") != 0) {

        remove("produtos_temp.txt");

        printf("\nErro ao criar backup do catalogo!\n");

        return NAV_VOLTAR;
    }

    /* ATUALIZAR CATALOGO */

    if (rename("produtos_temp.txt", "produtos.txt") != 0) {

        rename("produtos_backup.txt", "produtos.txt");

        remove("produtos_temp.txt");

        printf("\nErro ao atualizar o catalogo!\n");

        return NAV_VOLTAR;
    }

    corCatalogo(CAT_VERDE);

    printf("\nFlor removida do catalogo com sucesso!\n");

    corCatalogo(CAT_BRANCO);

    return NAV_OK;
}


/* =====================================
   CONSULTAR PRODUTOS
   ===================================== */

int consultarProdutos(void) {

    FILE *arquivo;
    struct Produto produto;

    arquivo = fopen("produtos.txt", "r");

    if (arquivo == NULL) {
        printf("\nNenhum produto cadastrado!\n");
        return NAV_VOLTAR;
    }

    printf("\n  PRODUTOS CADASTRADOS\n");

    while (lerProduto(arquivo, &produto)) {

        printf("\nCodigo: %d", produto.codigo);
        printf("\nFlor: %s", produto.nome);
        printf("\nPreco: R$ %.2f", produto.preco);
        printf("\nEstoque: %d", produto.quantidade);
        printf("\nEstoque minimo: %d", produto.estoqueMinimo);

        printf(
            "\nValidade: %02d/%02d/%04d",
            produto.diaValidade,
            produto.mesValidade,
            produto.anoValidade
        );

        if (produto.quantidade <= produto.estoqueMinimo) {

            corCatalogo(CAT_VERMELHO);
            printf("\nALERTA: Estoque baixo!");

            corCatalogo(CAT_BRANCO);
        }

        printf("\n------------------------------------------\n");
    }

    fclose(arquivo);

    return NAV_OK;
}

/* =====================================
   MOSTRAR CATALOGO - VITRINE
   ===================================== */

int mostrarCatalogo(void) {

    FILE *arquivo;

    struct Produto produto1;
    struct Produto produto2;

    int temProduto1;
    int temProduto2;

    arquivo = fopen("produtos.txt", "r");

    if (arquivo == NULL) {

        corCatalogo(CAT_VERMELHO);
        printf("\nCatalogo indisponivel!\n");

        corCatalogo(CAT_BRANCO);

        return NAV_VOLTAR;
    }

    system("cls");

    corCatalogo(CAT_DOURADO);

    printf("\n");
    printf("  ============================================================\n");
    printf("                         VAN GOGH\n");
    printf("                    FLORICULTURA\n");
    printf("  ============================================================\n");

    corCatalogo(CAT_BRANCO);

    printf("\n                 FLORES QUE ENCANTAM MOMENTOS\n");
    printf("                         NOSSA VITRINE\n\n");

    corCatalogo(CAT_VERDE);

    printf("  +----------------------------+  +----------------------------+\n");

    while (1) {

        temProduto1 = lerProduto(arquivo, &produto1);

        if (!temProduto1) {
            break;
        }

        temProduto2 = lerProduto(arquivo, &produto2);

        /* CODIGOS */

        corCatalogo(CAT_VERDE);
        printf("  |");

        corCatalogo(CAT_DOURADO);
        printf(" CODIGO: %-18d", produto1.codigo);

        corCatalogo(CAT_VERDE);
        printf("|  |");

        if (temProduto2) {

            corCatalogo(CAT_DOURADO);
            printf(" CODIGO: %-18d", produto2.codigo);

        } else {
            printf("                            ");
        }

        corCatalogo(CAT_VERDE);
        printf("|\n  |");

        /* NOMES */

        corCatalogo(CAT_BRANCO);
        printf(" %-26.26s", produto1.nome);

        corCatalogo(CAT_VERDE);
        printf("|  |");

        if (temProduto2) {

            corCatalogo(CAT_BRANCO);
            printf(" %-26.26s", produto2.nome);

        } else {
            printf("                            ");
        }

        corCatalogo(CAT_VERDE);
        printf("|\n  |");

        /* PRECOS */

        corCatalogo(CAT_DOURADO);
        printf(" PRECO: R$ %-16.2f", produto1.preco);

        corCatalogo(CAT_VERDE);
        printf("|  |");

        if (temProduto2) {

            corCatalogo(CAT_DOURADO);
            printf(" PRECO: R$ %-16.2f", produto2.preco);

        } else {
            printf("                            ");
        }

        corCatalogo(CAT_VERDE);
        printf("|\n  |");

        /* DISPONIBILIDADE */

        if (produto1.quantidade > 0) {

            corCatalogo(CAT_VERDE);
            printf(" DISPONIVEL                ");

        } else {

            corCatalogo(CAT_VERMELHO);
            printf(" ESGOTADO                  ");
        }

        corCatalogo(CAT_VERDE);
        printf("|  |");

        if (temProduto2) {

            if (produto2.quantidade > 0) {

                corCatalogo(CAT_VERDE);
                printf(" DISPONIVEL                ");

            } else {

                corCatalogo(CAT_VERMELHO);
                printf(" ESGOTADO                  ");
            }

        } else {
            printf("                            ");
        }

        corCatalogo(CAT_VERDE);

        printf("|\n");

        printf("  +----------------------------+  +----------------------------+\n");
    }

    corCatalogo(CAT_DOURADO);

    printf("\n                   VAN GOGH - FLORES COM ARTE\n");

    corCatalogo(CAT_BRANCO);

    printf("\n  Consulte nossos produtos e escolha suas flores!\n");

    corCatalogo(CAT_VERDE);

    printf("\n  ============================================================\n");

    corCatalogo(CAT_BRANCO);

    fclose(arquivo);

    return NAV_OK;
}

/* =====================================
   ADICIONAR AO CARRINHO PELO NOME
   ===================================== */

int adicionarAoCarrinho(
    struct ItemCarrinho carrinho[],
    int *totalItens
) {

    FILE *arquivo;

    struct Produto produto;

    char nomeFlor[50];

    int quantidade;
    int resultado;
    int encontrado = 0;
    int i;

    resultado = mostrarCatalogo();

    if (resultado != NAV_OK) {
        return resultado;
    }

    resultado = lerCampo(
        nomeFlor,
        sizeof(nomeFlor),
        "\nQual flor voce deseja adicionar ao carrinho? "
    );

    if (resultado != NAV_OK) {
        return resultado;
    }

    arquivo = fopen("produtos.txt", "r");

    if (arquivo == NULL) {

        printf("\nErro ao consultar os produtos!\n");

        return NAV_VOLTAR;
    }

    while (lerProduto(arquivo, &produto)) {

        if (nomesIguais(produto.nome, nomeFlor)) {

            encontrado = 1;

            break;
        }
    }

    fclose(arquivo);

    if (!encontrado) {

        corCatalogo(CAT_VERMELHO);

        printf("\nNao encontramos essa flor em nosso catalogo!\n");

        corCatalogo(CAT_BRANCO);

        return NAV_VOLTAR;
    }

    if (produto.quantidade <= 0) {

        corCatalogo(CAT_VERMELHO);

        printf("\nEssa flor esta esgotada!\n");

        corCatalogo(CAT_BRANCO);

        return NAV_VOLTAR;
    }

    printf("\nFlor selecionada: %s\n", produto.nome);
    printf("Preco: R$ %.2f\n", produto.preco);

    resultado = lerInteiro(
        &quantidade,
        "Quantas unidades voce deseja adicionar? "
    );

    if (resultado != NAV_OK) {
        return resultado;
    }

    if (quantidade <= 0) {

        printf("\nQuantidade invalida!\n");

        return NAV_VOLTAR;
    }

    for (i = 0; i < *totalItens; i++) {

        if (carrinho[i].produto.codigo == produto.codigo) {

            if (carrinho[i].quantidade + quantidade >
                produto.quantidade) {

                corCatalogo(CAT_VERMELHO);

                printf("\nEstoque insuficiente!\n");

                corCatalogo(CAT_BRANCO);

                return NAV_VOLTAR;
            }

            carrinho[i].quantidade += quantidade;

            corCatalogo(CAT_VERDE);

            printf("\nQuantidade atualizada no carrinho!\n");

            corCatalogo(CAT_BRANCO);

            return NAV_OK;
        }
    }

    if (*totalItens >= MAX_CARRINHO) {

        printf("\nCarrinho cheio!\n");

        return NAV_VOLTAR;
    }

    if (quantidade > produto.quantidade) {

        corCatalogo(CAT_VERMELHO);

        printf("\nEstoque insuficiente!\n");

        corCatalogo(CAT_BRANCO);

        return NAV_VOLTAR;
    }

    carrinho[*totalItens].produto = produto;

    carrinho[*totalItens].quantidade = quantidade;

    (*totalItens)++;

    corCatalogo(CAT_VERDE);

    printf("\n%s foi adicionada ao seu carrinho!\n", produto.nome);

    corCatalogo(CAT_BRANCO);

    return NAV_OK;
}

/* =====================================
   MOSTRAR CARRINHO
   ===================================== */

void mostrarCarrinho(
    struct ItemCarrinho carrinho[],
    int totalItens
) {

    int i;
    float subtotal;

    corCatalogo(CAT_DOURADO);

    printf("\n");
    printf("  ============================================================\n");
    printf("                         VAN GOGH\n");
    printf("                        MEU CARRINHO\n");
    printf("  ============================================================\n");

    corCatalogo(CAT_BRANCO);

    printf("\n              REVISE SEUS PRODUTOS ANTES DE CONTINUAR\n\n");

    if (totalItens == 0) {

        corCatalogo(CAT_VERMELHO);

        printf("                 SEU CARRINHO ESTA VAZIO!\n");

        corCatalogo(CAT_BRANCO);

        printf("\n              Que tal escolher algumas flores?\n");

        corCatalogo(CAT_VERDE);

        printf("\n  ============================================================\n");

        corCatalogo(CAT_BRANCO);

        return;
    }

    corCatalogo(CAT_VERDE);

    printf("  ------------------------------------------------------------\n");

    corCatalogo(CAT_DOURADO);

    printf("  PRODUTO                    QTD.       PRECO       SUBTOTAL\n");

    corCatalogo(CAT_VERDE);

    printf("  ------------------------------------------------------------\n");

    for (i = 0; i < totalItens; i++) {

        subtotal =
            carrinho[i].produto.preco *
            carrinho[i].quantidade;

        corCatalogo(CAT_BRANCO);

        printf("  %-25.25s", carrinho[i].produto.nome);

        printf("  %3d", carrinho[i].quantidade);

        corCatalogo(CAT_DOURADO);

        printf("    R$ %7.2f", carrinho[i].produto.preco);

        printf("    R$ %8.2f\n", subtotal);

        corCatalogo(CAT_VERDE);

        printf("  ------------------------------------------------------------\n");
    }

    corCatalogo(CAT_DOURADO);

    printf("\n                    TOTAL DA COMPRA\n");

    printf("                    R$ %.2f\n",
        calcularTotal(carrinho, totalItens));

    corCatalogo(CAT_VERDE);

    printf("\n  ============================================================\n");

    corCatalogo(CAT_BRANCO);

    printf("\n               Obrigado por escolher a VAN GOGH!\n\n");
}

/* =====================================
   CALCULAR TOTAL
   ===================================== */

float calcularTotal(
    struct ItemCarrinho carrinho[],
    int totalItens
) {

    float total = 0;
    int i;

    for (i = 0; i < totalItens; i++) {

        total += carrinho[i].produto.preco *
                 carrinho[i].quantidade;
    }

    return total;
}

/* =====================================
   CONSULTAR ESTOQUE ATUAL
   ===================================== */

int consultarEstoqueAtual(int codigo, int *estoque) {

    FILE *arquivo;

    struct Produto produto;

    arquivo = fopen("produtos.txt", "r");

    if (arquivo == NULL) {
        return 0;
    }

    while (lerProduto(arquivo, &produto)) {

        if (produto.codigo == codigo) {

            *estoque = produto.quantidade;

            fclose(arquivo);

            return 1;
        }
    }

    fclose(arquivo);

    return 0;
}

/* =====================================
   REMOVER ITEM DO CARRINHO
   ===================================== */

void removerItemCarrinho(
    struct ItemCarrinho carrinho[],
    int *totalItens,
    int posicao
) {

    int i;

    for (i = posicao; i < *totalItens - 1; i++) {

        carrinho[i] = carrinho[i + 1];
    }

    (*totalItens)--;
}

/* =====================================
   GERENCIAR CARRINHO PELO NOME
   ===================================== */

int gerenciarCarrinho(
    struct ItemCarrinho carrinho[],
    int *totalItens
) {

    int opcao;
    int resultado;
    int estoque;
    int posicao;
    int encontrado;

    char texto[50];
    char extra;
    char nomeFlor[50];

    while (1) {

        system("cls");

        mostrarCarrinho(carrinho, *totalItens);

        corCatalogo(CAT_DOURADO);

        printf("\n                  GERENCIAR MEU CARRINHO\n");

        corCatalogo(CAT_BRANCO);

        printf("\n  [1] Aumentar quantidade\n");
        printf("  [2] Diminuir quantidade\n");
        printf("  [3] Remover produto\n");

        printf("\n  [0] Voltar a loja\n");
        printf("  [9] Sair da conta\n");
        printf("  [8] Encerrar programa\n");

        printf("\n  Escolha uma opcao: ");

        if (fgets(texto, sizeof(texto), stdin) == NULL) {
            return NAV_VOLTAR;
        }

        if (sscanf(texto, "%d %c", &opcao, &extra) != 1) {

            corCatalogo(CAT_VERMELHO);

            printf("\n  Opcao invalida!\n");

            corCatalogo(CAT_BRANCO);

            system("pause");

            continue;
        }

        if (opcao == 0) {
            return NAV_VOLTAR;
        }

        if (opcao == 9) {
            return NAV_LOGOUT;
        }

        if (opcao == 8) {
            return NAV_SAIR;
        }

        if (opcao < 1 || opcao > 3) {

            corCatalogo(CAT_VERMELHO);

            printf("\n  Opcao invalida!\n");

            corCatalogo(CAT_BRANCO);

            system("pause");

            continue;
        }

        if (*totalItens == 0) {

            corCatalogo(CAT_VERMELHO);

            printf("\n  Adicione produtos antes de continuar!\n");

            corCatalogo(CAT_BRANCO);

            system("pause");

            continue;
        }

        /* ESCOLHER PRODUTO PELO NOME */

        resultado = lerCampo(
            nomeFlor,
            sizeof(nomeFlor),
            "\n  Qual flor voce deseja alterar? Digite o nome: "
        );

        if (resultado != NAV_OK) {
            return resultado;
        }

        encontrado = 0;

        for (posicao = 0; posicao < *totalItens; posicao++) {

            if (nomesIguais(
                carrinho[posicao].produto.nome,
                nomeFlor
            )) {

                encontrado = 1;

                break;
            }
        }

        if (!encontrado) {

            corCatalogo(CAT_VERMELHO);

            printf("\n  Essa flor nao esta no seu carrinho!\n");

            corCatalogo(CAT_BRANCO);

            system("pause");

            continue;
        }

        /* AUMENTAR QUANTIDADE */

        if (opcao == 1) {

            resultado = consultarEstoqueAtual(
                carrinho[posicao].produto.codigo,
                &estoque
            );

            if (!resultado) {

                corCatalogo(CAT_VERMELHO);

                printf("\n  Erro ao consultar estoque!\n");

                corCatalogo(CAT_BRANCO);

                system("pause");

                continue;
            }

            if (carrinho[posicao].quantidade >= estoque) {

                corCatalogo(CAT_VERMELHO);

                printf("\n  Estoque insuficiente!\n");

                corCatalogo(CAT_BRANCO);

                system("pause");

                continue;
            }

            carrinho[posicao].quantidade++;

            corCatalogo(CAT_VERDE);

            printf("\n  Quantidade aumentada com sucesso!\n");

            corCatalogo(CAT_BRANCO);

            system("pause");
        }

        /* DIMINUIR QUANTIDADE */

        else if (opcao == 2) {

            carrinho[posicao].quantidade--;

            if (carrinho[posicao].quantidade == 0) {

                removerItemCarrinho(
                    carrinho,
                    totalItens,
                    posicao
                );

                corCatalogo(CAT_DOURADO);

                printf("\n  Produto removido do carrinho.\n");

            } else {

                corCatalogo(CAT_VERDE);

                printf("\n  Quantidade diminuida com sucesso!\n");
            }

            corCatalogo(CAT_BRANCO);

            system("pause");
        }

        /* REMOVER PRODUTO */

        else if (opcao == 3) {

            removerItemCarrinho(
                carrinho,
                totalItens,
                posicao
            );

            corCatalogo(CAT_VERDE);

            printf("\n  Produto removido do carrinho!\n");

            corCatalogo(CAT_BRANCO);

            system("pause");
        }
    }
}

/* =====================================
   ATUALIZAR ESTOQUE APOS A VENDA
   ===================================== */

int atualizarEstoque(
    struct ItemCarrinho carrinho[],
    int totalItens
) {

    FILE *arquivo;
    FILE *temporario;

    char linha[300];

    int codigo;
    int quantidade;
    int estoqueMinimo;
    int dia, mes, ano;

    int resultado;
    int i;

    int atualizado[MAX_CARRINHO] = {0};

    char nome[50];
    float preco;

    if (totalItens <= 0 || totalItens > MAX_CARRINHO) {

        printf("\nCarrinho vazio ou invalido!\n");

        return NAV_VOLTAR;
    }

    arquivo = fopen("produtos.txt", "r");

    if (arquivo == NULL) {

        printf("\nErro ao abrir produtos.txt!\n");

        return NAV_VOLTAR;
    }

    temporario = fopen("produtos_temp.txt", "w");

    if (temporario == NULL) {

        fclose(arquivo);

        printf("\nErro ao criar arquivo temporario!\n");

        return NAV_VOLTAR;
    }

    while (fgets(linha, sizeof(linha), arquivo) != NULL) {

        resultado = sscanf(
            linha,
            "%d;%49[^;];%f;%d;%d;%d/%d/%d",
            &codigo,
            nome,
            &preco,
            &quantidade,
            &estoqueMinimo,
            &dia,
            &mes,
            &ano
        );

        if (resultado == 8) {

            for (i = 0; i < totalItens; i++) {

                if (carrinho[i].produto.codigo == codigo) {

                    if (quantidade < carrinho[i].quantidade) {

                        printf(
                            "\nEstoque insuficiente para %s!\n",
                            nome
                        );

                        fclose(arquivo);
                        fclose(temporario);

                        remove("produtos_temp.txt");

                        return NAV_VOLTAR;
                    }

                    quantidade -= carrinho[i].quantidade;

                    atualizado[i] = 1;

                    break;
                }
            }

            fprintf(
                temporario,
                "%d;%s;%.2f;%d;%d;%02d/%02d/%04d\n",
                codigo,
                nome,
                preco,
                quantidade,
                estoqueMinimo,
                dia,
                mes,
                ano
            );

        } else {

            fputs(linha, temporario);
        }
    }

    fclose(arquivo);
    fclose(temporario);

    for (i = 0; i < totalItens; i++) {

        if (!atualizado[i]) {

            printf(
                "\nProduto codigo %d nao encontrado!\n",
                carrinho[i].produto.codigo
            );

            remove("produtos_temp.txt");

            return NAV_VOLTAR;
        }
    }

    remove("produtos_backup.txt");

    if (rename("produtos.txt", "produtos_backup.txt") != 0) {

        printf("\nErro ao criar backup do estoque!\n");

        remove("produtos_temp.txt");

        return NAV_VOLTAR;
    }

    if (rename("produtos_temp.txt", "produtos.txt") != 0) {

        printf("\nErro ao atualizar estoque!\n");

        rename("produtos_backup.txt", "produtos.txt");

        remove("produtos_temp.txt");

        return NAV_VOLTAR;
    }

    printf("\nEstoque atualizado com sucesso!\n");

    return NAV_OK;
}

