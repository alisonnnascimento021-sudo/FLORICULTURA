
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <windows.h>
#include <wininet.h>
#include <math.h>
#include <ctype.h>

#include "Login.h"
#include "produtos.h"

/* =====================================
   PALETA VAN GOGH
   ===================================== */

#define COR_VERDE       (FOREGROUND_GREEN | FOREGROUND_INTENSITY)
#define COR_DOURADO     (FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY)
#define COR_BRANCO      (FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY)
#define COR_VERMELHO    (FOREGROUND_RED | FOREGROUND_INTENSITY)
#define COR_AZUL_NOITE  (FOREGROUND_BLUE)
#define COR_AZUL        (FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY)

void consultarPedidos(void);
void atualizarStatusPedido(void);
void registrarObservacaoPedido(void);
void consultarObservacoesPedido(void);
void emitirComprovantePedido(void);

void consultarCEP(void);

void registrarAvaliacao(
    char cliente[],
    char modalidade[]
);

void registrarVenda(
    char cliente[],
    float total,
    char pagamento[]
);

void consultarAvaliacoes(void);
int registrarPedido(
    char cliente[],
    struct ItemCarrinho carrinho[],
    int totalItens,
    float total,
    char pagamento[],
    char modalidade[],
    char cepEntrega[],
    float taxaEntrega,
    double distanciaEntrega
);


void corTexto(WORD cor) {
    HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(console, cor);
}

void consultarPedidos(void) {

    FILE *arquivo;
    char linha[1000];
    char *campos[11];
    char *token;

    int i;
    int quantidadePedidos = 0;

    arquivo = fopen("pedidos.txt", "r");

    cabecalho();
    tituloPagina("PEDIDOS REGISTRADOS");

    if (arquivo == NULL) {

        corTexto(COR_DOURADO);
        printf("\nNenhum pedido registrado ainda.\n");
        corTexto(COR_BRANCO);

        system("pause");
        return;
    }

    while (fgets(linha, sizeof(linha), arquivo) != NULL) {

        i = 0;

        token = strtok(linha, ";");

        while (token != NULL && i < 11) {

            campos[i] = token;
            i++;

            token = strtok(NULL, ";");
        }

        if (i < 11) {
            continue;
        }

        quantidadePedidos++;

        printf("\n========================================\n");

        corTexto(COR_DOURADO);
        printf("PEDIDO NUMERO: %s\n", campos[0]);

        corTexto(COR_BRANCO);
        printf("Cliente: %s\n", campos[1]);
        printf("Data: %s\n", campos[2]);
        printf("Total: R$ %s\n", campos[3]);
        printf("Pagamento: %s\n", campos[4]);
        printf("Modalidade: %s\n", campos[5]);
        printf("CEP: %s\n", campos[6]);
        printf("Taxa de entrega: R$ %s\n", campos[7]);
        printf("Distancia: %s km\n", campos[8]);

        corTexto(COR_VERDE);
        printf("Situacao: %s\n", campos[9]);

        corTexto(COR_BRANCO);
        printf("Produtos e quantidades:\n");

        token = strtok(campos[10], "|");

        while (token != NULL) {

            printf(" - %s\n", token);

            token = strtok(NULL, "|");
        }
    }

    fclose(arquivo);

    if (quantidadePedidos == 0) {

        corTexto(COR_DOURADO);
        printf("\nNenhum pedido registrado.\n");
        corTexto(COR_BRANCO);
    }

    printf("\n========================================\n");

    system("pause");
}


void atualizarStatusPedido(void) {

    FILE *arquivo;
    FILE *temporario;

    char linha[1000];
    char copia[1000];
    char *campos[11];
    char *inicio;
    char *separador;

    char numeroPedido[30];
    char entrada[20];
    char novoStatus[40];

    int i;
    int opcao;
    int encontrado = 0;
    int atualizado = 0;

    cabecalho();
    tituloPagina("ATUALIZAR STATUS DO PEDIDO");

    printf("\nDigite o numero do pedido: ");

    fgets(numeroPedido, sizeof(numeroPedido), stdin);
    numeroPedido[strcspn(numeroPedido, "\n")] = '\0';

    if (strlen(numeroPedido) == 0) {
        printf("\nOperacao cancelada.\n");
        system("pause");
        return;
    }

    arquivo = fopen("pedidos.txt", "r");

    if (arquivo == NULL) {
        printf("\nNenhum pedido registrado.\n");
        system("pause");
        return;
    }

    temporario = fopen("pedidos_temp.txt", "w");

    if (temporario == NULL) {
        fclose(arquivo);
        printf("\nErro ao abrir arquivo temporario.\n");
        system("pause");
        return;
    }

    while (fgets(linha, sizeof(linha), arquivo) != NULL) {

        strcpy(copia, linha);

        inicio = copia;
        i = 0;

        while (i < 10) {

            separador = strchr(inicio, ';');

            if (separador == NULL) {
                break;
            }

            *separador = '\0';
            campos[i] = inicio;

            inicio = separador + 1;
            i++;
        }

        if (i == 10) {

            campos[10] = inicio;
            campos[10][strcspn(campos[10], "\r\n")] = '\0';

            if (strcmp(campos[0], numeroPedido) == 0) {

                encontrado = 1;

                printf("\nPedido encontrado!\n");
                printf("Cliente: %s\n", campos[1]);
                printf("Status atual: %s\n", campos[9]);

                printf("\nNOVO STATUS:\n");
                printf("1 - Pendente\n");
                printf("2 - Em preparacao\n");
                printf("3 - Pronto para retirada\n");
                printf("4 - Saiu para entrega\n");
                printf("5 - Entregue\n");
                printf("6 - Cancelado\n");
                printf("0 - Cancelar operacao\n");

                printf("\nEscolha: ");

                if (fgets(entrada, sizeof(entrada), stdin) == NULL) {
                    opcao = 0;
                } else {
                    opcao = atoi(entrada);
                }

                switch (opcao) {

                    case 1:
                        strcpy(novoStatus, "Pendente");
                        break;

                    case 2:
                        strcpy(novoStatus, "Em preparacao");
                        break;

                    case 3:
                        strcpy(novoStatus, "Pronto para retirada");
                        break;

                    case 4:
                        strcpy(novoStatus, "Saiu para entrega");
                        break;

                    case 5:
                        strcpy(novoStatus, "Entregue");
                        break;

                    case 6:
                        strcpy(novoStatus, "Cancelado");
                        break;

                    default:
                        strcpy(novoStatus, campos[9]);
                        break;
                }

                if (opcao >= 1 && opcao <= 6) {

                    strcpy(campos[9], novoStatus);
                    atualizado = 1;
                }
            }

            for (i = 0; i < 9; i++) {
                fprintf(temporario, "%s;", campos[i]);
            }

            fprintf(temporario, "%s;%s\n", campos[9], campos[10]);

        } else {

            /* Preserva linhas que nao seguem o formato esperado. */
            fprintf(temporario, "%s", linha);
        }
    }

    fclose(arquivo);
    fclose(temporario);

    if (!encontrado) {

        remove("pedidos_temp.txt");

        corTexto(COR_VERMELHO);
        printf("\nPedido nao encontrado!\n");
        corTexto(COR_BRANCO);

    } else if (atualizado) {

        if (MoveFileExA(
            "pedidos_temp.txt",
            "pedidos.txt",
            MOVEFILE_REPLACE_EXISTING
        )) {

            corTexto(COR_VERDE);
            printf("\nStatus do pedido atualizado com sucesso!\n");
            corTexto(COR_BRANCO);

        } else {

            corTexto(COR_VERMELHO);
            printf("\nErro ao salvar a atualizacao!\n");
            corTexto(COR_BRANCO);

            remove("pedidos_temp.txt");
        }

    } else {

        remove("pedidos_temp.txt");

        printf("\nOperacao cancelada. O status foi mantido.\n");
    }

    system("pause");
}

void registrarObservacaoPedido(void) {

    FILE *arquivo;

    char numeroPedido[30];
    char observacao[300];

    cabecalho();
    tituloPagina("OBSERVACOES DO PEDIDO");

    printf("\nDigite o numero do pedido: ");

    fgets(numeroPedido, sizeof(numeroPedido), stdin);
    numeroPedido[strcspn(numeroPedido, "\n")] = '\0';

    if (strlen(numeroPedido) == 0) {

        printf("\nNumero do pedido nao informado.\n");
        system("pause");
        return;
    }

    printf("\nDigite a observacao:\n");

    fgets(observacao, sizeof(observacao), stdin);
    observacao[strcspn(observacao, "\n")] = '\0';

    if (strlen(observacao) == 0) {

        printf("\nA observacao nao pode ficar vazia.\n");
        system("pause");
        return;
    }

    arquivo = fopen("observacoes_pedidos.txt", "a");

    if (arquivo == NULL) {

        corTexto(COR_VERMELHO);
        printf("\nErro ao salvar a observacao!\n");
        corTexto(COR_BRANCO);

        system("pause");
        return;
    }

    fprintf(
        arquivo,
        "%s;%s\n",
        numeroPedido,
        observacao
    );

    fclose(arquivo);

    corTexto(COR_VERDE);
    printf("\nObservacao registrada com sucesso!\n");
    corTexto(COR_BRANCO);

    system("pause");
}


