
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Login.h"
#include "produtos.h"

int main(void) {
    char usuario[50];
    int perfil = 0;
    int opcao;
    int resultadoLogin;
    int continuar = 1;

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
        }

        if (perfil == 2) {
            printf("6 - Consultar produtos e estoque\n");
        }

        if (perfil == 0) {
            printf("8 - Fazer login\n");
            printf("9 - Criar conta de cliente\n");
        } else if (perfil == 3) {
            printf("8 - Sair da conta\n");
        }

        printf("0 - Sair da loja\n");
        printf("\nEscolha uma opcao: ");

        if (scanf("%d", &opcao) != 1) {
            printf("\nEntrada invalida!\n");

            while (getchar() != '\n') {
                /* Limpa a entrada incorreta */
            }

            continue;
        }

        switch (opcao) {
            case 1:
                mostrarCatalogo();

                printf("\nDeseja adicionar um produto ao carrinho?\n");
                printf("1 - Sim\n");
                printf("0 - Nao\n");
                printf("Escolha: ");

                if (scanf("%d", &opcao) != 1) {
                    printf("Opcao invalida!\n");

                    while (getchar() != '\n') {
                        /* Limpa a entrada */
                    }

                    break;
                }

                if (opcao == 1) {
                    adicionarAoCarrinho(carrinho, &totalItens);
                }
                break;

            case 2:
                mostrarCarrinho(carrinho, totalItens);
                break;

            case 3:
                printf("\n========== FINALIZAR COMPRA ==========\n");

                if (totalItens == 0) {
                    printf("Seu carrinho esta vazio!\n");
                    break;
                }

                mostrarCarrinho(carrinho, totalItens);

                if (perfil == 0) {
                    printf("\nPara continuar, faca login na sua conta.\n");

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

                if (perfil != 3) {
                    printf("\nA finalizacao da compra exige uma");
                    printf(" conta de cliente.\n");

                    if (perfil == 1 || perfil == 2) {
                        printf("Entre com uma conta de cliente para comprar.\n");
                    }

                    break;
                }

                printf("\nCliente: %s\n", usuario);
                printf("Total da compra: R$ %.2f\n",
                       calcularTotal(carrinho, totalItens));

                printf("\nCompra simulada finalizada!\n");
                printf("Obrigado por comprar na van Gogh!\n");

                /*
                   Neste momento, a compra e simulada.
                   O estoque ainda nao e descontado e o pedido
                   ainda nao e salvo em um arquivo.
                */

                totalItens = 0;

                continuar = 0;
                break;

            case 4:
                mostrarCatalogo();
                break;

            case 5:
                if (perfil == 1) {
                    cadastrarProduto();
                } else {
                    printf("\nSomente o gerente pode cadastrar produtos.\n");
                }
                break;

            case 6:
                if (perfil == 1 || perfil == 2) {
                    consultarProdutos();
                } else {
                    printf("\nAcesso negado.\n");
                }
                break;

            case 7:
                if (perfil == 1) {
                    cadastrarBalconista();
                } else {
                    printf("\nSomente o gerente pode cadastrar balconistas.\n");
                }
                break;

            case 8:
                if (perfil == 0) {
                    resultadoLogin = cadastroELogin(
                        usuario,
                        sizeof(usuario),
                        &perfil
                    );

                    if (resultadoLogin) {
                        printf("\nLogin realizado com sucesso!\n");
                        printf("Bem-vindo(a), %s!\n", usuario);
                    } else {
                        strcpy(usuario, "Visitante");
                        perfil = 0;

                        printf("\nVoce continua como visitante.\n");
                    }
                } else if (perfil == 3) {
                    strcpy(usuario, "Visitante");
                    perfil = 0;

                    printf("\nVoce saiu da sua conta.\n");
                    printf("Seu carrinho continua disponivel nesta sessao.\n");
                } else {
                    printf("\nPara trocar de conta administrativa,");
                    printf(" encerre o programa e entre novamente.\n");
                }
                break;

            case 9:
                if (perfil == 0) {
                    cadastrarCliente();
                    printf("\nDepois, escolha Fazer login para entrar.\n");
                } else {
                    printf("\nOpcao disponivel somente para visitantes.\n");
                }
                break;

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
        printf("      CARRINHO NAO FINALIZADO\n");
        printf("====================================\n");

        mostrarCarrinho(carrinho, totalItens);
        printf("\nOs itens nao foram comprados.\n");
    }

    printf("\nObrigado por visitar a Floricultura van Gogh!\n");
    printf("Volte sempre!\n");

    return 0;
}

