
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "Login.h"
#include "produtos.h"

/* =====================================
   REGISTRAR VENDA NO ARQUIVO
   ===================================== */
void registrarVenda(char cliente[], float total, char pagamento[]) {
    FILE *arquivo;
    time_t agora;
    struct tm *dataAtual;
    char data[30];

    agora = time(NULL);
    dataAtual = localtime(&agora);

    if (dataAtual == NULL) {
        printf("\nErro ao obter a data da venda.\n");
        return;
    }

    strftime(data, sizeof(data), "%d/%m/%Y %H:%M", dataAtual);

    arquivo = fopen("vendas.txt", "a");

    if (arquivo == NULL) {
        printf("\nErro ao salvar a venda!\n");
        return;
    }

    fprintf(arquivo, "%s;%.2f;%s;%s\n",
            cliente, total, pagamento, data);

    fclose(arquivo);

    printf("\nVenda registrada no sistema!\n");
}

/* =====================================
   RELATORIO GERENCIAL
   ===================================== */
void relatorioGerencial(void) {
    FILE *arquivo;
    char linha[250];

    char cliente[100];
    char pagamento[40];
    char data[30];

    float valor;
    float faturamento = 0.0f;

    int totalVendas = 0;
    int pix = 0;
    int credito = 0;
    int debito = 0;
    int dinheiro = 0;
    int resultado;

    arquivo = fopen("vendas.txt", "r");

    printf("\n====================================\n");
    printf("       FLORICULTURA VAN GOGH\n");
    printf("          RELATORIO GERENCIAL\n");
    printf("====================================\n");

    if (arquivo == NULL) {
        printf("\nNenhuma venda registrada ainda.\n");
        return;
    }

    while (fgets(linha, sizeof(linha), arquivo) != NULL) {

        resultado = sscanf(
            linha,
            "%99[^;];%f;%39[^;];%29[^\n]",
            cliente,
            &valor,
            pagamento,
            data
        );

        if (resultado == 4) {
            totalVendas++;
            faturamento += valor;

            if (strcmp(pagamento, "Pix") == 0) {
                pix++;
            } else if (strcmp(pagamento, "Cartao de credito") == 0) {
                credito++;
            } else if (strcmp(pagamento, "Cartao de debito") == 0) {
                debito++;
            } else if (strcmp(pagamento, "Dinheiro") == 0) {
                dinheiro++;
            }

            printf("\nCliente: %s\n", cliente);
            printf("Valor: R$ %.2f\n", valor);
            printf("Pagamento: %s\n", pagamento);
            printf("Data: %s\n", data);
            printf("------------------------------------\n");
        }
    }

    fclose(arquivo);

    printf("\n========== RESUMO DAS VENDAS ==========\n");
    printf("Quantidade de vendas: %d\n", totalVendas);
    printf("Faturamento total: R$ %.2f\n", faturamento);

    printf("\nVENDAS POR FORMA DE PAGAMENTO\n");
    printf("Pix: %d\n", pix);
    printf("Cartao de credito: %d\n", credito);
    printf("Cartao de debito: %d\n", debito);
    printf("Dinheiro: %d\n", dinheiro);

    printf("\n====================================\n");
}

/* =====================================
   ESCOLHER FORMA DE PAGAMENTO
   ===================================== */
int escolherPagamento(char pagamento[]) {
    int opcao;

    printf("\n====================================\n");
    printf("          FORMA DE PAGAMENTO\n");
    printf("====================================\n");

    printf("1 - Pix\n");
    printf("2 - Cartao de credito\n");
    printf("3 - Cartao de debito\n");
    printf("4 - Dinheiro\n");

    printf("\nEscolha uma opcao: ");

    if (scanf("%d", &opcao) != 1) {
        printf("\nOpcao invalida!\n");

        while (getchar() != '\n') {
        }

        return 0;
    }

    switch (opcao) {
        case 1:
            strcpy(pagamento, "Pix");
            break;

        case 2:
            strcpy(pagamento, "Cartao de credito");
            break;

        case 3:
            strcpy(pagamento, "Cartao de debito");
            break;

        case 4:
            strcpy(pagamento, "Dinheiro");
            break;

        default:
            printf("\nForma de pagamento invalida!\n");
            return 0;
    }

    return 1;
}