void consultarObservacoesPedido(void) {

    FILE *arquivo;

    char linha[400];
    char numeroPedido[30];
    char numeroSalvo[30];
    char *separador;

    int encontrou = 0;

    cabecalho();
    tituloPagina("CONSULTAR OBSERVACOES");

    printf("\nDigite o numero do pedido: ");

    fgets(numeroPedido, sizeof(numeroPedido), stdin);

    numeroPedido[strcspn(numeroPedido, "\n")] = '\0';

    if (strlen(numeroPedido) == 0) {

        printf("\nNumero do pedido nao informado.\n");
        system("pause");
        return;
    }

    arquivo = fopen("observacoes_pedidos.txt", "r");

    if (arquivo == NULL) {

        printf("\nNenhuma observacao registrada.\n");
        system("pause");
        return;
    }

    corTexto(COR_DOURADO);
    printf("\nOBSERVACOES DO PEDIDO %s\n", numeroPedido);
    corTexto(COR_BRANCO);

    while (fgets(linha, sizeof(linha), arquivo) != NULL) {

        linha[strcspn(linha, "\r\n")] = '\0';

        separador = strchr(linha, ';');

        if (separador == NULL) {
            continue;
        }

        *separador = '\0';

        strcpy(numeroSalvo, linha);

        if (strcmp(numeroSalvo, numeroPedido) == 0) {

            encontrou = 1;

            printf("\n- %s\n", separador + 1);
        }
    }

    fclose(arquivo);

    if (!encontrou) {

        corTexto(COR_DOURADO);
        printf("\nNenhuma observacao encontrada para este pedido.\n");
        corTexto(COR_BRANCO);
    }

    printf("\n========================================\n");

    system("pause");
}


void emitirComprovantePedido(void) {

    FILE *arquivo;
    FILE *comprovante;

    char linha[1000];
    char *campos[11];
    char nomeArquivo[80];
    char numeroPedido[30];
    char *token;

    int i;
    int encontrado = 0;

    cabecalho();
    tituloPagina("EMITIR COMPROVANTE");

    printf("\nDigite o numero do pedido: ");

    fgets(numeroPedido, sizeof(numeroPedido), stdin);
    numeroPedido[strcspn(numeroPedido, "\n")] = '\0';

    if (strlen(numeroPedido) == 0 ||
        strspn(numeroPedido, "0123456789") != strlen(numeroPedido)) {

        printf("\nNumero do pedido invalido.\n");
        system("pause");
        return;
    }

    arquivo = fopen("pedidos.txt", "r");

    if (arquivo == NULL) {

        printf("\nNenhum pedido registrado.\n");
        system("pause");
        return;
    }

    while (fgets(linha, sizeof(linha), arquivo) != NULL) {

        i = 0;

        token = strtok(linha, ";");

        while (token != NULL && i < 11) {

            campos[i] = token;
            i++;

            token = strtok(NULL, ";");
        }

        if (i < 11) {
            continue;
        }

        campos[10][strcspn(campos[10], "\r\n")] = '\0';

        if (strcmp(campos[0], numeroPedido) == 0) {

            encontrado = 1;

            sprintf(
                nomeArquivo,
                "comprovante_%s.txt",
                numeroPedido
            );

            comprovante = fopen(nomeArquivo, "w");

            if (comprovante == NULL) {

                fclose(arquivo);

                printf("\nErro ao criar comprovante.\n");
                system("pause");
                return;
            }

            fprintf(comprovante, "========================================\n");
            fprintf(comprovante, "       FLORICULTURA VAN GOGH\n");
            fprintf(comprovante, "          COMPROVANTE DE PEDIDO\n");
            fprintf(comprovante, "========================================\n\n");

            fprintf(comprovante, "Numero do pedido: %s\n", campos[0]);
            fprintf(comprovante, "Cliente: %s\n", campos[1]);
            fprintf(comprovante, "Data: %s\n", campos[2]);

            fprintf(comprovante, "\nPRODUTOS:\n");

            token = strtok(campos[10], "|");

            while (token != NULL) {

                fprintf(comprovante, "- %s\n", token);

                token = strtok(NULL, "|");
            }

            fprintf(comprovante, "\n----------------------------------------\n");

            fprintf(comprovante, "Valor total: R$ %s\n", campos[3]);
            fprintf(comprovante, "Pagamento: %s\n", campos[4]);
            fprintf(comprovante, "Recebimento: %s\n", campos[5]);

            if (strcmp(campos[5], "Entrega") == 0 ||
                strcmp(campos[5], "entrega") == 0) {

                fprintf(comprovante, "CEP: %s\n", campos[6]);
                fprintf(comprovante, "Taxa de entrega: R$ %s\n", campos[7]);
                fprintf(comprovante, "Distancia: %s km\n", campos[8]);
            }

            fprintf(comprovante, "Status: %s\n", campos[9]);

            fprintf(comprovante, "\n========================================\n");
            fprintf(comprovante, "       Obrigado pela preferencia!\n");
            fprintf(comprovante, "          FLORICULTURA VAN GOGH\n");
            fprintf(comprovante, "========================================\n");

            fclose(comprovante);

            break;
        }
    }

    fclose(arquivo);

    if (encontrado) {

        corTexto(COR_VERDE);
        printf("\nComprovante emitido com sucesso!\n");
        corTexto(COR_BRANCO);

        printf("\nArquivo gerado: %s\n", nomeArquivo);

    } else {

        corTexto(COR_VERMELHO);
        printf("\nPedido nao encontrado!\n");
        corTexto(COR_BRANCO);
    }

    system("pause");
}




/* =====================================
   LINHA DECORATIVA
   ===================================== */

void linhaDecorativa(void) {

    corTexto(COR_DOURADO);

    printf("====================================================\n");

    corTexto(COR_BRANCO);
}

/* =====================================
   CABECALHO PRINCIPAL
   ===================================== */

void cabecalho(void) {

    linhaDecorativa();

    corTexto(COR_VERDE);
    printf("                 VAN GOGH\n");

    corTexto(COR_DOURADO);
    printf("                 FLORICULTURA\n");

    corTexto(COR_BRANCO);
    printf("           Flores que encantam momentos\n");

    linhaDecorativa();
}

/* =====================================
   TITULO DAS PAGINAS
   ===================================== */

void tituloPagina(char titulo[]) {

    printf("\n");

    linhaDecorativa();

    corTexto(COR_VERDE);
    printf("              %s\n", titulo);

    corTexto(COR_BRANCO);

    linhaDecorativa();
}

/* =====================================
   NOVA PAGINA INICIAL
   ===================================== */

void paginaInicial(char usuario[], int perfil) {

    system("cls");

    /* TOPO DA LOJA */

    corTexto(COR_AZUL_NOITE);

    printf("\n");
    printf("  +==================================================+\n");

    corTexto(COR_DOURADO);

    printf("  |                                                  |\n");
    printf("  |                  VAN GOGH                        |\n");
    printf("  |                 FLORICULTURA                     |\n");
    printf("  |                                                  |\n");

    corTexto(COR_BRANCO);

    printf("  |          FLORES QUE ENCANTAM MOMENTOS            |\n");

    corTexto(COR_AZUL_NOITE);

    printf("  +==================================================+\n");

    /* BOAS-VINDAS */

    printf("\n");

    corTexto(COR_DOURADO);
    printf("  BEM-VINDO A NOSSA LOJA!\n");

    corTexto(COR_BRANCO);

    if (perfil == 0) {
        printf("  Ola! Explore nossa colecao de flores.\n");
    } else {
        printf("  Ola, %s!\n", usuario);
    }

    printf("  Perfil: ");

    if (perfil == 1) {

        corTexto(COR_VERMELHO);
        printf("GERENTE");

    } else if (perfil == 2) {

        corTexto(COR_VERDE);
        printf("BALCONISTA");

    } else if (perfil == 3) {

        corTexto(COR_AZUL);
        printf("CLIENTE");

    } else {

        corTexto(COR_DOURADO);
        printf("VISITANTE");
    }

    corTexto(COR_BRANCO);

    printf("\n\n");

    /* MENU DE NAVEGACAO */

    corTexto(COR_DOURADO);

    printf("  +--------------------------------------------------+\n");
    printf("  |              COMO DESEJA CONTINUAR?              |\n");
    printf("  +--------------------------------------------------+\n");

    corTexto(COR_BRANCO);

    printf("\n  [01]  Explorar catalogo de flores\n");
    printf("        Conheca nossa colecao especial.\n");

    printf("\n  [02]  Meu carrinho\n");
    printf("        Confira os produtos selecionados.\n");

    if (perfil == 0 || perfil == 3) {

        printf("\n  [03]  Finalizar minha compra\n");
        printf("        Escolha a forma de pagamento.\n");
    }

    if (perfil == 0) {

        printf("\n  [04]  Entrar na minha conta\n");
        printf("        Acesse sua area pessoal.\n");

        printf("\n  [05]  Criar minha conta\n");
        printf("        Cadastre-se na VAN GOGH.\n");
    }

    if (perfil == 1) {

        printf("\n  [04]  Painel administrativo\n");
        printf("        Gerencie sua floricultura.\n");
    }

    if (perfil == 2) {

        printf("\n  [04]  Area do balconista\n");
        printf("        Acesse suas ferramentas de trabalho.\n");
    }

    if (perfil == 3) {

        printf("\n  [04]  Minha conta\n");
        printf("        Consulte seus dados cadastrais.\n");
    }

    corTexto(COR_DOURADO);

    printf("\n  +--------------------------------------------------+\n");

    corTexto(COR_BRANCO);

    printf("  [00]  Atualizar pagina inicial\n");

    if (perfil != 0) {
        printf("  [09]  Sair da minha conta\n");
    }

    corTexto(COR_VERMELHO);
    printf("  [08]  Encerrar programa\n");

    corTexto(COR_DOURADO);
    printf("\n  +--------------------------------------------------+\n");

    corTexto(COR_BRANCO);

    printf("\n  Selecione uma opcao: ");
}

