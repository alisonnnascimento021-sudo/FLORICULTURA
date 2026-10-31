
#include <stdio.h>
#include <string.h>
#include "Login.h"

/* =====================================
   CRIAR O PRIMEIRO GERENTE
   ===================================== */
int criarPrimeiroGerente(void) {
    FILE *arquivo;
    char usuario[50];
    char senha[50];
    char confirmarSenha[50];

    printf("\n=== CONFIGURACAO INICIAL ===\n");
    printf("Nenhum usuario encontrado.\n");
    printf("Crie o primeiro gerente da Floricultura.\n\n");

    printf("Usuario do gerente: ");
    scanf("%49s", usuario);

    printf("Senha do gerente: ");
    scanf("%49s", senha);

    printf("Confirme a senha: ");
    scanf("%49s", confirmarSenha);

    if (strcmp(senha, confirmarSenha) != 0) {
        printf("\nAs senhas nao conferem!\n");
        return 0;
    }

    arquivo = fopen("usuarios.txt", "w");

    if (arquivo == NULL) {
        printf("\nErro ao criar usuarios.txt!\n");
        return 0;
    }

    /* Perfil 1 = gerente */
    fprintf(arquivo, "%s;%s;1\n", usuario, senha);

    fclose(arquivo);

    printf("\nPrimeiro gerente cadastrado com sucesso!\n");

    return 1;
}

/* =====================================
   LOGIN E CONTROLE DE PERFIL
   ===================================== */
int cadastroELogin(char usuarioLogado[], int tamanho,
                   int *perfilLogado) {

    FILE *arquivo;
    char usuarioDigitado[50];
    char senhaDigitada[50];
    char usuarioArquivo[50];
    char senhaArquivo[50];
    int perfilArquivo;
    int tentativas = 3;
    int encontrouUsuario;

    /* Verifica se o arquivo de usuarios existe */
    arquivo = fopen("usuarios.txt", "r");

    if (arquivo == NULL) {
        if (!criarPrimeiroGerente()) {
            return 0;
        }
    } else {
        fclose(arquivo);
    }

    printf("\n=== LOGIN FLORICULTURA ===\n");

    while (tentativas > 0) {

        printf("Usuario: ");
        scanf("%49s", usuarioDigitado);

        printf("Senha: ");
        scanf("%49s", senhaDigitada);

        arquivo = fopen("usuarios.txt", "r");

        if (arquivo == NULL) {
            printf("\nErro ao abrir usuarios.txt!\n");
            return 0;
        }

        encontrouUsuario = 0;

        while (fscanf(arquivo, "%49[^;];%49[^;];%d\n",
                      usuarioArquivo,
                      senhaArquivo,
                      &perfilArquivo) == 3) {

            if (strcmp(usuarioDigitado, usuarioArquivo) == 0 &&
                strcmp(senhaDigitada, senhaArquivo) == 0) {

                encontrouUsuario = 1;
                break;
            }
        }

        fclose(arquivo);

        if (encontrouUsuario) {

            strncpy(usuarioLogado, usuarioArquivo, tamanho - 1);
            usuarioLogado[tamanho - 1] = '\0';

            *perfilLogado = perfilArquivo;

            printf("\nLogin realizado com sucesso!\n");

            if (*perfilLogado == 1) {
                printf("Perfil: GERENTE\n");
            } else if (*perfilLogado == 2) {
                printf("Perfil: BALCONISTA\n");
            } else {
                printf("Perfil desconhecido. Acesso negado.\n");
                return 0;
            }

            return 1;
        }

        tentativas--;

        printf("\nUsuario ou senha incorretos!\n");
        printf("Tentativas restantes: %d\n", tentativas);
    }

    printf("\nAcesso bloqueado nesta execucao.\n");

    return 0;
}

/* =====================================
   CADASTRAR BALCONISTA
   Somente o gerente pode chamar esta funcao
   ===================================== */