/* =====================================
   PROGRAMA PRINCIPAL
   ===================================== */
int main(void) {
    char usuario[50];
    char pagamento[40];

    int perfil = 0;
    int opcao;
    int resultadoLogin;
    int continuar = 1;

    float total;

    struct ItemCarrinho carrinho[MAX_CARRINHO];
    int totalItens = 0;

    strcpy(usuario, "Visitante");

    printf("\n====================================\n");
    printf("       FLORICULTURA VAN GOGH\n");
    printf("====================================\n");
    printf("Bem-vindo a nossa loja!\n");

    while (continuar) {

        printf("\n====================================\n");
        printf("       FLORICULTURA VAN GOGH\n");
        printf("====================================\n");

        if (perfil == 0) {
            printf("Perfil: VISITANTE\n");
        } else if (perfil == 1) {
            printf("Usuario: %s | Perfil: GERENTE\n", usuario);
        } else if (perfil == 2) {
            printf("Usuario: %s | Perfil: BALCONISTA\n", usuario);
        } else if (perfil == 3) {
            printf("Usuario: %s | Perfil: CLIENTE\n", usuario);
        }

        printf("\n========== MENU PRINCIPAL ==========\n");

        printf("1 - Ver catalogo e adicionar produto\n");
        printf("2 - Ver meu carrinho\n");
        printf("3 - Finalizar compra\n");
        printf("4 - Ver catalogo publico\n");

        if (perfil == 1) {
            printf("5 - Cadastrar produto\n");
            printf("6 - Consultar produtos e estoque\n");
            printf("7 - Cadastrar balconista\n");
            printf("10 - Relatorios gerenciais\n");
        }

        if (perfil == 2) {
            printf("6 - Consultar produtos e estoque\n");
        }

        if (perfil == 0) {
            printf("8 - Fazer login\n");
            printf("9 - Criar conta de cliente\n");
        }

        if (perfil == 3) {
            printf("8 - Sair da conta\n");
        }

        printf("0 - Sair da loja\n");

        printf("\nEscolha uma opcao: ");

        if (scanf("%d", &opcao) != 1) {
            printf("\nEntrada invalida!\n");

            while (getchar() != '\n') {
            }

            continue;
        }

        switch (opcao) {

            /* VER CATALOGO E ADICIONAR PRODUTO */
            case 1:
                mostrarCatalogo();

                printf("\nDeseja adicionar um produto ao carrinho?\n");
                printf("1 - Sim\n");
                printf("0 - Nao\n");
                printf("Escolha: ");

                if (scanf("%d", &opcao) != 1) {
                    printf("Opcao invalida!\n");

                    while (getchar() != '\n') {
                    }

                    break;
                }

                if (opcao == 1) {
                    adicionarAoCarrinho(carrinho, &totalItens);
                }

                break;

            /* MOSTRAR CARRINHO */
            case 2:
                mostrarCarrinho(carrinho, totalItens);
                break;

            /* FINALIZAR COMPRA */
            case 3:

                printf("\n========== FINALIZAR COMPRA ==========\n");

                if (totalItens == 0) {
                    printf("Seu carrinho esta vazio!\n");
                    break;
                }

                mostrarCarrinho(carrinho, totalItens);

                /* Exige login */
                if (perfil == 0) {
                    printf("\nPara finalizar a compra, faca login.\n");

                    resultadoLogin = cadastroELogin(
                        usuario,
                        sizeof(usuario),
                        &perfil
                    );

                    if (!resultadoLogin) {
                        strcpy(usuario, "Visitante");
                        perfil = 0;

                        printf("\nLogin nao realizado.\n");
                        printf("Sua compra continua no carrinho.\n");
                        break;
                    }
                }

                /* Somente cliente pode comprar */
                if (perfil != 3) {
                    printf("\nSomente uma conta de cliente");
                    printf(" pode finalizar a compra.\n");

                    if (perfil == 1 || perfil == 2) {
                        printf("Entre com uma conta de cliente.\n");
                    }

                    break;
                }

                total = calcularTotal(carrinho, totalItens);

                printf("\nCliente: %s\n", usuario);
                printf("Total: R$ %.2f\n", total);

                /* Escolher pagamento */
                if (!escolherPagamento(pagamento)) {
                    printf("\nCompra nao finalizada.\n");
                    break;
                }

                printf("\n====================================\n");
                printf("         CONFIRMACAO DA COMPRA\n");
                printf("====================================\n");

                printf("Cliente: %s\n", usuario);
                printf("Valor: R$ %.2f\n", total);
                printf("Pagamento: %s\n", pagamento);

                printf("\nConfirmar compra?\n");
                printf("1 - Confirmar\n");
                printf("0 - Cancelar\n");
                printf("Escolha: ");

                if (scanf("%d", &opcao) != 1) {
                    printf("Opcao invalida!\n");

                    while (getchar() != '\n') {
                    }

                    break;
                }

                if (opcao == 1) {

                    registrarVenda(usuario, total, pagamento);

                    printf("\nCompra finalizada com sucesso!\n");
                    printf("Obrigado por comprar na van Gogh!\n");

                    /* Esvazia o carrinho */
                    totalItens = 0;

                } else {
                    printf("\nCompra cancelada.\n");
                    printf("Seus produtos continuam no carrinho.\n");
                }

                break;

            /* CATALOGO PUBLICO */
            case 4:
                mostrarCatalogo();
                break;

            /* CADASTRAR PRODUTO */
            case 5:
                if (perfil == 1) {
                    cadastrarProduto();
                } else {
                    printf("\nSomente o gerente pode cadastrar produtos.\n");
                }
                break;

            /* CONSULTAR ESTOQUE */
            case 6:
                if (perfil == 1 || perfil == 2) {
                    consultarProdutos();
                } else {
                    printf("\nAcesso negado!\n");
                }
                break;

            /* CADASTRAR BALCONISTA */
            case 7:
                if (perfil == 1) {
                    cadastrarBalconista();
                } else {
                    printf("\nSomente o gerente pode cadastrar balconistas.\n");
                }
                break;

            /* LOGIN OU SAIR DA CONTA */
            case 8:

                if (perfil == 0) {

                    resultadoLogin = cadastroELogin(
                        usuario,
                        sizeof(usuario),
                        &perfil
                    );

                    if (resultadoLogin) {
                        printf("\nBem-vindo(a), %s!\n", usuario);
                    } else {
                        strcpy(usuario, "Visitante");
                        perfil = 0;

                        printf("\nVoce continua como visitante.\n");
                    }

                } else if (perfil == 3) {

                    strcpy(usuario, "Visitante");
                    perfil = 0;

                    printf("\nVoce saiu da sua conta.\n");
                    printf("Seu carrinho continua disponivel.\n");

                } else {
                    printf("\nPara trocar de conta administrativa,");
                    printf(" encerre o programa e entre novamente.\n");
                }

                break;

            /* CADASTRO DE CLIENTE */
            case 9:
                if (perfil == 0) {
                    cadastrarCliente();

                    printf("\nAgora voce pode fazer login.\n");
                } else {
                    printf("\nOpcao disponivel somente para visitantes.\n");
                }
                break;

            /* RELATORIO GERENCIAL */
            case 10:
                if (perfil == 1) {
                    relatorioGerencial();
                } else {
                    printf("\nAcesso negado!\n");
                    printf("Somente o gerente pode acessar os relatorios.\n");
                }
                break;

            /* SAIR */
            case 0:
                printf("\nSaindo da Floricultura van Gogh...\n");
                continuar = 0;
                break;

            default:
                printf("\nOpcao invalida! Tente novamente.\n");
                break;
        }
    }

    if (totalItens > 0) {
        printf("\n====================================\n");
        printf("       CARRINHO NAO FINALIZADO\n");
        printf("====================================\n");

        mostrarCarrinho(carrinho, totalItens);

        printf("\nEsses produtos nao foram comprados.\n");
    }

    printf("\nObrigado por visitar a Floricultura van Gogh!\n");
    printf("Volte sempre!\n");

    return 0;
}