/* =====================================
   LIMPAR ENTRADA DO TECLADO
   ===================================== */

void limparEntrada(void) {

    int c;

    while ((c = getchar()) != '\n' && c != EOF) {
    }
}

/* =====================================
   LER OPCAO DO MENU
   ===================================== */

int lerOpcao(void) {

    int opcao;

    if (scanf("%d", &opcao) != 1) {

        limparEntrada();

        return -1;
    }

    limparEntrada();

    return opcao;
}

/* =====================================
   PROCESSAR NAVEGACAO
   ===================================== */

int processarNavegacao(
    int resultado,
    char usuario[],
    int *perfil,
    int *totalItens
) {

    if (resultado == NAV_LOGOUT) {

        corTexto(COR_DOURADO);
        printf("\nSaindo da conta...\n");

        strcpy(usuario, "Visitante");
        consultarCEP();
		system("pause");

        *perfil = 0;
        *totalItens = 0;

        printf("Voce voltou ao menu de visitante.\n");

        corTexto(COR_BRANCO);

    } else if (resultado == NAV_SAIR) {

        corTexto(COR_DOURADO);
        printf("\nEncerrando a Floricultura van Gogh...\n");

        corTexto(COR_BRANCO);

        return 0;
    }

    return 1;
}

/* =====================================
   REGISTRAR VENDA
   ===================================== */

/* =====================================
   REGISTRAR VENDA COMPLETA
   ===================================== */

void registrarVendaCompleta(
    char cliente[],
    float total,
    char pagamento[],
    char modalidade[],
    char cepEntrega[],
    float taxaEntrega,
    double distanciaEntrega
) {
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

    fprintf(
        arquivo,
        "%s;%.2f;%s;%s;%s;%s;%.2f;%.2f\n",
        cliente,
        total,
        pagamento,
        data,
        modalidade,
        cepEntrega,
        taxaEntrega,
        distanciaEntrega
    );

    fclose(arquivo);

    corTexto(COR_VERDE);
    printf("\nVenda registrada no sistema!\n");
    corTexto(COR_BRANCO);
}

void registrarVenda(
    char cliente[],
    float total,
    char pagamento[]
) {
    registrarVendaCompleta(
        cliente,
        total,
        pagamento,
        "Venda no balcao",
        "Nao se aplica",
        0.0f,
        0.0
    );
}

/* =====================================
   RELATORIO GERENCIAL
   ===================================== */


void relatorioGerencial(void) {

    FILE *arquivo;

    char linha[300];

    char cliente[100];
    char pagamento[40];
    char data[30];
    char modalidade[40];
    char cepEntrega[20];

    float valor;
    float taxaEntrega;
    double distanciaEntrega;

    float faturamento = 0.0f;
    float totalTaxas = 0.0f;

    int totalVendas = 0;
    int pix = 0;
    int credito = 0;
    int debito = 0;
    int dinheiro = 0;

    int totalEntregas = 0;
    int totalRetiradas = 0;

    int resultado;

    cabecalho();

    tituloPagina("RELATORIO GERENCIAL");

    arquivo = fopen("vendas.txt", "r");

    if (arquivo == NULL) {

        corTexto(COR_DOURADO);
        printf("\nNenhuma venda registrada ainda.\n");
        corTexto(COR_BRANCO);

        system("pause");

        return;
    }

    /* =====================================
       LEITURA DO HISTORICO DE VENDAS
       ===================================== */

    while (fgets(linha, sizeof(linha), arquivo) != NULL) {

        resultado = sscanf(
            linha,
            "%99[^;];%f;%39[^;];%29[^;];%39[^;];%19[^;];%f;%lf",
            cliente,
            &valor,
            pagamento,
            data,
            modalidade,
            cepEntrega,
            &taxaEntrega,
            &distanciaEntrega
        );

        if (resultado == 4 || resultado == 8) {

            totalVendas++;

            faturamento += valor;

            data[strcspn(data, "\r\n")] = '\0';

            /* FORMAS DE PAGAMENTO */

            if (strcmp(pagamento, "Pix") == 0) {

                pix++;

            } else if (strcmp(pagamento, "Cartao de credito") == 0) {

                credito++;

            } else if (strcmp(pagamento, "Cartao de debito") == 0) {

                debito++;

            } else if (strcmp(pagamento, "Dinheiro") == 0) {

                dinheiro++;
            }

            /* RECEBIMENTO */

            if (resultado == 8) {

                if (strcmp(modalidade, "Entrega em casa") == 0) {

                    totalEntregas++;
                    totalTaxas += taxaEntrega;

                } else if (strcmp(modalidade, "Retirada na loja") == 0) {

                    totalRetiradas++;
                }
            }
        }
    }

    fclose(arquivo);

    /* =====================================
       RESUMO FINANCEIRO
       ===================================== */

    cabecalho();

    tituloPagina("RELATORIO GERENCIAL");

    corTexto(COR_DOURADO);
    printf("\n========== RESUMO FINANCEIRO ==========\n");
    corTexto(COR_BRANCO);

    printf("\nQuantidade de vendas: %d\n", totalVendas);
    printf("Faturamento total: R$ %.2f\n", faturamento);

    /* =====================================
       FORMAS DE PAGAMENTO
       ===================================== */

    corTexto(COR_DOURADO);
    printf("\n========== FORMAS DE PAGAMENTO ==========\n");
    corTexto(COR_BRANCO);

    printf("\nPIX: %d\n", pix);
    printf("Cartao de credito: %d\n", credito);
    printf("Cartao de debito: %d\n", debito);
    printf("Dinheiro: %d\n", dinheiro);

    /* =====================================
       ENTREGAS E RETIRADAS
       ===================================== */

    corTexto(COR_DOURADO);
    printf("\n========== RECEBIMENTO ==========\n");
    corTexto(COR_BRANCO);

    printf("\nEntregas em casa: %d\n", totalEntregas);
    printf("Retiradas na loja: %d\n", totalRetiradas);
    printf("Total arrecadado em taxas: R$ %.2f\n", totalTaxas);

    /* =====================================
       HISTORICO DE VENDAS
       ===================================== */

    arquivo = fopen("vendas.txt", "r");

    if (arquivo != NULL) {

        corTexto(COR_DOURADO);
        printf("\n========== HISTORICO DE VENDAS ==========\n");
        corTexto(COR_BRANCO);

        while (fgets(linha, sizeof(linha), arquivo) != NULL) {

            resultado = sscanf(
                linha,
                "%99[^;];%f;%39[^;];%29[^;];%39[^;];%19[^;];%f;%lf",
                cliente,
                &valor,
                pagamento,
                data,
                modalidade,
                cepEntrega,
                &taxaEntrega,
                &distanciaEntrega
            );

            if (resultado == 4 || resultado == 8) {

                data[strcspn(data, "\r\n")] = '\0';

                printf("\nCliente: %s\n", cliente);
                printf("Valor: R$ %.2f\n", valor);
                printf("Pagamento: %s\n", pagamento);
                printf("Data: %s\n", data);

                if (resultado == 8) {

                    printf("Recebimento: %s\n", modalidade);

                    if (strcmp(modalidade, "Entrega em casa") == 0) {

                        printf("CEP: %s\n", cepEntrega);
                        printf("Distancia: %.2f km\n", distanciaEntrega);
                        printf("Taxa: R$ %.2f\n", taxaEntrega);

                    } else {

                        printf("Retirada na loja: gratuita\n");
                    }

                } else {

                    printf("Registro antigo ou venda no balcao\n");
                }

                printf("----------------------------------------\n");
            }
        }

        fclose(arquivo);
    }

    linhaDecorativa();

    system("pause");
}