void cadastrarBalconista(void) {

    FILE *arquivo;
    char novoUsuario[50];
    char novaSenha[50];
    char confirmarSenha[50];

    char usuarioArquivo[50];
    char senhaArquivo[50];
    int perfilArquivo;

    int encontrou = 0;

    printf("\n=== CADASTRO DE BALCONISTA ===\n");

    printf("Novo usuario: ");
    scanf("%49s", novoUsuario);

    /* Verifica se o usuario ja existe */
    arquivo = fopen("usuarios.txt", "r");

    if (arquivo == NULL) {
        printf("\nErro ao abrir usuarios.txt!\n");
        return;
    }

    while (fscanf(arquivo, "%49[^;];%49[^;];%d\n",
                  usuarioArquivo,
                  senhaArquivo,
                  &perfilArquivo) == 3) {

        if (strcmp(novoUsuario, usuarioArquivo) == 0) {
            encontrou = 1;
            break;
        }
    }

    fclose(arquivo);

    if (encontrou) {
        printf("\nEsse usuario ja esta cadastrado!\n");
        return;
    }

    printf("Crie uma senha: ");
    scanf("%49s", novaSenha);

    printf("Confirme a senha: ");
    scanf("%49s", confirmarSenha);

    if (strcmp(novaSenha, confirmarSenha) != 0) {
        printf("\nAs senhas nao conferem!\n");
        return;
    }

    /* "a" acrescenta um usuario sem apagar os anteriores */
    arquivo = fopen("usuarios.txt", "a");

    if (arquivo == NULL) {
        printf("\nErro ao salvar o balconista!\n");
        return;
    }

    /* Perfil 2 = balconista */
    fprintf(arquivo, "%s;%s;2\n",
            novoUsuario, novaSenha);

    fclose(arquivo);

    printf("\nBalconista cadastrado com sucesso!\n");
    printf("Usuario: %s\n", novoUsuario);
    printf("Perfil: BALCONISTA\n");
}
/* =====================================
   CADASTRAR CLIENTE
   ===================================== */
void cadastrarCliente(void) {
    FILE *arquivoUsuarios;
    FILE *arquivoClientes;

    char nomeCompleto[100];
    char cpf[15];
    char telefone[20];
    char email[100];
    char endereco[150];

    char novoUsuario[50];
    char novaSenha[50];
    char confirmarSenha[50];

    char usuarioArquivo[50];
    char senhaArquivo[50];
    int perfilArquivo;
    int encontrou = 0;

    printf("\n====== CADASTRO DE CLIENTE ======\n");

    printf("Nome completo: ");
    scanf(" %99[^\n]", nomeCompleto);

    printf("CPF (somente numeros): ");
    scanf("%14s", cpf);

    printf("Telefone: ");
    scanf(" %19[^\n]", telefone);

    printf("E-mail: ");
    scanf("%99s", email);

    printf("Endereco completo: ");
    scanf(" %149[^\n]", endereco);

    printf("\nCrie seu nome de usuario (sem espacos): ");
    scanf("%49s", novoUsuario);

    /* Verifica se o nome de usuario ja existe */
    arquivoUsuarios = fopen("usuarios.txt", "r");

    if (arquivoUsuarios == NULL) {
        printf("\nErro ao abrir usuarios.txt!\n");
        printf("Cadastre primeiro o gerente do sistema.\n");
        return;
    }

    while (fscanf(arquivoUsuarios, "%49[^;];%49[^;];%d\n",
                  usuarioArquivo, senhaArquivo,
                  &perfilArquivo) == 3) {

        if (strcmp(novoUsuario, usuarioArquivo) == 0) {
            encontrou = 1;
            break;
        }
    }

    fclose(arquivoUsuarios);

    if (encontrou) {
        printf("\nEsse nome de usuario ja existe!\n");
        printf("Escolha outro nome de usuario.\n");
        return;
    }

    printf("Crie uma senha: ");
    scanf("%49s", novaSenha);

    printf("Confirme a senha: ");
    scanf("%49s", confirmarSenha);

    if (strcmp(novaSenha, confirmarSenha) != 0) {
        printf("\nAs senhas nao conferem!\n");
        return;
    }

    /* Salva os dados pessoais do cliente */
    arquivoClientes = fopen("clientes.txt", "a");

    if (arquivoClientes == NULL) {
        printf("\nErro ao abrir clientes.txt!\n");
        return;
    }

    fprintf(arquivoClientes, "%s;%s;%s;%s;%s;%s\n",
            novoUsuario,
            nomeCompleto,
            cpf,
            telefone,
            email,
            endereco);

    fclose(arquivoClientes);

    /* Salva o login: perfil 3 = cliente */
    arquivoUsuarios = fopen("usuarios.txt", "a");

    if (arquivoUsuarios == NULL) {
        printf("\nErro ao salvar o login do cliente!\n");
        printf("Verifique os arquivos antes de tentar novamente.\n");
        return;
    }

    fprintf(arquivoUsuarios, "%s;%s;3\n",
            novoUsuario, novaSenha);

    fclose(arquivoUsuarios);

    printf("\nCadastro realizado com sucesso!\n");
    printf("Bem-vindo(a), %s!\n", nomeCompleto);
    printf("Agora voce pode fazer login como cliente.\n");
}