/* =====================================
   REGISTRAR PEDIDO
   ===================================== */

int registrarPedido(
    char cliente[],
    struct ItemCarrinho carrinho[],
    int totalItens,
    float total,
    char pagamento[],
    char modalidade[],
    char cepEntrega[],
    float taxaEntrega,
    double distanciaEntrega
) {

    FILE *arquivo;
    int i;
    int numeroPedido = 1;
    int numeroAtual;
    char linha[1000];

    time_t agora;
    struct tm *dataAtual;
    char data[30];

    agora = time(NULL);
    dataAtual = localtime(&agora);

    if (dataAtual == NULL) {
        return 0;
    }

    strftime(data, sizeof(data), "%d/%m/%Y %H:%M", dataAtual);

    arquivo = fopen("pedidos.txt", "a+");

    if (arquivo == NULL) {
        return 0;
    }

    rewind(arquivo);

    while (fgets(linha, sizeof(linha), arquivo) != NULL) {

        if (sscanf(linha, "%d;", &numeroAtual) == 1) {

            if (numeroAtual >= numeroPedido) {
                numeroPedido = numeroAtual + 1;
            }
        }
    }

    fprintf(
        arquivo,
        "%d;%s;%s;%.2f;%s;%s;%s;%.2f;%.2f;Pendente;",
        numeroPedido,
        cliente,
        data,
        total,
        pagamento,
        modalidade,
        cepEntrega,
        taxaEntrega,
        distanciaEntrega
    );

    for (i = 0; i < totalItens; i++) {

        fprintf(
            arquivo,
            "%s,%d",
            carrinho[i].produto.nome,
            carrinho[i].quantidade
        );

        if (i < totalItens - 1) {
            fprintf(arquivo, "|");
        }
    }

    fprintf(arquivo, "\n");

    if (ferror(arquivo)) {
        fclose(arquivo);
        return 0;
    }

    fclose(arquivo);

    corTexto(COR_VERDE);
    printf("\nPedido registrado! Numero: %d\n", numeroPedido);
    corTexto(COR_BRANCO);

    return 1;
}


/* =====================================
   ESCOLHER FORMA DE PAGAMENTO
   ===================================== */

int escolherPagamento(char pagamento[]) {

    int opcao;

    while (1) {

        cabecalho();

        tituloPagina("FORMA DE PAGAMENTO");

        printf("1 - Pix\n");
        printf("2 - Cartao de credito\n");
        printf("3 - Cartao de debito\n");
        printf("4 - Dinheiro\n");

        printf("\n0 - Cancelar e voltar\n");
        printf("9 - Sair da conta\n");
        printf("8 - Fechar programa\n");

        printf("\nEscolha uma opcao: ");

        opcao = lerOpcao();

        if (opcao == 0) {
            return NAV_VOLTAR;
        }

        if (opcao == 9) {
            return NAV_LOGOUT;
        }

        if (opcao == 8) {
            return NAV_SAIR;
        }

        switch (opcao) {

            case 1:

                strcpy(pagamento, "Pix");

                return NAV_OK;

            case 2:

                strcpy(pagamento, "Cartao de credito");

                return NAV_OK;

            case 3:

                strcpy(pagamento, "Cartao de debito");

                return NAV_OK;

            case 4:

                strcpy(pagamento, "Dinheiro");

                return NAV_OK;

            default:

                corTexto(COR_VERMELHO);
                printf("\nForma de pagamento invalida!\n");

                corTexto(COR_BRANCO);
        }
    }
}

/* =====================================
   CAIXA PROPRIO DO BALCONISTA
   ===================================== */

int caixaBalconista(void) {

    struct ItemCarrinho carrinho[MAX_CARRINHO];

    char cliente[100];
    char pagamento[40];
    char texto[100];
    char extra;

    int totalItens = 0;
    int opcao;
    int resultado;
    int i;

    float total;
    float recebido;
    float troco;

    cabecalho();

    tituloPagina("CAIXA VAN GOGH");

    printf("Nome do cliente (0 cancelar): ");

    if (fgets(cliente, sizeof(cliente), stdin) == NULL) {
        return NAV_VOLTAR;
    }

    cliente[strcspn(cliente, "\n")] = '\0';

    if (strcmp(cliente, "0") == 0) {
        return NAV_VOLTAR;
    }

    if (strcmp(cliente, "9") == 0) {
        return NAV_LOGOUT;
    }

    if (strcmp(cliente, "8") == 0) {
        return NAV_SAIR;
    }

    if (strlen(cliente) == 0) {

        corTexto(COR_VERMELHO);
        printf("\nNome do cliente obrigatorio!\n");

        corTexto(COR_BRANCO);

        return NAV_VOLTAR;
    }

    /* ADICIONAR PRODUTOS */

    while (1) {

        cabecalho();

        tituloPagina("PRODUTOS DA VENDA");

        mostrarCatalogo();

        printf("\n1 - Adicionar produto\n");
        printf("0 - Finalizar selecao\n");
        printf("9 - Sair da conta\n");
        printf("8 - Encerrar programa\n");

        printf("\nEscolha: ");

        opcao = lerOpcao();

        if (opcao == 0) {
            break;
        }

        if (opcao == 9) {
            return NAV_LOGOUT;
        }

        if (opcao == 8) {
            return NAV_SAIR;
        }

        if (opcao == 1) {

            resultado = adicionarAoCarrinho(
                carrinho,
                &totalItens
            );

            if (resultado == NAV_LOGOUT ||
                resultado == NAV_SAIR) {

                return resultado;
            }

        } else {

            corTexto(COR_VERMELHO);
            printf("\nOpcao invalida!\n");

            corTexto(COR_BRANCO);
        }
    }

    if (totalItens == 0) {

        corTexto(COR_VERMELHO);
        printf("\nNenhum produto foi adicionado.\n");

        corTexto(COR_BRANCO);

        return NAV_VOLTAR;
    }

    /* MOSTRAR RESUMO */

    cabecalho();

    tituloPagina("RESUMO DA COMPRA");

    printf("Cliente: %s\n", cliente);

    mostrarCarrinho(carrinho, totalItens);
    total = calcularTotal(carrinho, totalItens);

    /* PAGAMENTO */

    resultado = escolherPagamento(pagamento);

    if (resultado != NAV_OK) {
        return resultado;
    }

    /* CALCULAR TROCO */

    if (strcmp(pagamento, "Dinheiro") == 0) {

        while (1) {

            printf("\nTotal: R$ %.2f\n", total);
            printf("Valor recebido: R$ ");

            if (fgets(texto, sizeof(texto), stdin) == NULL) {
                return NAV_VOLTAR;
            }

            texto[strcspn(texto, "\n")] = '\0';

            for (i = 0; texto[i] != '\0'; i++) {

                if (texto[i] == ',') {
                    texto[i] = '.';
                }
            }

            if (sscanf(texto, "%f %c", &recebido, &extra) != 1) {

                corTexto(COR_VERMELHO);
                printf("\nValor invalido!\n");

                corTexto(COR_BRANCO);

                continue;
            }

            if (recebido < total) {

                corTexto(COR_VERMELHO);
                printf("\nValor insuficiente para pagamento!\n");

                corTexto(COR_BRANCO);

                continue;
            }

            break;
        }

        troco = recebido - total;

        printf("\nValor recebido: R$ %.2f\n", recebido);
        printf("Troco: R$ %.2f\n", troco);
    }

    /* CONFIRMAR VENDA */

    cabecalho();

    tituloPagina("CONFIRMAR VENDA");

    printf("Cliente: %s\n", cliente);
    printf("Total: R$ %.2f\n", total);
    printf("Pagamento: %s\n", pagamento);

    printf("\n1 - Confirmar venda\n");
    printf("0 - Cancelar venda\n");
    printf("9 - Sair da conta\n");
    printf("8 - Encerrar programa\n");

    printf("\nEscolha: ");

    opcao = lerOpcao();

    if (opcao == 0) {

        corTexto(COR_VERMELHO);
        printf("\nVenda cancelada.\n");

        corTexto(COR_BRANCO);

        return NAV_VOLTAR;
    }

    if (opcao == 9) {
        return NAV_LOGOUT;
    }

    if (opcao == 8) {
        return NAV_SAIR;
    }

    if (opcao != 1) {

        corTexto(COR_VERMELHO);
        printf("\nOpcao invalida. Venda cancelada.\n");

        corTexto(COR_BRANCO);

        return NAV_VOLTAR;
    }

    /* ATUALIZAR ESTOQUE */

    resultado = atualizarEstoque(carrinho, totalItens);

    if (resultado != NAV_OK) {

        corTexto(COR_VERMELHO);
        printf("\nNao foi possivel concluir a venda.\n");

        corTexto(COR_BRANCO);

        return NAV_VOLTAR;
    }

    /* REGISTRAR VENDA */

    registrarVenda(cliente, total, pagamento);

    /* VENDA FINALIZADA */

    corTexto(COR_VERDE);

    printf("\n====================================\n");
    printf("          VENDA FINALIZADA!\n");
    printf("====================================\n");

    corTexto(COR_BRANCO);

    return NAV_OK;
}

/* =====================================
   MENU DA AREA DO BALCONISTA
   ===================================== */

int menuBalconista(char usuario[]) {

    int opcao;
    int resultado;

    while (1) {

        cabecalho();

        tituloPagina("AREA DO BALCONISTA");

        /* PRODUTOS E ESTOQUE */

        corTexto(COR_DOURADO);
        printf("\n========== PRODUTOS E ESTOQUE ==========\n");
        corTexto(COR_BRANCO);

        printf("1 - Consultar catalogo de flores\n");
        printf("2 - Consultar produtos e estoque\n");
        printf("3 - Consultar pedidos\n");
        printf("4 - Atualizar status dos pedidos\n");
        printf("5 - Registrar observacao em pedido\n");
        printf("6 - Consultar observacoes do pedido\n");
        printf("7 - Emitir comprovante de pedido\n");

        /* ATENDIMENTO E VENDAS */

        corTexto(COR_DOURADO);
        printf("\n========== ATENDIMENTO E VENDAS ==========\n");
        corTexto(COR_BRANCO);

        printf("10 - Abrir caixa / Realizar venda\n");

        /* MINHA CONTA */

        corTexto(COR_DOURADO);
        printf("\n=============== MINHA CONTA ===============\n");
        corTexto(COR_BRANCO);

        printf("11 - Meus dados cadastrais\n");

        /* NAVEGACAO */

        corTexto(COR_DOURADO);
        printf("\n=============== NAVEGACAO ===============\n");
        corTexto(COR_BRANCO);

        printf("0 - Voltar ao menu principal\n");
        printf("9 - Sair da conta\n");
        printf("8 - Encerrar programa\n");

        printf("\nEscolha uma opcao: ");

        opcao = lerOpcao();

        if (opcao == 0) {
            return NAV_VOLTAR;
        }

        if (opcao == 9) {
            return NAV_LOGOUT;
        }

        if (opcao == 8) {
            return NAV_SAIR;
        }

        switch (opcao) {

            case 1:

                resultado = mostrarCatalogo();

                if (resultado != NAV_OK) {
                    return resultado;
                }

                break;

            case 2:

                resultado = consultarProdutos();

                if (resultado != NAV_OK) {
                    return resultado;
                }

                break;
                
                case 3:

    		consultarPedidos();

    		break;
    		
    		case 4:

    		atualizarStatusPedido();

    		break;
    		
    	case 5:

    	registrarObservacaoPedido();

    	break;
    	
    	case 6:

    	consultarObservacoesPedido();

    	break;
    	
    	case 7:

    	emitirComprovantePedido();

    	break;

            case 10:

                resultado = caixaBalconista();

                if (resultado != NAV_OK) {
                    return resultado;
                }

                break;

            case 11:

                areaBalconista(usuario);

                break;

            default:

                corTexto(COR_VERMELHO);
                printf("\nOpcao invalida!\n");
                corTexto(COR_BRANCO);

                system("pause");

                break;
        }
    }
}

/* =====================================
   MENU ADMINISTRATIVO DO GERENTE
   ===================================== */

int menuGerente(void) {

    int opcao;
    int resultado;

    while (1) {

        cabecalho();

        tituloPagina("AREA DO GERENTE");

        /* PRODUTOS E ESTOQUE */

        corTexto(COR_DOURADO);
        printf("\n========== PRODUTOS E ESTOQUE ==========\n");
        corTexto(COR_BRANCO);

        printf("1 - Cadastrar produto\n");
        printf("2 - Consultar produtos e estoque\n");
        printf("5 - Alterar flores e precos\n");
        printf("6 - Remover flor do catalogo\n");

        /* ADMINISTRACAO */

        corTexto(COR_DOURADO);
        printf("\n============= ADMINISTRACAO =============\n");
        corTexto(COR_BRANCO);

        printf("3 - Cadastrar balconista\n");
        printf("7 - Remover conta de balconista ou cliente\n");

        /* VENDAS E AVALIACOES */

        corTexto(COR_DOURADO);
        printf("\n========== VENDAS E AVALIACOES ==========\n");
        corTexto(COR_BRANCO);

        printf("4 - Relatorios gerenciais\n");
        printf("8 - Consultar avaliacoes dos clientes\n");

        /* NAVEGACAO */

        corTexto(COR_DOURADO);
        printf("\n=============== NAVEGACAO ===============\n");
        corTexto(COR_BRANCO);

        printf("0  - Voltar ao menu principal\n");
        printf("9  - Sair da conta\n");
        printf("10 - Fechar programa\n");

        printf("\nEscolha uma opcao: ");

        opcao = lerOpcao();

        if (opcao == 0) {
            return NAV_VOLTAR;
        }

        if (opcao == 9) {
            return NAV_LOGOUT;
        }

        if (opcao == 10) {
            return NAV_SAIR;
        }

        switch (opcao) {

            case 1:

                resultado = cadastrarProduto();

                if (resultado != NAV_OK) {
                    return resultado;
                }

                break;

            case 2:

                resultado = consultarProdutos();

                if (resultado != NAV_OK) {
                    return resultado;
                }

                break;

            case 3:

                cadastrarBalconista();

                break;

            case 4:

                relatorioGerencial();

                break;

            case 5:

                resultado = alterarProduto();

                if (resultado != NAV_OK) {
                    return resultado;
                }

                break;

            case 6:

                resultado = removerProduto();

                if (resultado != NAV_OK) {
                    return resultado;
                }

                break;

            case 7:

                removerConta();

                break;

            case 8:

                consultarAvaliacoes();

                break;

            default:

                corTexto(COR_VERMELHO);
                printf("\nOpcao invalida!\n");
                corTexto(COR_BRANCO);

                system("pause");

                break;
        }
    }
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
    int resultado;
    char modalidade[30];
	char cepEntrega[20];

	float subtotal;
	float taxaEntrega;

	double distanciaEntrega;

    float total;

    struct ItemCarrinho carrinho[MAX_CARRINHO];

    int totalItens = 0;

    strcpy(usuario, "Visitante");

    while (continuar) {

        /* MOSTRAR NOVA PAGINA INICIAL */

        paginaInicial(usuario, perfil);

        opcao = lerOpcao();

        if (opcao == -1) {

            corTexto(COR_VERMELHO);
            printf("\nEntrada invalida! Digite um numero.\n");

            corTexto(COR_BRANCO);

            system("pause");

            continue;
        }

        /* ENCERRAR PROGRAMA */

        if (opcao == 8) {

            continuar = 0;

            continue;
        }

        /* SAIR DA CONTA */

        if (opcao == 9) {

            if (perfil != 0) {

                corTexto(COR_DOURADO);
                printf("\nAte logo, %s!\n", usuario);

                strcpy(usuario, "Visitante");

                perfil = 0;
                totalItens = 0;

                printf("Voce saiu da conta com sucesso.\n");

                corTexto(COR_BRANCO);

            } else {

                corTexto(COR_DOURADO);
                printf("\nVoce ja esta como visitante.\n");

                corTexto(COR_BRANCO);
            }

            system("pause");

            continue;
        }

        /* ATUALIZAR PAGINA INICIAL */

        if (opcao == 0) {
            continue;
        }

        switch (opcao) {

            /* CATALOGO E CARRINHO */

            case 1:

                mostrarCatalogo();

                printf("\nDeseja adicionar um produto ao carrinho?\n");
                printf("1 - Sim\n");
                printf("0 - Voltar ao menu principal\n");
                printf("9 - Sair da conta\n");
                printf("8 - Encerrar programa\n");

                printf("Escolha: ");

                opcao = lerOpcao();

                if (opcao == 1) {

                    resultado = adicionarAoCarrinho(
                        carrinho,
                        &totalItens
                    );

                    continuar = processarNavegacao(
                        resultado,
                        usuario,
                        &perfil,
                        &totalItens
                    );

                } else if (opcao == 9) {

                    if (perfil != 0) {

                        strcpy(usuario, "Visitante");

                        perfil = 0;
                        totalItens = 0;

                        corTexto(COR_DOURADO);
                        printf("\nVoce saiu da conta.\n");

                        corTexto(COR_BRANCO);
                    }

                } else if (opcao == 8) {

                    continuar = 0;

                } else if (opcao != 0) {

                    corTexto(COR_VERMELHO);
                    printf("\nOpcao invalida!\n");

                    corTexto(COR_BRANCO);
                }

                break;

            /* CARRINHO */

            case 2:

    resultado = gerenciarCarrinho(
        carrinho,
        &totalItens
    );

    continuar = processarNavegacao(
        resultado,
        usuario,
        &perfil,
        &totalItens
    );

    break;

            /* FINALIZAR COMPRA */

            case 3:

                if (perfil != 0 && perfil != 3) {

                    corTexto(COR_VERMELHO);
                    printf("\nEste perfil nao pode realizar compras.\n");

                    corTexto(COR_BRANCO);

                    break;
                }

                cabecalho();

                tituloPagina("FINALIZAR COMPRA");

                if (totalItens == 0) {

                    corTexto(COR_DOURADO);
                    printf("Seu carrinho esta vazio!\n");

                    corTexto(COR_BRANCO);

                    break;
                }

                mostrarCarrinho(carrinho, totalItens);

                /* EXIGIR LOGIN DO CLIENTE */

                if (perfil == 0) {

                    printf("\nPara finalizar a compra, faca login.\n");

                    resultadoLogin = cadastroELogin(
                        usuario,
                        sizeof(usuario),
                        &perfil
                    );

                    if (resultadoLogin != 1) {

                        continuar = processarNavegacao(
                            resultadoLogin,
                            usuario,
                            &perfil,
                            &totalItens
                        );

                        corTexto(COR_VERMELHO);
                        printf("\nLogin nao realizado.\n");

                        corTexto(COR_BRANCO);

                        printf("Sua compra continua no carrinho, se voce apenas voltou.\n");

                        break;
                    }
                }

                /* SOMENTE CLIENTE PODE COMPRAR */

                if (perfil != 3) {

                    corTexto(COR_VERMELHO);
                    printf("\nSomente uma conta de cliente");
                    printf(" pode finalizar a compra.\n");

                    corTexto(COR_BRANCO);

                    break;
                }

               subtotal = calcularTotal(carrinho, totalItens);
               
			resultado = escolherRecebimento(
    		modalidade,
    		cepEntrega,
    		&taxaEntrega,
    		&distanciaEntrega
);

if (resultado != NAV_OK) {

    continuar = processarNavegacao(
        resultado,
        usuario,
        &perfil,
        &totalItens
    );

    break;
}

total = subtotal + taxaEntrega;

                printf("\nCliente: %s\n", usuario);
                printf("Total: R$ %.2f\n", total);

                resultado = escolherPagamento(pagamento);

                if (resultado != NAV_OK) {

                    continuar = processarNavegacao(
                        resultado,
                        usuario,
                        &perfil,
                        &totalItens
                    );

                    if (resultado == NAV_VOLTAR) {

                        corTexto(COR_VERMELHO);
                        printf("\nCompra cancelada.\n");

                        corTexto(COR_BRANCO);

                        printf("Seus produtos continuam no carrinho.\n");
                    }

                    break;
                }

                cabecalho();

                tituloPagina("FORMA DE PAGAMENTO");

				printf("Cliente: %s\n", usuario);
				printf("Subtotal: R$ %.2f\n", subtotal);
				printf("Recebimento: %s\n", modalidade);

				if (strcmp(modalidade, "Entrega em casa") == 0) {

    			printf("CEP de entrega: %s\n", cepEntrega);
    			printf("Distancia: %.2f km\n", distanciaEntrega);
}

				printf("Taxa de entrega: R$ %.2f\n", taxaEntrega);
				printf("TOTAL: R$ %.2f\n", total);
				printf("Pagamento: %s\n", pagamento);
                printf("\nConfirmar compra?\n");
                printf("1 - Confirmar\n");
                printf("0 - Cancelar\n");
                printf("9 - Sair da conta\n");
                printf("8 - Encerrar programa\n");

                printf("Escolha: ");

                opcao = lerOpcao();

                if (opcao == 1) {

                    resultado = atualizarEstoque(
                        carrinho,
                        totalItens
                    );

                    if (resultado != NAV_OK) {

                        corTexto(COR_VERMELHO);

                        printf("\nNao foi possivel concluir a venda.\n");
                        printf("Compra mantida no carrinho.\n");

                        corTexto(COR_BRANCO);

                        break;
                    }

                    registrarVendaCompleta(
    			usuario,
    			total,
    			pagamento,
    			modalidade,
    			cepEntrega,
    			taxaEntrega,
    			distanciaEntrega
				);
				if (!registrarPedido(
    usuario,
    carrinho,
    totalItens,
    total,
    pagamento,
    modalidade,
    cepEntrega,
    taxaEntrega,
    distanciaEntrega
)) {

    corTexto(COR_VERMELHO);
    printf("\nErro ao registrar o pedido!\n");
    corTexto(COR_BRANCO);
}

                    corTexto(COR_VERDE);

                    printf("\nCompra finalizada com sucesso!\n");
					printf("Obrigado por comprar na van Gogh!\n");

			corTexto(COR_BRANCO);

			/* CONVIDAR CLIENTE A AVALIAR */

				registrarAvaliacao(usuario, modalidade);

				totalItens = 0;
                    corTexto(COR_BRANCO);



                } else if (opcao == 9) {

                    strcpy(usuario, "Visitante");

                    perfil = 0;
                    totalItens = 0;

                    corTexto(COR_DOURADO);
                    printf("\nVoce saiu da conta.\n");

                    corTexto(COR_BRANCO);

                } else if (opcao == 8) {

                    continuar = 0;

                } else if (opcao == 0) {

                    corTexto(COR_VERMELHO);
                    printf("\nCompra cancelada.\n");

                    corTexto(COR_BRANCO);

                    printf("Seus produtos continuam no carrinho.\n");

                } else {

                    corTexto(COR_VERMELHO);
                    printf("\nOpcao invalida!\n");

                    corTexto(COR_BRANCO);
                }

                break;

            /* LOGIN, AREA ADMINISTRATIVA OU CONSULTA */

            case 4:

                if (perfil == 0) {

                    resultadoLogin = cadastroELogin(
                        usuario,
                        sizeof(usuario),
                        &perfil
                    );

                    if (resultadoLogin == 1) {

                        corTexto(COR_VERDE);
                        printf("\nBem-vindo(a), %s!\n", usuario);

                        corTexto(COR_BRANCO);

                        system("pause");

                    } else {

                        continuar = processarNavegacao(
                            resultadoLogin,
                            usuario,
                            &perfil,
                            &totalItens
                        );
                    }

                } else if (perfil == 1) {

                    resultado = menuGerente();

                    continuar = processarNavegacao(
                        resultado,
                        usuario,
                        &perfil,
                        &totalItens
                    );

                } else if (perfil == 2) {

                    resultado = menuBalconista(usuario);

                    continuar = processarNavegacao(
                        resultado,
                        usuario,
                        &perfil,
                        &totalItens
                    );

                } else if (perfil == 3) {

                    areaCliente(usuario);

                } else {

                    corTexto(COR_VERMELHO);
                    printf("\nOpcao indisponivel para este perfil.\n");

                    corTexto(COR_BRANCO);
                }

                break;

            /* CADASTRO DE CLIENTE */

            case 5:

                if (perfil == 0) {

                    cadastrarCliente();

                    corTexto(COR_VERDE);
                    printf("\nAgora voce pode fazer login.\n");

                    corTexto(COR_BRANCO);

                    system("pause");

                } else {

                    corTexto(COR_VERMELHO);
                    printf("\nOpcao indisponivel para este perfil.\n");

                    corTexto(COR_BRANCO);
                }

                break;

            default:

                corTexto(COR_VERMELHO);
                printf("\nOpcao invalida! Tente novamente.\n");

                corTexto(COR_BRANCO);

                system("pause");

                break;
        }
    }

    /* MOSTRAR CARRINHO NAO FINALIZADO */

    if (totalItens > 0) {

        cabecalho();

        tituloPagina("CARRINHO NAO FINALIZADO!");

        mostrarCarrinho(carrinho, totalItens);

        corTexto(COR_DOURADO);
        printf("\nEsses produtos nao foram comprados.\n");

        corTexto(COR_BRANCO);
    }

    corTexto(COR_DOURADO);

    printf("\nObrigado por visitar a Floricultura van Gogh!\n");
    printf("Volte sempre!\n");

    corTexto(COR_BRANCO);

    return 0;
}

void testarWinINet(void) {
    HINTERNET conexao;

    conexao = InternetOpen(
        "Floricultura van Gogh",
        INTERNET_OPEN_TYPE_PRECONFIG,
        NULL,
        NULL,
        0
    );

    if (conexao != NULL) {
        printf("\nBiblioteca WinINet funcionando!\n");
        InternetCloseHandle(conexao);
    } else {
        printf("\nErro ao iniciar a WinINet.\n");
    }
}


/* =====================================
   EXTRAIR CAMPO DA RESPOSTA JSON
   ===================================== */

int extrairCampoJSON(
    const char json[],
    const char campo[],
    char valor[],
    int tamanho
) {
    char chave[60];
    const char *inicio;
    const char *fim;
    int comprimento;

    sprintf(chave, "\"%s\":\"", campo);

    inicio = strstr(json, chave);

    if (inicio == NULL) {
        valor[0] = '\0';
        return 0;
    }

    inicio += strlen(chave);

    fim = strchr(inicio, '"');

    if (fim == NULL) {
        valor[0] = '\0';
        return 0;
    }

    comprimento = (int)(fim - inicio);

    if (comprimento >= tamanho) {
        comprimento = tamanho - 1;
    }

    strncpy(valor, inicio, comprimento);
    valor[comprimento] = '\0';

    return 1;
}

/* =====================================
   CONSULTAR CEP PELO VIACEP
   ===================================== */
void consultarCEP(void) {

    HINTERNET conexao;
    HINTERNET requisicao;

    char entrada[30];
    char cep[9];
    char url[100];

    char resposta[4096];
    DWORD bytesLidos;
    int quantidade = 0;
    int i;

    char logradouro[150];
    char bairro[100];
    char cidade[100];
    char uf[10];

    cabecalho();
    tituloPagina("CONSULTA DE ENDERECO");

    printf("Digite o CEP: ");

    if (fgets(entrada, sizeof(entrada), stdin) == NULL) {
        return;
    }

    entrada[strcspn(entrada, "\n")] = '\0';

    /* ACEITAR CEP COM OU SEM HIFEN */

    for (i = 0; entrada[i] != '\0'; i++) {

        if (isdigit((unsigned char)entrada[i])) {

            if (quantidade >= 8) {
                break;
            }

            cep[quantidade] = entrada[i];
            quantidade++;

        } else if (entrada[i] != '-') {

            quantidade = -1;
            break;
        }
    }

    if (quantidade != 8) {

        corTexto(COR_VERMELHO);
        printf("\nCEP invalido! Digite os 8 numeros.\n");
        corTexto(COR_BRANCO);

        return;
    }

    cep[8] = '\0';

    sprintf(url, "https://viacep.com.br/ws/%s/json/", cep);

    conexao = InternetOpenA(
        "Floricultura van Gogh",
        INTERNET_OPEN_TYPE_PRECONFIG,
        NULL,
        NULL,
        0
    );

    if (conexao == NULL) {

        printf("\nNao foi possivel iniciar a conexao.\n");
        return;
    }

    requisicao = InternetOpenUrlA(
        conexao,
        url,
        NULL,
        0,
        INTERNET_FLAG_RELOAD | INTERNET_FLAG_NO_CACHE_WRITE,
        0
    );

    if (requisicao == NULL) {

        printf("\nNao foi possivel consultar o CEP.\n");

        InternetCloseHandle(conexao);
        return;
    }

    /* RECEBER RESPOSTA */

    quantidade = 0;

    while (quantidade < (int)sizeof(resposta) - 1) {

        if (!InternetReadFile(
                requisicao,
                resposta + quantidade,
                sizeof(resposta) - 1 - quantidade,
                &bytesLidos
            )) {
            break;
        }

        if (bytesLidos == 0) {
            break;
        }

        quantidade += bytesLidos;
    }

    resposta[quantidade] = '\0';

    InternetCloseHandle(requisicao);
    InternetCloseHandle(conexao);

    if (strstr(resposta, "\"erro\": true") != NULL) {

        corTexto(COR_VERMELHO);
        printf("\nCEP nao encontrado!\n");
        corTexto(COR_BRANCO);

        return;
    }

    /* EXTRAIR DADOS */

    extrairCampoJSON(resposta, "logradouro",
                     logradouro, sizeof(logradouro));

    extrairCampoJSON(resposta, "bairro",
                     bairro, sizeof(bairro));

    extrairCampoJSON(resposta, "localidade",
                     cidade, sizeof(cidade));

    extrairCampoJSON(resposta, "uf",
                     uf, sizeof(uf));

    /* MOSTRAR ENDERECO */

    cabecalho();
    tituloPagina("ENDERECO ENCONTRADO");

    corTexto(COR_VERDE);
    printf("\nCEP valido!\n\n");

    corTexto(COR_BRANCO);

    printf("Logradouro: %s\n", logradouro);
    printf("Bairro: %s\n", bairro);
    printf("Cidade: %s\n", cidade);
    printf("Estado: %s\n", uf);

    linhaDecorativa();
}

/* =====================================
   CONSULTAR COORDENADAS PELO CEP
   OPENSTREETMAP / NOMINATIM
   ===================================== */

int obterCoordenadasCEP(
    char cepEntrada[],
    double *latitude,
    double *longitude
) {
    HINTERNET conexao;
    HINTERNET requisicao;

    char cep[9];
    char url[300];
    char resposta[8192];

    const char *campo;
    DWORD bytesLidos;

    int i;
    int quantidade = 0;

    char agente[] =
        "FloriculturaVanGogh/1.0 (contato: SEU_EMAIL)";

    /* LIMPAR CEP */

    for (i = 0; cepEntrada[i] != '\0'; i++) {

        if (isdigit((unsigned char)cepEntrada[i])) {

            if (quantidade >= 8) {
                return 0;
            }

            cep[quantidade] = cepEntrada[i];
            quantidade++;

        } else if (cepEntrada[i] != '-') {

            return 0;
        }
    }

    if (quantidade != 8) {
        return 0;
    }

    cep[8] = '\0';

    /* MONTAR CONSULTA */

    sprintf(
        url,
        "https://nominatim.openstreetmap.org/search?postalcode=%s&country=Brazil&format=jsonv2&limit=1",
        cep
    );

    conexao = InternetOpenA(
        agente,
        INTERNET_OPEN_TYPE_PRECONFIG,
        NULL,
        NULL,
        0
    );

    if (conexao == NULL) {
        return 0;
    }

    requisicao = InternetOpenUrlA(
        conexao,
        url,
        NULL,
        0,
        INTERNET_FLAG_RELOAD |
        INTERNET_FLAG_NO_CACHE_WRITE,
        0
    );

    if (requisicao == NULL) {

        InternetCloseHandle(conexao);

        return 0;
    }

    /* LER RESPOSTA */

    while (quantidade = 0, 0) {
        /* Nao utilizado */
    }

    quantidade = 0;

    while (quantidade < sizeof(resposta) - 1) {

        if (!InternetReadFile(
            requisicao,
            resposta + quantidade,
            sizeof(resposta) - 1 - quantidade,
            &bytesLidos
        )) {
            break;
        }

        if (bytesLidos == 0) {
            break;
        }

        quantidade += bytesLidos;
    }

    resposta[quantidade] = '\0';

    InternetCloseHandle(requisicao);
    InternetCloseHandle(conexao);

    if (quantidade == 0 || strstr(resposta, "[]") != NULL) {
        return 0;
    }

    /* EXTRAIR LATITUDE */

    campo = strstr(resposta, "\"lat\"");

    if (campo == NULL) {
        return 0;
    }

    campo = strchr(campo, ':');

    if (campo == NULL) {
        return 0;
    }

    campo++;

    while (isspace((unsigned char)*campo)) {
        campo++;
    }

    if (*campo == '"') {
        campo++;
    }

    *latitude = atof(campo);

    /* EXTRAIR LONGITUDE */

    campo = strstr(resposta, "\"lon\"");

    if (campo == NULL) {
        return 0;
    }

    campo = strchr(campo, ':');

    if (campo == NULL) {
        return 0;
    }

    campo++;

    while (isspace((unsigned char)*campo)) {
        campo++;
    }

    if (*campo == '"') {
        campo++;
    }

    *longitude = atof(campo);

    return 1;
}

/* =====================================
   CALCULAR DISTANCIA EM LINHA RETA
   FORMULA DE HAVERSINE
   ===================================== */

double calcularDistanciaKm(
    double lat1,
    double lon1,
    double lat2,
    double lon2
) {
    double pi = 3.14159265358979323846;
    double raioTerra = 6371.0;

    double diferencaLat;
    double diferencaLon;

    double a;
    double c;

    diferencaLat = (lat2 - lat1) * pi / 180.0;
    diferencaLon = (lon2 - lon1) * pi / 180.0;

    lat1 = lat1 * pi / 180.0;
    lat2 = lat2 * pi / 180.0;

    a = sin(diferencaLat / 2) *
        sin(diferencaLat / 2) +
        cos(lat1) * cos(lat2) *
        sin(diferencaLon / 2) *
        sin(diferencaLon / 2);

    c = 2 * atan2(sqrt(a), sqrt(1 - a));

    return raioTerra * c;
}

/* =====================================
   TESTAR DISTANCIA ENTRE CEPS
   ===================================== */

void testarDistanciaCEP(void) {

    char cepCliente[30];

    double latLoja;
    double lonLoja;

    double latCliente;
    double lonCliente;

    double distancia;

    cabecalho();
    tituloPagina("TESTE DE DISTANCIA");

    printf("Digite o CEP do cliente: ");

    if (fgets(cepCliente, sizeof(cepCliente), stdin) == NULL) {
        return;
    }

    cepCliente[strcspn(cepCliente, "\n")] = '\0';

    /* CONSULTAR CEP DA LOJA */

    printf("\nConsultando CEP da loja...\n");

    if (!obterCoordenadasCEP(
        "05427-020",
        &latLoja,
        &lonLoja
    )) {

        printf("\nNao foi possivel localizar o CEP da loja.\n");
        return;
    }

    /* RESPEITAR INTERVALO DE CONSULTA */

    Sleep(1100);

    /* CONSULTAR CEP DO CLIENTE */

    printf("Consultando CEP do cliente...\n");

    if (!obterCoordenadasCEP(
        cepCliente,
        &latCliente,
        &lonCliente
    )) {

        printf("\nNao foi possivel localizar o CEP do cliente.\n");
        return;
    }

    /* CALCULAR DISTANCIA */

    distancia = calcularDistanciaKm(
        latLoja,
        lonLoja,
        latCliente,
        lonCliente
    );

    corTexto(COR_VERDE);

    printf("\nDistancia calculada com sucesso!\n");

    corTexto(COR_BRANCO);

    printf("\nDistancia em linha reta: %.2f km\n", distancia);

    linhaDecorativa();
}

/* =====================================
   ESCOLHER FORMA DE RECEBIMENTO
   ===================================== */

int escolherRecebimento(
    char modalidade[],
    char cepEntrega[],
    float *taxaEntrega,
    double *distanciaEntrega
) {
    int opcao;

    double latLoja;
    double lonLoja;
    double latCliente;
    double lonCliente;

    *taxaEntrega = 0.0f;
    *distanciaEntrega = 0.0;

    while (1) {

        cabecalho();
        tituloPagina("FORMA DE RECEBIMENTO");

        printf("1 - Retirar na loja (gratuito)\n");
        printf("2 - Receber em casa (com taxa)\n");

        printf("\n0 - Voltar\n");
        printf("9 - Sair da conta\n");
        printf("8 - Encerrar programa\n");

        printf("\nEscolha: ");

        opcao = lerOpcao();

        if (opcao == 0) return NAV_VOLTAR;
        if (opcao == 9) return NAV_LOGOUT;
        if (opcao == 8) return NAV_SAIR;

        if (opcao == 1) {

            strcpy(modalidade, "Retirada na loja");
            strcpy(cepEntrega, "Nao se aplica");

            return NAV_OK;
        }

        if (opcao == 2) {

            strcpy(modalidade, "Entrega em casa");

            cabecalho();
            tituloPagina("ENDERECO DE ENTREGA");

            printf("Digite o CEP: ");

            if (fgets(cepEntrega, 20, stdin) == NULL) {
                return NAV_VOLTAR;
            }

            cepEntrega[strcspn(cepEntrega, "\n")] = '\0';

            printf("\nConsultando endereco...\n");

            if (!obterCoordenadasCEP(
                "05427-020",
                &latLoja,
                &lonLoja
            )) {

                printf("\nNao foi possivel localizar o CEP da loja.\n");
                return NAV_VOLTAR;
            }

            Sleep(1100);

            if (!obterCoordenadasCEP(
                cepEntrega,
                &latCliente,
                &lonCliente
            )) {

                corTexto(COR_VERMELHO);
                printf("\nCEP invalido ou nao localizado.\n");
                corTexto(COR_BRANCO);

                system("pause");
                continue;
            }

            *distanciaEntrega = calcularDistanciaKm(
                latLoja,
                lonLoja,
                latCliente,
                lonCliente
            );

            if (*distanciaEntrega <= 5.0) {
                *taxaEntrega = 8.00f;
            } else if (*distanciaEntrega <= 10.0) {
                *taxaEntrega = 12.00f;
            } else if (*distanciaEntrega <= 15.0) {
                *taxaEntrega = 16.00f;
            } else {
                *taxaEntrega = 20.00f;
            }

            cabecalho();
            tituloPagina("RESUMO DA ENTREGA");

            printf("CEP: %s\n", cepEntrega);
            printf("Distancia em linha reta: %.2f km\n",
                   *distanciaEntrega);
            printf("Taxa de entrega: R$ %.2f\n",
                   *taxaEntrega);

            system("pause");

            return NAV_OK;
        }

        corTexto(COR_VERMELHO);
        printf("\nOpcao invalida!\n");
        corTexto(COR_BRANCO);
    }
}

/* =====================================
   CONSULTAR AVALIACOES DOS CLIENTES
   ===================================== */

void consultarAvaliacoes(void) {

    FILE *arquivo;

    char linha[500];

    cabecalho();

    tituloPagina("AVALIACOES DOS CLIENTES");

    arquivo = fopen("avaliacoes.txt", "r");

    if (arquivo == NULL) {

        corTexto(COR_DOURADO);
        printf("\nNenhuma avaliacao registrada ainda.\n");
        corTexto(COR_BRANCO);

        system("pause");

        return;
    }

    while (fgets(linha, sizeof(linha), arquivo) != NULL) {

        printf("\n%s", linha);
        printf("----------------------------------------\n");
    }

    fclose(arquivo);

    printf("\n");

    system("pause");
}

/* =====================================
   REGISTRAR AVALIACAO DO CLIENTE
   ===================================== */

	void registrarAvaliacao(char cliente[], char modalidade[]) {

    FILE *arquivo;

    int qualidade;
    int atendimento;
    int entrega = 0;
    int opcao;

    char comentario[300];
    char data[30];

    time_t agora;
    struct tm *dataAtual;

    cabecalho();

    tituloPagina("AVALIE SUA COMPRA");

    printf("Sua opiniao e muito importante para nos!\n");

    printf("\nQualidade dos produtos (1 a 5): ");

    qualidade = lerOpcao();

    while (qualidade < 1 || qualidade > 5) {

        printf("Digite uma nota de 1 a 5: ");
        qualidade = lerOpcao();
    }

    printf("\nAtendimento da loja (1 a 5): ");

    atendimento = lerOpcao();

    while (atendimento < 1 || atendimento > 5) {

        printf("Digite uma nota de 1 a 5: ");
        atendimento = lerOpcao();
    }

    if (strcmp(modalidade, "Entrega em casa") == 0) {

        printf("\nAtendimento do entregador (1 a 5): ");

        entrega = lerOpcao();

        while (entrega < 1 || entrega > 5) {

            printf("Digite uma nota de 1 a 5: ");
            entrega = lerOpcao();
        }
    }

    printf("\nDeseja deixar um comentario?\n");
    printf("1 - Sim\n");
    printf("2 - Nao\n");
    printf("Escolha: ");

    opcao = lerOpcao();

    while (opcao != 1 && opcao != 2) {

        printf("Digite 1 ou 2: ");
        opcao = lerOpcao();
    }

    strcpy(comentario, "Sem comentario");

    if (opcao == 1) {

        printf("\nDigite seu comentario: ");

        fgets(comentario, sizeof(comentario), stdin);

        comentario[strcspn(comentario, "\n")] = '\0';
    }

    agora = time(NULL);
    dataAtual = localtime(&agora);

    strftime(data, sizeof(data), "%d/%m/%Y %H:%M", dataAtual);

    arquivo = fopen("avaliacoes.txt", "a");

    if (arquivo == NULL) {

        printf("\nNao foi possivel salvar sua avaliacao.\n");
        system("pause");
        return;
    }

    if (entrega > 0) {

        fprintf(arquivo,
            "Cliente: %s | Qualidade: %d | Atendimento: %d | Entrega: %d | Comentario: %s | Data: %s\n",
            cliente,
            qualidade,
            atendimento,
            entrega,
            comentario,
            data
        );

    } else {

        fprintf(arquivo,
            "Cliente: %s | Qualidade: %d | Atendimento: %d | Comentario: %s | Data: %s\n",
            cliente,
            qualidade,
            atendimento,
            comentario,
            data
        );
    }

    fclose(arquivo);

    corTexto(COR_VERDE);

    printf("\nObrigado por avaliar a Floricultura van Gogh!\n");

    corTexto(COR_BRANCO);

    system("pause");
}



