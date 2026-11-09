
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <windows.h>

#include "Login.h"

/* =====================================
   IDENTIDADE VISUAL VAN GOGH
   ===================================== */

#define LOGIN_VERDE \
    (FOREGROUND_GREEN | FOREGROUND_INTENSITY)

#define LOGIN_DOURADO \
    (FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY)

#define LOGIN_BRANCO \
    (FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY)

#define LOGIN_VERMELHO \
    (FOREGROUND_RED | FOREGROUND_INTENSITY)

/* =====================================
   FUNCOES AUXILIARES
   ===================================== */

void limparTeclado(void) {
    int c;

    while ((c = getchar()) != '\n' && c != EOF) {
    }
}

/* =====================================
   CONTROLE DE CORES DO LOGIN
   ===================================== */

void corLogin(WORD cor) {
    HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(console, cor);
}

void restaurarCorLogin(void) {
    corLogin(LOGIN_BRANCO);
}
/* =====================================
   CABECALHO VAN GOGH - LOGIN
   ===================================== */

void cabecalhoLogin(void) {

    printf("\n");

    corLogin(LOGIN_VERDE);
    printf("==================================================\n");

    corLogin(LOGIN_DOURADO);
    printf("                    VAN GOGH\n");

    corLogin(LOGIN_BRANCO);
    printf("                   FLORICULTURA\n");

    corLogin(LOGIN_VERDE);
    printf("==================================================\n");

    corLogin(LOGIN_DOURADO);
    printf("\n             FLORES QUE ENCANTAM MOMENTOS\n");

    restaurarCorLogin();

    printf("\n");
}

/* =====================================
   LER TEXTO COM POSSIBILIDADE DE CANCELAR
   ===================================== */

int lerTexto(char texto[], int tamanho, const char mensagem[]) {
    int c;
    
    printf("%s", mensagem);

    if (fgets(texto, tamanho, stdin) == NULL) {
        return 0;
    }

    if (strchr(texto, '\n') == NULL) {
        while ((c = getchar()) != '\n' && c != EOF) {
        }
    }

    texto[strcspn(texto, "\n")] = '\0';

    if (strcmp(texto, "0") == 0) {
        printf("\nOperacao cancelada.\n");
        return 0;
    }

    if (strlen(texto) == 0) {
        printf("\nO campo nao pode ficar vazio!\n");
        return 0;
    }

    return 1;
}

/* =====================================
   VERIFICAR SE UM TEXTO CONTEM OUTRO
   IGNORANDO MAIUSCULAS E MINUSCULAS
   ===================================== */

int contemTexto(char texto[], char palavra[]) {
    char textoCopia[200];
    char palavraCopia[100];
    int i;

    strncpy(textoCopia, texto, sizeof(textoCopia) - 1);
    textoCopia[sizeof(textoCopia) - 1] = '\0';

    strncpy(palavraCopia, palavra, sizeof(palavraCopia) - 1);
    palavraCopia[sizeof(palavraCopia) - 1] = '\0';

    for (i = 0; textoCopia[i] != '\0'; i++) {
        textoCopia[i] = (char)tolower((unsigned char)textoCopia[i]);
    }

    for (i = 0; palavraCopia[i] != '\0'; i++) {
        palavraCopia[i] = (char)tolower((unsigned char)palavraCopia[i]);
    }

    return strstr(textoCopia, palavraCopia) != NULL;
}

/* =====================================
   VALIDAR SENHA
   ===================================== */

int validarSenha(char senha[], char nome[]) {
    int i;
    int letra = 0;
    int numero = 0;
    int simbolo = 0;

    if (strlen(senha) < 6) {
        printf("\nATENCAO: A senha precisa ter no minimo 6 caracteres!\n");
        return 0;
    }

    for (i = 0; senha[i] != '\0'; i++) {
        if (isalpha((unsigned char)senha[i])) {
            letra = 1;
        } else if (isdigit((unsigned char)senha[i])) {
            numero = 1;
        } else if (ispunct((unsigned char)senha[i])) {
            simbolo = 1;
        }
    }

    if (!letra) {
        printf("\nATENCAO: Adicione pelo menos uma letra!\n");
        return 0;
    }

    if (!numero) {
        printf("\nATENCAO: Adicione pelo menos um numero!\n");
        return 0;
    }

    if (!simbolo) {
        printf("\nATENCAO: Adicione pelo menos um simbolo! Exemplo: @, #, ! ou $\n");
        return 0;
    }

    if (nome != NULL && strlen(nome) > 0 &&
        contemTexto(senha, nome)) {
        printf("\nATENCAO: A senha nao pode conter seu nome ou usuario!\n");
        return 0;
    }

    return 1;
}

/* =====================================
   VALIDAR CPF
   ===================================== */

int validarCPF(char cpf[]) {
    int i;

    if (strlen(cpf) != 11) {
        printf("\nO CPF precisa ter exatamente 11 numeros.\n");
        return 0;
    }

    for (i = 0; cpf[i] != '\0'; i++) {
        if (!isdigit((unsigned char)cpf[i])) {
            printf("\nO CPF deve conter somente numeros.\n");
            return 0;
        }
    }

    return 1;
}

/* ==========================================
	VALIDAR TELEFONE
	========================================== */

int validarTelefone(char telefone[]) {

    int i;
    int digitos = 0;
    int numeroDDD = 0;
    char apenasNumeros[12];

    for (i = 0; telefone[i] != '\0'; i++) {

        if (isdigit((unsigned char)telefone[i])) {

            if (digitos < 11) {
                apenasNumeros[digitos] = telefone[i];
            }

            digitos++;

        } else if (telefone[i] != '(' &&
                   telefone[i] != ')' &&
                   telefone[i] != '-' &&
                   telefone[i] != ' ') {

            printf("\nTelefone invalido! Utilize somente numeros.\n");
            return 0;
        }
    }

    if (digitos != 10 && digitos != 11) {
        printf("\nInforme o DDD e o telefone com 10 ou 11 digitos.\n");
        return 0;
    }

    apenasNumeros[digitos] = '\0';

    numeroDDD = (apenasNumeros[0] - '0') * 10 +
                (apenasNumeros[1] - '0');

    if (numeroDDD < 11 || numeroDDD > 99) {
        printf("\nDDD invalido!\n");
        return 0;
    }

    return 1;
}
/* =====================================
   VALIDAR EMAIL
   ===================================== */

int validarEmail(char email[]) {
    char *arroba;

    arroba = strchr(email, '@');

    if (arroba == NULL || arroba == email ||
        *(arroba + 1) == '\0') {
        printf("\nEmail invalido! Tente novamente.\n");
        return 0;
    }

    if (strchr(arroba + 1, '.') == NULL) {
        printf("\nEmail invalido! Informe um dominio valido.\n");
        return 0;
    }

    return 1;
}

/* =====================================
   VALIDAR ENDERECO
   Formato: Rua, numero, cidade, estado
   ===================================== */

int validarEndereco(char endereco[]) {
    char copia[150];
    char *rua;
    char *numero;
    char *cidade;
    char *estado;
    int i;
    int temNumero = 0;

    strcpy(copia, endereco);

    rua = strtok(copia, ",");
    numero = strtok(NULL, ",");
    cidade = strtok(NULL, ",");
    estado = strtok(NULL, ",");

    if (rua == NULL || numero == NULL ||
        cidade == NULL || estado == NULL) {
        printf("\nInforme rua, numero, cidade e estado.\n");
        return 0;
    }

    for (i = 0; numero[i] != '\0'; i++) {
        if (isdigit((unsigned char)numero[i])) {
            temNumero = 1;
        }
    }

    if (strlen(rua) == 0 || strlen(numero) == 0 ||
        strlen(cidade) == 0 || strlen(estado) == 0 ||
        !temNumero) {
        printf("\nEndereco incompleto ou numero invalido.\n");
        return 0;
    }

    return 1;
}

int lerEnderecoSeparado(char endereco[], int tamanho) {

    char rua[60];
    char numero[15];
    char cidade[40];
    char estado[30];

    int i;

    printf("\n--- ENDERECO ---\n");

    if (!lerTexto(rua, sizeof(rua), "Rua: ")) {
        return 0;
    }

    if (!campoValido(rua)) {
        return 0;
    }

    if (!lerTexto(numero, sizeof(numero), "Numero: ")) {
        return 0;
    }

    if (!campoValido(numero)) {
        return 0;
    }

    for (i = 0; numero[i] != '\0'; i++) {
        if (!isdigit((unsigned char)numero[i])) {
            printf("\nO numero do endereco deve conter somente digitos.\n");
            return 0;
        }
    }

    if (!lerTexto(cidade, sizeof(cidade), "Cidade: ")) {
        return 0;
    }

    if (!campoValido(cidade)) {
        return 0;
    }

    if (!lerTexto(estado, sizeof(estado), "Estado: ")) {
        return 0;
    }

    if (!campoValido(estado)) {
        return 0;
    }

    if (snprintf(endereco, tamanho, "%s, %s, %s, %s",
                 rua, numero, cidade, estado) >= tamanho) {

        printf("\nEndereco muito grande!\n");
        return 0;
    }

    return 1;
}

/* =====================================
   IMPEDIR PONTO E VIRGULA NOS CAMPOS
   ===================================== */

int campoValido(char texto[]) {
    if (strchr(texto, ';') != NULL ||
        strchr(texto, '\n') != NULL) {
        printf("\nNao utilize ponto e virgula nesse campo.\n");
        return 0;
    }

    return 1;
}

/* =====================================
   SUBSTITUIR ARQUIVO COM SEGURANCA
   ===================================== */

int substituirArquivo(
    char original[],
    char temporario[],
    char backup[]
) {
    remove(backup);

    if (rename(original, backup) != 0) {
        printf("\nErro ao criar copia de seguranca do arquivo.\n");
        return 0;
    }

    if (rename(temporario, original) != 0) {
        printf("\nErro ao substituir o arquivo.\n");

        rename(backup, original);
        return 0;
    }

    remove(backup);

    return 1;
}


/* =====================================
   CADASTRAR DADOS DE RECUPERACAO
   DO GERENTE
   ===================================== */

void cadastrarRecuperacaoGerente(char usuario[]) {

    FILE *arquivo;
    char linha[150];
    char usuarioArquivo[50];
    char cpfArquivo[15];
    char telefoneArquivo[20];

    char cpf[15];
    char telefone[20];

    arquivo = fopen("gerente.txt", "r");

    if (arquivo != NULL) {

        while (fgets(linha, sizeof(linha), arquivo) != NULL) {

            if (sscanf(linha, "%49[^;];%14[^;];%19[^\n]",
                       usuarioArquivo, cpfArquivo,
                       telefoneArquivo) == 3) {

                if (strcmp(usuario, usuarioArquivo) == 0) {
                    fclose(arquivo);
                    return;
                }
            }
        }

        fclose(arquivo);
    }

    printf("\n====================================\n");
    printf("     DADOS DE RECUPERACAO\n");
    printf("====================================\n");

    printf("\nSeu gerente ainda nao possui dados de recuperacao.\n");
    printf("Vamos cadastrar essas informacoes.\n");
    printf("Digite 0 para deixar para depois.\n\n");

    do {
        if (!lerTexto(cpf, sizeof(cpf), "CPF (11 numeros): ")) {
            return;
        }
    } while (!validarCPF(cpf));

    do {
        if (!lerTexto(telefone, sizeof(telefone),
                      "Telefone com DDD: ")) {
            return;
        }

        if (!validarTelefone(telefone)) {
            printf("\nTelefone invalido! Informe 10 ou 11 digitos.\n");
        }

    } while (!validarTelefone(telefone));

    arquivo = fopen("gerente.txt", "a");

    if (arquivo == NULL) {
        printf("\nErro ao salvar dados de recuperacao.\n");
        return;
    }

    fprintf(arquivo, "%s;%s;%s\n", usuario, cpf, telefone);

    fclose(arquivo);

    printf("\nDados de recuperacao cadastrados!\n");
}

/* =====================================
   ATUALIZAR SENHA DE QUALQUER PERFIL
   ===================================== */

int atualizarSenhaGeral(char usuario[], char novaSenha[]) {

    FILE *arquivo;
    FILE *temporario;

    char linha[200];
    char usuarioArquivo[50];
    char senhaArquivo[50];

    int perfil;
    int encontrado = 0;

    arquivo = fopen("usuarios.txt", "r");

    if (arquivo == NULL) {
        printf("\nErro ao abrir usuarios.txt!\n");
        return 0;
    }

    temporario = fopen("usuarios_temp.txt", "w");

    if (temporario == NULL) {
        fclose(arquivo);
        return 0;
    }

    while (fgets(linha, sizeof(linha), arquivo) != NULL) {

        if (sscanf(linha, "%49[^;];%49[^;];%d",
                   usuarioArquivo, senhaArquivo, &perfil) == 3) {

            if (strcmp(usuario, usuarioArquivo) == 0) {

                fprintf(temporario, "%s;%s;%d\n",
                        usuarioArquivo, novaSenha, perfil);

                encontrado = 1;

            } else {
                fprintf(temporario, "%s", linha);
            }

        } else {
            fprintf(temporario, "%s", linha);
        }
    }

    fclose(arquivo);

    if (fclose(temporario) != 0 || !encontrado) {
        remove("usuarios_temp.txt");
        return 0;
    }

    return substituirArquivo(
        "usuarios.txt",
        "usuarios_temp.txt",
        "usuarios_backup.txt"
    );
}

/* =====================================
   RECUPERAR SENHA
   CLIENTE, BALCONISTA OU GERENTE
   ===================================== */

int recuperarSenha(void) {

    FILE *arquivo;

    char usuario[50];
    char cpf[15];
    char telefone[20];

    char usuarioArquivo[50];
    char cpfArquivo[15];
    char telefoneArquivo[20];

    char nomeArquivo[100];
    char emailArquivo[100];
    char enderecoArquivo[150];

    char senha[50];
    char confirmar[50];

    char linha[600];

    int perfil = 0;
    int encontrou = 0;

    printf("\n====================================\n");
    printf("          RECUPERAR SENHA\n");
    printf("====================================\n");

    printf("\nDigite 0 para cancelar.\n\n");

    if (!lerTexto(usuario, sizeof(usuario), "Nome de usuario: ")) {
        return 0;
    }

    /* IDENTIFICAR O PERFIL */

    arquivo = fopen("usuarios.txt", "r");

    if (arquivo == NULL) {
        printf("\nErro ao acessar usuarios.txt!\n");
        return 0;
    }

    while (fgets(linha, sizeof(linha), arquivo) != NULL) {

        char senhaArquivo[50];

        if (sscanf(linha, "%49[^;];%49[^;];%d",
                   usuarioArquivo, senhaArquivo, &perfil) == 3) {

            if (strcmp(usuario, usuarioArquivo) == 0) {
                encontrou = 1;
                break;
            }
        }
    }

    fclose(arquivo);

    if (!encontrou) {
        printf("\nUsuario nao encontrado!\n");
        return 0;
    }

    /* SOLICITAR CPF E TELEFONE */

    if (!lerTexto(cpf, sizeof(cpf), "CPF cadastrado: ")) {
        return 0;
    }

    if (!validarCPF(cpf)) {
        return 0;
    }

    if (!lerTexto(telefone, sizeof(telefone),
                  "Telefone cadastrado: ")) {
        return 0;
    }

    if (!validarTelefone(telefone)) {
        printf("\nTelefone invalido!\n");
        return 0;
    }

    /* CONFERIR DADOS DO GERENTE */

    if (perfil == 1) {
    	
    	encontrou = 0;

        arquivo = fopen("gerente.txt", "r");

        if (arquivo != NULL) {

            while (fgets(linha, sizeof(linha), arquivo) != NULL) {

                if (sscanf(linha, "%49[^;];%14[^;];%19[^\n]",
                           usuarioArquivo, cpfArquivo,
                           telefoneArquivo) == 3) {

                    if (strcmp(usuario, usuarioArquivo) == 0 &&
                        strcmp(cpf, cpfArquivo) == 0 &&
                        strcmp(telefone, telefoneArquivo) == 0) {

                        encontrou = 1;
                        break;
                    }
                }
            }

            fclose(arquivo);

        } else {
            encontrou = 0;
        }
    }

    /* CONFERIR DADOS DO BALCONISTA */

    else if (perfil == 2) {

        encontrou = 0;

        arquivo = fopen("balconistas.txt", "r");

        if (arquivo != NULL) {

            while (fgets(linha, sizeof(linha), arquivo) != NULL) {

                if (sscanf(linha,
                    "%49[^;];%99[^;];%14[^;];%19[^;]",
                    usuarioArquivo, nomeArquivo,
                    cpfArquivo, telefoneArquivo) == 4) {

                    if (strcmp(usuario, usuarioArquivo) == 0 &&
                        strcmp(cpf, cpfArquivo) == 0 &&
                        strcmp(telefone, telefoneArquivo) == 0) {

                        encontrou = 1;
                        break;
                    }
                }
            }

            fclose(arquivo);
        }
    }

    /* CONFERIR DADOS DO CLIENTE */

    else if (perfil == 3) {

        encontrou = 0;

        arquivo = fopen("clientes.txt", "r");

        if (arquivo != NULL) {

            while (fgets(linha, sizeof(linha), arquivo) != NULL) {

                if (sscanf(linha,
                    "%49[^;];%99[^;];%14[^;];%19[^;]",
                    usuarioArquivo, nomeArquivo,
                    cpfArquivo, telefoneArquivo) == 4) {

                    if (strcmp(usuario, usuarioArquivo) == 0 &&
                        strcmp(cpf, cpfArquivo) == 0 &&
                        strcmp(telefone, telefoneArquivo) == 0) {

                        encontrou = 1;
                        break;
                    }
                }
            }

            fclose(arquivo);
        }
    }

    if (!encontrou) {
        printf("\nOs dados informados nao conferem com o cadastro.\n");
        return 0;
    }

    /* CADASTRAR NOVA SENHA */

    do {

        if (!lerTexto(senha, sizeof(senha), "Nova senha: ")) {
            return 0;
        }

    } while (!validarSenha(senha, usuario));

    if (!lerTexto(confirmar, sizeof(confirmar),
                  "Confirme a nova senha: ")) {
        return 0;
    }

    if (strcmp(senha, confirmar) != 0) {
        printf("\nAs senhas nao conferem!\n");
        return 0;
    }

    if (!atualizarSenhaGeral(usuario, senha)) {
        printf("\nNao foi possivel atualizar a senha.\n");
        return 0;
    }

    printf("\nSenha alterada com sucesso!\n");
    printf("Agora voce pode entrar na sua conta.\n");

    return 1;
}


/* =====================================
   VERIFICAR USUARIO EXISTENTE
   ===================================== */

int usuarioExiste(char usuario[]) {
    FILE *arquivo;
    char linha[200];
    char usuarioArquivo[50];
    char senhaArquivo[50];
    int perfil;

    arquivo = fopen("usuarios.txt", "r");

    if (arquivo == NULL) {
        return 0;
    }

    while (fgets(linha, sizeof(linha), arquivo) != NULL) {
        if (sscanf(linha, "%49[^;];%49[^;];%d",
                   usuarioArquivo, senhaArquivo, &perfil) == 3) {

            if (strcmp(usuario, usuarioArquivo) == 0) {
                fclose(arquivo);
                return 1;
            }
        }
    }

    fclose(arquivo);
    return 0;
}

/* =====================================
   VERIFICAR CPF EXISTENTE
   ===================================== */

int cpfExiste(char cpf[]) {
    FILE *arquivo;
    char linha[400];
    char usuario[50];
    char nome[100];
    char cpfArquivo[15];

    arquivo = fopen("clientes.txt", "r");

    if (arquivo == NULL) {
        return 0;
    }

    while (fgets(linha, sizeof(linha), arquivo) != NULL) {
        if (sscanf(linha, "%49[^;];%99[^;];%14[^;]",
                   usuario, nome, cpfArquivo) == 3) {

            if (strcmp(cpf, cpfArquivo) == 0) {
                fclose(arquivo);
                return 1;
            }
        }
    }

    fclose(arquivo);
    return 0;
}

/* =====================================
   CRIAR PRIMEIRO GERENTE
   ===================================== */

int criarPrimeiroGerente(void) {

    FILE *arquivo;

    char usuario[50];
    char senha[50];
    char confirmar[50];

    printf("\n=== PRIMEIRO ACESSO ===\n");
    printf("Digite 0 para cancelar.\n\n");

    if (!lerTexto(usuario, sizeof(usuario), "Usuario: ")) {
        return 0;
    }

    if (!campoValido(usuario)) {
        return 0;
    }

    do {
        if (!lerTexto(senha, sizeof(senha), "Senha: ")) {
            return 0;
        }

    } while (!validarSenha(senha, usuario));

    if (!lerTexto(confirmar, sizeof(confirmar), "Confirme a senha: ")) {
        return 0;
    }

    if (strcmp(senha, confirmar) != 0) {
        printf("\nAs senhas nao conferem!\n");
        return 0;
    }

    arquivo = fopen("usuarios.txt", "a");

    if (arquivo == NULL) {
        printf("\nErro ao criar arquivo!\n");
        return 0;
    }

    fprintf(arquivo, "%s;%s;1\n", usuario, senha);

    if (fclose(arquivo) != 0) {
        printf("\nErro ao salvar gerente!\n");
        return 0;
    }

    printf("\nGerente cadastrado com sucesso!\n");

    cadastrarRecuperacaoGerente(usuario);

    return 1;
}
/* =====================================
   LOGIN - IDENTIDADE VISUAL VAN GOGH
   ===================================== */

int cadastroELogin(
    char usuarioLogado[],
    int tamanho,
    int *perfilLogado
) {
    FILE *arquivo;

    char usuario[50];
    char senha[50];
    char usuarioArquivo[50];
    char senhaArquivo[50];
    char linha[200];

    int perfil;
    int tentativas;
    int opcao;

    if (tamanho <= 0 || perfilLogado == NULL) {
        return 0;
    }

    arquivo = fopen("usuarios.txt", "r");

    if (arquivo == NULL) {
        if (!criarPrimeiroGerente()) {
            return 0;
        }
    } else {
        fclose(arquivo);
    }

    while (1) {

        system("cls");

        cabecalhoLogin();

        corLogin(LOGIN_VERDE);
        printf("==================================================\n");

        corLogin(LOGIN_DOURADO);
        printf("                    BEM-VINDO!\n");

        corLogin(LOGIN_VERDE);
        printf("==================================================\n");

        restaurarCorLogin();

        printf("\n");
        printf("       A sua floricultura de momentos especiais.\n");

        printf("\n");
        printf("       1 - ENTRAR NA MINHA CONTA\n");
        printf("       2 - CRIAR CONTA\n");
        printf("       3 - ESQUECI MINHA SENHA\n");
        printf("       0 - SAIR\n");

        corLogin(LOGIN_VERDE);
        printf("\n--------------------------------------------------\n");

        restaurarCorLogin();
        printf("Escolha uma opcao: ");

        if (scanf("%d", &opcao) != 1) {
            limparTeclado();

            corLogin(LOGIN_VERMELHO);
            printf("\nOpcao invalida!\n");
            restaurarCorLogin();

            system("pause");
            continue;
        }

        limparTeclado();

        if (opcao == 0) {
            return 0;
        }

        if (opcao == 2) {

            cadastrarCliente();

            system("pause");
            continue;
        }
        
        if (opcao == 3) {

    	system("cls");

    	cabecalhoLogin();

    	recuperarSenha();

    	system("pause");
    	continue;
}

        if (opcao != 1) {

            corLogin(LOGIN_VERMELHO);
            printf("\nOpcao invalida!\n");
            restaurarCorLogin();

            system("pause");
            continue;
        }

        /* =====================================
           ACESSAR MINHA CONTA
           ===================================== */

        tentativas = 3;

        while (tentativas > 0) {

            system("cls");

            cabecalhoLogin();

            corLogin(LOGIN_VERDE);
            printf("==================================================\n");

            corLogin(LOGIN_DOURADO);
            printf("                  ACESSAR CONTA\n");

            corLogin(LOGIN_VERDE);
            printf("==================================================\n");

            restaurarCorLogin();

            printf("\nDigite 0 no usuario para voltar.\n\n");

            if (!lerTexto(usuario, sizeof(usuario), "Usuario: ")) {
                break;
            }

            if (!lerTexto(senha, sizeof(senha), "Senha: ")) {
                break;
            }

            arquivo = fopen("usuarios.txt", "r");

            if (arquivo == NULL) {

                corLogin(LOGIN_VERMELHO);
                printf("\nErro ao abrir usuarios.txt!\n");
                restaurarCorLogin();

                return 0;
            }

            while (fgets(linha, sizeof(linha), arquivo) != NULL) {

                if (sscanf(linha, "%49[^;];%49[^;];%d",
                           usuarioArquivo,
                           senhaArquivo,
                           &perfil) == 3) {

                    if (strcmp(usuario, usuarioArquivo) == 0 &&
                        strcmp(senha, senhaArquivo) == 0) {

                        fclose(arquivo);

                        strncpy(
                            usuarioLogado,
                            usuarioArquivo,
                            tamanho - 1
                        );

                        usuarioLogado[tamanho - 1] = '\0';

                        *perfilLogado = perfil;
                        
                        /* GARANTIR DADOS DE RECUPERACAO DO GERENTE */

						if (perfil == 1) {
    					cadastrarRecuperacaoGerente(usuarioArquivo);
}

                        corLogin(LOGIN_VERDE);
                        printf("\nLogin realizado com sucesso!\n");
                        restaurarCorLogin();

                        system("pause");

                        return 1;
                    }
                }
            }

            fclose(arquivo);

            tentativas--;

            corLogin(LOGIN_VERMELHO);
            printf("\nUsuario ou senha incorretos!\n");
            restaurarCorLogin();

            printf("Tentativas restantes: %d\n", tentativas);

            if (tentativas > 0) {
                system("pause");
            }
        }
    }
}


/* =====================================
   CADASTRAR BALCONISTA
   ===================================== */


void cadastrarBalconista(void) {

    FILE *arquivoUsuarios;
    FILE *arquivoBalconistas;

    char nome[100];
    char cpf[15];
    char telefone[20];
    char email[100];
    char endereco[150];

    char usuario[50];
    char senha[50];
    char confirmar[50];

    printf("\n====================================\n");
    printf("       CADASTRO DE BALCONISTA\n");
    printf("====================================\n");

    printf("\nDigite 0 em qualquer campo para cancelar.\n");

    if (!lerTexto(nome, sizeof(nome), "\nNome completo: ")) {
        return;
    }

    if (!campoValido(nome)) {
        return;
    }

    do {
        if (!lerTexto(cpf, sizeof(cpf), "\nCPF (11 numeros): ")) {
            return;
        }
    } while (!validarCPF(cpf));

    do {
        if (!lerTexto(telefone, sizeof(telefone),
                      "\nTelefone com DDD: ")) {
            return;
        }
    } while (!validarTelefone(telefone));

    do {
        if (!lerTexto(email, sizeof(email), "\nEmail: ")) {
            return;
        }
    } while (!validarEmail(email));

    if (!campoValido(email)) {
        return;
    }

    if (!lerEnderecoSeparado(endereco, sizeof(endereco))) {
        return;
    }

    if (!lerTexto(usuario, sizeof(usuario), "\nNome de usuario: ")) {
        return;
    }

    if (!campoValido(usuario)) {
        return;
    }

    if (usuarioExiste(usuario)) {
        printf("\nEsse usuario ja existe!\n");
        return;
    }

    do {
        if (!lerTexto(senha, sizeof(senha), "\nNova senha: ")) {
            return;
        }
    } while (!validarSenha(senha, nome) ||
             !validarSenha(senha, usuario));

    if (!lerTexto(confirmar, sizeof(confirmar),
                  "\nConfirme a senha: ")) {
        return;
    }

    if (strcmp(senha, confirmar) != 0) {
        printf("\nAs senhas nao conferem!\n");
        return;
    }

    arquivoUsuarios = fopen("usuarios.txt", "a");

    if (arquivoUsuarios == NULL) {
        printf("\nErro ao abrir usuarios.txt!\n");
        return;
    }

    arquivoBalconistas = fopen("balconistas.txt", "a");

    if (arquivoBalconistas == NULL) {
        fclose(arquivoUsuarios);
        printf("\nErro ao abrir balconistas.txt!\n");
        return;
    }

    fprintf(arquivoUsuarios, "%s;%s;2\n", usuario, senha);

    fprintf(arquivoBalconistas, "%s;%s;%s;%s;%s;%s\n",
            usuario, nome, cpf, telefone, email, endereco);

    fclose(arquivoUsuarios);
    fclose(arquivoBalconistas);

    printf("\nBalconista cadastrado com sucesso!\n");
}
/* =====================================
   CADASTRAR CLIENTE
   ===================================== */

/* =====================================
   CADASTRAR CLIENTE
   VISUAL VAN GOGH
   ===================================== */



void cadastrarCliente(void) {

    FILE *arquivoClientes;
    FILE *arquivoUsuarios;

    char nome[100];
    char cpf[15];
    char telefone[20];
    char email[100];

    char rua[60];
    char numero[15];
    char cidade[40];
    char estado[30];
    char endereco[150];

    char usuario[50];
    char senha[50];
    char confirmar[50];

    int i;
    int quantidadeDigitos;
    int formatoValido;

    /* =====================================
       CABECALHO
       ===================================== */

    system("cls");

    cabecalhoLogin();

    corLogin(LOGIN_DOURADO);
    printf("\n              CRIAR MINHA CONTA\n");

    corLogin(LOGIN_BRANCO);
    printf("        Flores que encantam momentos\n");

    corLogin(LOGIN_VERDE);
    printf("\n==================================================\n");

    corLogin(LOGIN_VERMELHO);
    printf("Digite 0 em qualquer campo para cancelar.\n");

    corLogin(LOGIN_VERDE);
    printf("==================================================\n");

    /* =====================================
       ETAPA 1 - DADOS PESSOAIS
       ===================================== */

    corLogin(LOGIN_DOURADO);
    printf("\n  01 - DADOS PESSOAIS\n");

    corLogin(LOGIN_VERDE);
    printf("--------------------------------------------------\n");

    corLogin(LOGIN_BRANCO);

    if (!lerTexto(nome, sizeof(nome),
                  "\nNome completo: ")) {
        return;
    }

    if (!campoValido(nome)) {
        return;
    }

    do {

        if (!lerTexto(cpf, sizeof(cpf),
                      "\nCPF (11 numeros): ")) {
            return;
        }

    } while (!validarCPF(cpf));

    if (cpfExiste(cpf)) {

        corLogin(LOGIN_VERMELHO);
        printf("\nEsse CPF ja esta cadastrado!\n");

        restaurarCorLogin();
        system("pause");

        return;
    }

    /* =====================================
   TELEFONE COM DDD
   ===================================== */

do {

    if (!lerTexto(telefone, sizeof(telefone),
                  "\nTelefone com DDD: ")) {
        return;
    }

    if (!campoValido(telefone)) {
        return;
    }

} while (!validarTelefone(telefone));

    /* =====================================
       ETAPA 2 - CONTATO E ENDERECO
       ===================================== */

    corLogin(LOGIN_DOURADO);
    printf("\n\n  02 - CONTATO E ENDERECO\n");

    corLogin(LOGIN_VERDE);
    printf("--------------------------------------------------\n");

    corLogin(LOGIN_BRANCO);

    do {

        if (!lerTexto(email, sizeof(email),
                      "\nE-mail: ")) {
            return;
        }

        if (!campoValido(email)) {
            return;
        }

    } while (!validarEmail(email));

    /* RUA */

    if (!lerTexto(rua, sizeof(rua),
                  "\nRua: ")) {
        return;
    }

    if (!campoValido(rua)) {
        return;
    }

    /* NUMERO */

    if (!lerTexto(numero, sizeof(numero),
                  "\nNumero: ")) {
        return;
    }

    if (!campoValido(numero)) {
        return;
    }

    /* CIDADE */

    if (!lerTexto(cidade, sizeof(cidade),
                  "\nCidade: ")) {
        return;
    }

    if (!campoValido(cidade)) {
        return;
    }

    /* ESTADO */

    if (!lerTexto(estado, sizeof(estado),
                  "\nEstado: ")) {
        return;
    }

    if (!campoValido(estado)) {
        return;
    }

    /* JUNTAR OS CAMPOS DO ENDERECO */

    if (snprintf(endereco, sizeof(endereco),
                 "%s, %s, %s, %s",
                 rua, numero, cidade, estado)
        >= (int)sizeof(endereco)) {

        corLogin(LOGIN_VERMELHO);
        printf("\nEndereco muito grande!\n");

        restaurarCorLogin();
        system("pause");

        return;
    }

    if (!validarEndereco(endereco)) {
        return;
    }

    /* =====================================
       ETAPA 3 - DADOS DE ACESSO
       ===================================== */

    corLogin(LOGIN_DOURADO);
    printf("\n\n  03 - DADOS DE ACESSO\n");

    corLogin(LOGIN_VERDE);
    printf("--------------------------------------------------\n");

    corLogin(LOGIN_BRANCO);

    if (!lerTexto(usuario, sizeof(usuario),
                  "\nCrie seu nome de usuario: ")) {
        return;
    }

    if (!campoValido(usuario)) {
        return;
    }

    if (usuarioExiste(usuario)) {

        corLogin(LOGIN_VERMELHO);
        printf("\nEsse usuario ja esta cadastrado!\n");

        restaurarCorLogin();
        system("pause");

        return;
    }

    do {

        if (!lerTexto(senha, sizeof(senha),
                      "\nCrie sua senha: ")) {
            return;
        }

    } while (!validarSenha(senha, nome) ||
             !validarSenha(senha, usuario));

    if (!lerTexto(confirmar, sizeof(confirmar),
                  "\nConfirme sua senha: ")) {
        return;
    }

    if (strcmp(senha, confirmar) != 0) {

        corLogin(LOGIN_VERMELHO);
        printf("\nAs senhas nao conferem!\n");

        restaurarCorLogin();
        system("pause");

        return;
    }

    /* =====================================
       SALVAR DADOS DO CLIENTE
       ===================================== */

    arquivoClientes = fopen("clientes.txt", "a");

    if (arquivoClientes == NULL) {

        corLogin(LOGIN_VERMELHO);
        printf("\nErro ao abrir clientes.txt!\n");

        restaurarCorLogin();
        system("pause");

        return;
    }

    arquivoUsuarios = fopen("usuarios.txt", "a");

    if (arquivoUsuarios == NULL) {

        corLogin(LOGIN_VERMELHO);
        printf("\nErro ao abrir usuarios.txt!\n");

        fclose(arquivoClientes);

        restaurarCorLogin();
        system("pause");

        return;
    }

    /* SALVAR INFORMACOES PESSOAIS */

    fprintf(arquivoClientes,
            "%s;%s;%s;%s;%s;%s\n",
            usuario,
            nome,
            cpf,
            telefone,
            email,
            endereco);

    /* SALVAR INFORMACOES DE ACESSO */

    fprintf(arquivoUsuarios,
            "%s;%s;3\n",
            usuario,
            senha);

    /* FECHAR ARQUIVOS */

    if (fclose(arquivoClientes) != 0) {

        corLogin(LOGIN_VERMELHO);
        printf("\nErro ao salvar os dados do cliente!\n");

        fclose(arquivoUsuarios);

        restaurarCorLogin();
        system("pause");

        return;
    }

    if (fclose(arquivoUsuarios) != 0) {

        corLogin(LOGIN_VERMELHO);
        printf("\nErro ao salvar os dados de login!\n");

        restaurarCorLogin();
        system("pause");

        return;
    }

    /* =====================================
       CONFIRMACAO DO CADASTRO
       ===================================== */

    system("cls");

    cabecalhoLogin();

    corLogin(LOGIN_VERDE);

    printf("\n==================================================\n");
    printf("                                                  \n");
    printf("          CADASTRO REALIZADO COM SUCESSO!         \n");
    printf("                                                  \n");
    printf("==================================================\n");

    corLogin(LOGIN_BRANCO);

    printf("\nBem-vindo(a), %s!\n", nome);
    printf("\nSeu nome de usuario: %s\n", usuario);

    printf("\nSeu cadastro foi salvo com sucesso.");

    corLogin(LOGIN_DOURADO);

    printf("\n\nAgora voce ja pode acessar a VAN GOGH!");

    corLogin(LOGIN_VERDE);

    printf("\n\n==================================================\n");

    corLogin(LOGIN_DOURADO);

    printf("          FLORES QUE ENCANTAM MOMENTOS            \n");

    corLogin(LOGIN_VERDE);

    printf("==================================================\n");

    restaurarCorLogin();

    system("pause");
}

/* =====================================
   ALTERAR USUARIO DO BALCONISTA
   ===================================== */

int alterarUsuarioBalconista(char usuario[], char novoUsuario[]) {

    FILE *arquivo;
    FILE *tempBalconistas;
    FILE *tempUsuarios;

    char linha[600];
    char usuarioArquivo[50];
    char restante[550];

    char senhaArquivo[150];
    int perfil;

    int encontrouBalconista = 0;
    int encontrouUsuario = 0;

    if (strlen(novoUsuario) == 0 || strlen(novoUsuario) >= 50) {
        printf("\nNome de usuario invalido!\n");
        return 0;
    }

    if (strcmp(usuario, novoUsuario) == 0) {
        printf("\nO novo usuario deve ser diferente do atual.\n");
        return 0;
    }

    if (usuarioExisteParaOutroCliente(novoUsuario, usuario)) {
        printf("\nEsse nome de usuario ja esta cadastrado!\n");
        return 0;
    }

    /* ATUALIZAR BALCONISTAS.TXT */

    arquivo = fopen("balconistas.txt", "r");

    if (arquivo == NULL) {
        printf("\nErro ao abrir balconistas.txt!\n");
        return 0;
    }

    tempBalconistas = fopen("balconistas_temp.txt", "w");

    if (tempBalconistas == NULL) {
        fclose(arquivo);
        return 0;
    }

    while (fgets(linha, sizeof(linha), arquivo) != NULL) {

        linha[strcspn(linha, "\n")] = '\0';

        if (sscanf(linha, "%49[^;];%549[^\n]",
                   usuarioArquivo, restante) == 2) {

            if (strcmp(usuarioArquivo, usuario) == 0) {
                fprintf(tempBalconistas, "%s;%s\n",
                        novoUsuario, restante);

                encontrouBalconista = 1;

            } else {
                fprintf(tempBalconistas, "%s\n", linha);
            }

        } else {
            fprintf(tempBalconistas, "%s\n", linha);
        }
    }

    fclose(arquivo);

    if (fclose(tempBalconistas) != 0 || !encontrouBalconista) {
        remove("balconistas_temp.txt");
        printf("\nCadastro do balconista nao encontrado.\n");
        return 0;
    }

    /* ATUALIZAR USUARIOS.TXT */

    arquivo = fopen("usuarios.txt", "r");

    if (arquivo == NULL) {
        remove("balconistas_temp.txt");
        return 0;
    }

    tempUsuarios = fopen("usuarios_temp.txt", "w");

    if (tempUsuarios == NULL) {
        fclose(arquivo);
        remove("balconistas_temp.txt");
        return 0;
    }

    while (fgets(linha, sizeof(linha), arquivo) != NULL) {

        if (sscanf(linha, "%49[^;];%149[^;];%d",
                   usuarioArquivo, senhaArquivo, &perfil) == 3) {

            if (strcmp(usuarioArquivo, usuario) == 0 &&
                perfil == 2) {

                fprintf(tempUsuarios, "%s;%s;2\n",
                        novoUsuario, senhaArquivo);

                encontrouUsuario = 1;

            } else {
                fprintf(tempUsuarios, "%s", linha);
            }

        } else {
            fprintf(tempUsuarios, "%s", linha);
        }
    }

    fclose(arquivo);

    if (fclose(tempUsuarios) != 0 || !encontrouUsuario) {
        remove("balconistas_temp.txt");
        remove("usuarios_temp.txt");
        printf("\nLogin do balconista nao encontrado.\n");
        return 0;
    }

    /* CRIAR BACKUPS */

    remove("balconistas_backup.txt");
    remove("usuarios_backup.txt");

    if (rename("balconistas.txt", "balconistas_backup.txt") != 0) {
        remove("balconistas_temp.txt");
        remove("usuarios_temp.txt");
        return 0;
    }

    if (rename("usuarios.txt", "usuarios_backup.txt") != 0) {
        rename("balconistas_backup.txt", "balconistas.txt");
        remove("balconistas_temp.txt");
        remove("usuarios_temp.txt");
        return 0;
    }

    /* SUBSTITUIR OS ARQUIVOS */

    if (rename("balconistas_temp.txt", "balconistas.txt") != 0) {

        rename("balconistas_backup.txt", "balconistas.txt");
        rename("usuarios_backup.txt", "usuarios.txt");

        remove("usuarios_temp.txt");

        return 0;
    }

    if (rename("usuarios_temp.txt", "usuarios.txt") != 0) {

        remove("balconistas.txt");

        rename("balconistas_backup.txt", "balconistas.txt");
        rename("usuarios_backup.txt", "usuarios.txt");

        return 0;
    }

    remove("balconistas_backup.txt");
    remove("usuarios_backup.txt");

    strcpy(usuario, novoUsuario);

    return 1;
}


/* =====================================
   ATUALIZAR DADOS DO CLIENTE
   ===================================== */

int atualizarDadosCliente(
    char usuario[],
    int opcao,
    char novoValor[]
) {
    FILE *arquivo;
    FILE *temporario;

    char linha[600];

    char usuarioArquivo[50];
    char nome[100];
    char cpf[15];
    char telefone[20];
    char email[100];
    char endereco[150];

    int encontrado = 0;

    arquivo = fopen("clientes.txt", "r");

    if (arquivo == NULL) {
        printf("\nErro ao abrir clientes.txt!\n");
        return 0;
    }

    temporario = fopen("clientes_temp.txt", "w");

    if (temporario == NULL) {
        fclose(arquivo);
        printf("\nErro ao criar arquivo temporario!\n");
        return 0;
    }

    while (fgets(linha, sizeof(linha), arquivo) != NULL) {

        linha[strcspn(linha, "\n")] = '\0';

        if (sscanf(linha,
            "%49[^;];%99[^;];%14[^;];%19[^;];%99[^;];%149[^\n]",
            usuarioArquivo, nome, cpf, telefone, email, endereco) == 6) {

            if (strcmp(usuario, usuarioArquivo) == 0) {

                encontrado = 1;

                switch (opcao) {

                    case 2:
                        strcpy(nome, novoValor);
                        break;

                    case 3:
                        strcpy(telefone, novoValor);
                        break;

                    case 4:
                        strcpy(email, novoValor);
                        break;

                    case 5:
                        strcpy(endereco, novoValor);
                        break;
                }

                fprintf(temporario,
                    "%s;%s;%s;%s;%s;%s\n",
                    usuarioArquivo,
                    nome,
                    cpf,
                    telefone,
                    email,
                    endereco
                );

            } else {
                fprintf(temporario, "%s\n", linha);
            }

        } else {
            fprintf(temporario, "%s\n", linha);
        }
    }

    fclose(arquivo);

    if (fclose(temporario) != 0) {
        printf("\nErro ao gravar arquivo temporario!\n");
        remove("clientes_temp.txt");
        return 0;
    }

    if (!encontrado) {
        remove("clientes_temp.txt");
        printf("\nCliente nao encontrado!\n");
        return 0;
    }

    return substituirArquivo(
        "clientes.txt",
        "clientes_temp.txt",
        "clientes_backup.txt"
    );
}
/* =====================================
   ATUALIZAR DADOS DO BALCONISTA
   ===================================== */

int atualizarDadosBalconista(
    char usuario[],
    int opcao,
    char novoValor[]
) {
    FILE *arquivo;
    FILE *temporario;

    char linha[600];

    char usuarioArquivo[50];
    char nome[100];
    char cpf[15];
    char telefone[20];
    char email[100];
    char endereco[150];

    int encontrado = 0;

    arquivo = fopen("balconistas.txt", "r");

    if (arquivo == NULL) {
        printf("\nErro ao abrir balconistas.txt!\n");
        return 0;
    }

    temporario = fopen("balconistas_temp.txt", "w");

    if (temporario == NULL) {
        fclose(arquivo);
        printf("\nErro ao criar arquivo temporario!\n");
        return 0;
    }

    while (fgets(linha, sizeof(linha), arquivo) != NULL) {

        linha[strcspn(linha, "\n")] = '\0';

        if (sscanf(linha,
            "%49[^;];%99[^;];%14[^;];%19[^;];%99[^;];%149[^\n]",
            usuarioArquivo, nome, cpf, telefone, email, endereco) == 6) {

            if (strcmp(usuario, usuarioArquivo) == 0) {

                encontrado = 1;

                switch (opcao) {

                    case 2:
                        strcpy(nome, novoValor);
                        break;

                    case 3:
                        strcpy(telefone, novoValor);
                        break;

                    case 4:
                        strcpy(email, novoValor);
                        break;

                    case 5:
                        strcpy(endereco, novoValor);
                        break;
                }

                fprintf(temporario,
                    "%s;%s;%s;%s;%s;%s\n",
                    usuarioArquivo,
                    nome,
                    cpf,
                    telefone,
                    email,
                    endereco
                );

            } else {
                fprintf(temporario, "%s\n", linha);
            }

        } else {
            fprintf(temporario, "%s\n", linha);
        }
    }

    fclose(arquivo);

    if (fclose(temporario) != 0) {
        printf("\nErro ao gravar arquivo temporario!\n");
        remove("balconistas_temp.txt");
        return 0;
    }

    if (!encontrado) {
        remove("balconistas_temp.txt");
        printf("\nBalconista nao encontrado!\n");
        return 0;
    }

    return substituirArquivo(
        "balconistas.txt",
        "balconistas_temp.txt",
        "balconistas_backup.txt"
    );
}

/* =====================================
   ALTERAR SENHA DO CLIENTE
   ===================================== */

int alterarSenhaCliente(char usuario[], char novaSenha[]) {
    FILE *arquivo;
    FILE *temporario;

    char linha[200];
    char usuarioArquivo[50];
    char senhaArquivo[50];

    int perfil;
    int encontrado = 0;

    arquivo = fopen("usuarios.txt", "r");

    if (arquivo == NULL) {
        printf("\nErro ao abrir usuarios.txt!\n");
        return 0;
    }

    temporario = fopen("usuarios_temp.txt", "w");

    if (temporario == NULL) {
        fclose(arquivo);
        printf("\nErro ao criar arquivo temporario!\n");
        return 0;
    }

    while (fgets(linha, sizeof(linha), arquivo) != NULL) {
        if (sscanf(linha, "%49[^;];%49[^;];%d",
                   usuarioArquivo, senhaArquivo, &perfil) == 3) {

            if (strcmp(usuario, usuarioArquivo) == 0 && perfil == 3) {
                fprintf(temporario, "%s;%s;3\n",
                        usuarioArquivo, novaSenha);

                encontrado = 1;
            } else {
                fprintf(temporario, "%s", linha);
            }
        } else {
            fprintf(temporario, "%s", linha);
        }
    }

    fclose(arquivo);

    if (fclose(temporario) != 0) {
        printf("\nErro ao gravar arquivo temporario!\n");
        remove("usuarios_temp.txt");
        return 0;
    }

    if (!encontrado) {
        remove("usuarios_temp.txt");
        printf("\nCliente nao encontrado no arquivo de usuarios!\n");
        return 0;
    }

    return substituirArquivo(
        "usuarios.txt",
        "usuarios_temp.txt",
        "usuarios_backup.txt"
    );
}

/* =====================================
   ALTERAR SENHA DO BALCONISTA
   ===================================== */

int alterarSenhaBalconista(char usuario[], char novaSenha[]) {

    FILE *arquivo;
    FILE *temporario;

    char linha[200];
    char usuarioArquivo[50];
    char senhaArquivo[50];

    int perfil;
    int encontrado = 0;

    arquivo = fopen("usuarios.txt", "r");

    if (arquivo == NULL) {
        printf("\nErro ao abrir usuarios.txt!\n");
        return 0;
    }

    temporario = fopen("usuarios_temp.txt", "w");

    if (temporario == NULL) {
        fclose(arquivo);
        printf("\nErro ao criar arquivo temporario!\n");
        return 0;
    }

    while (fgets(linha, sizeof(linha), arquivo) != NULL) {

        if (sscanf(linha, "%49[^;];%49[^;];%d",
                   usuarioArquivo, senhaArquivo, &perfil) == 3) {

            if (strcmp(usuario, usuarioArquivo) == 0 && perfil == 2) {

                fprintf(temporario, "%s;%s;2\n",
                        usuarioArquivo, novaSenha);

                encontrado = 1;

            } else {
                fprintf(temporario, "%s", linha);
            }

        } else {
            fprintf(temporario, "%s", linha);
        }
    }

    fclose(arquivo);

    if (fclose(temporario) != 0) {
        printf("\nErro ao gravar arquivo temporario!\n");
        remove("usuarios_temp.txt");
        return 0;
    }

    if (!encontrado) {
        remove("usuarios_temp.txt");
        printf("\nBalconista nao encontrado no arquivo de usuarios!\n");
        return 0;
    }

    return substituirArquivo(
        "usuarios.txt",
        "usuarios_temp.txt",
        "usuarios_backup.txt"
    );
}

/* =====================================
   CONSULTAR DADOS DO CLIENTE
   ===================================== */

void consultarDadosCliente(char usuario[]) {

    FILE *arquivo;

    char linha[600];

    char usuarioArquivo[50];
    char nome[100];
    char cpf[15];
    char telefone[20];
    char email[100];
    char endereco[150];

    int encontrado = 0;

    arquivo = fopen("clientes.txt", "r");

    if (arquivo == NULL) {
        printf("\nErro ao abrir clientes.txt!\n");
        return;
    }

    while (fgets(linha, sizeof(linha), arquivo) != NULL) {

        if (sscanf(linha,
            "%49[^;];%99[^;];%14[^;];%19[^;];%99[^;];%149[^\n]",
            usuarioArquivo,
            nome,
            cpf,
            telefone,
            email,
            endereco) == 6) {

            if (strcmp(usuario, usuarioArquivo) == 0) {

                encontrado = 1;

                printf("\n====================================\n");
                printf("        MEUS DADOS CADASTRAIS\n");
                printf("====================================\n");

                printf("\nNome completo: %s\n", nome);
                printf("Usuario:       %s\n", usuarioArquivo);
                printf("CPF:           %s\n", cpf);
                printf("Telefone:      %s\n", telefone);
                printf("Email:         %s\n", email);
                printf("Endereco:      %s\n", endereco);

                printf("\n====================================\n");

                break;
            }
        }
    }

    fclose(arquivo);

    if (!encontrado) {
        printf("\nNao foi possivel localizar seus dados.\n");
    }
}

/* =====================================
   CONSULTAR DADOS DO BALCONISTA
   ===================================== */

int consultarDadosBalconista(char usuario[]) {

    FILE *arquivo;

    char linha[600];

    char usuarioArquivo[50];
    char nome[100];
    char cpf[15];
    char telefone[20];
    char email[100];
    char endereco[150];

    int encontrado = 0;

    arquivo = fopen("balconistas.txt", "r");

    if (arquivo == NULL) {
        printf("\nErro ao abrir balconistas.txt!\n");
        return 0;
    }

    while (fgets(linha, sizeof(linha), arquivo) != NULL) {

        if (sscanf(linha,
            "%49[^;];%99[^;];%14[^;];%19[^;];%99[^;];%149[^\n]",
            usuarioArquivo,
            nome,
            cpf,
            telefone,
            email,
            endereco) == 6) {

            if (strcmp(usuario, usuarioArquivo) == 0) {

                encontrado = 1;

                printf("\n====================================\n");
                printf("        MEUS DADOS CADASTRAIS\n");
                printf("====================================\n");

                printf("\nNome completo: %s\n", nome);
                printf("Usuario:       %s\n", usuarioArquivo);
                printf("CPF:           %s\n", cpf);
                printf("Telefone:      %s\n", telefone);
                printf("Email:         %s\n", email);
                printf("Endereco:      %s\n", endereco);

                printf("\n====================================\n");

                break;
            }
        }
    }

    fclose(arquivo);

    if (!encontrado) {
        printf("\nNao foi possivel localizar seus dados cadastrais.\n");
        return 0;
    }

    return 1;
}

/* =====================================
   VERIFICAR USUARIO DE OUTRO CLIENTE
   ===================================== */

int usuarioExisteParaOutroCliente(
    char novoUsuario[],
    char usuarioAtual[]
) {
    FILE *arquivo;

    char linha[200];
    char usuarioArquivo[50];

    arquivo = fopen("usuarios.txt", "r");

    if (arquivo == NULL) {
        return 0;
    }

    while (fgets(linha, sizeof(linha), arquivo) != NULL) {

        if (sscanf(linha, "%49[^;]", usuarioArquivo) == 1) {

            if (strcmp(novoUsuario, usuarioArquivo) == 0 &&
                strcmp(usuarioAtual, usuarioArquivo) != 0) {

                fclose(arquivo);
                return 1;
            }
        }
    }

    fclose(arquivo);

    return 0;
}
/* =====================================
   ALTERAR NOME DE USUARIO DO CLIENTE
   ===================================== */

int alterarUsuarioCliente(char usuario[], char novoUsuario[]) {

    FILE *arquivo;
    FILE *tempClientes;
    FILE *tempUsuarios;

    char linha[600];
    char usuarioArquivo[50];
    char restante[550];

    char senhaArquivo[150];
    int perfil;

    int encontrouCliente = 0;
    int encontrouUsuario = 0;

    if (strlen(novoUsuario) == 0 || strlen(novoUsuario) >= 50) {
        printf("\nNome de usuario invalido!\n");
        return 0;
    }

    if (strcmp(usuario, novoUsuario) == 0) {
        printf("\nO novo usuario deve ser diferente do atual.\n");
        return 0;
    }

    if (usuarioExisteParaOutroCliente(novoUsuario, usuario)) {
        printf("\nEsse nome de usuario ja esta cadastrado!\n");
        return 0;
    }

    /* PREPARAR NOVO CLIENTES.TXT */

    arquivo = fopen("clientes.txt", "r");

    if (arquivo == NULL) {
        printf("\nErro ao abrir clientes.txt!\n");
        return 0;
    }

    tempClientes = fopen("clientes_temp.txt", "w");

    if (tempClientes == NULL) {
        fclose(arquivo);
        return 0;
    }

    while (fgets(linha, sizeof(linha), arquivo) != NULL) {

        linha[strcspn(linha, "\n")] = '\0';

        if (sscanf(linha, "%49[^;];%549[^\n]",
                   usuarioArquivo, restante) == 2) {

            if (strcmp(usuarioArquivo, usuario) == 0) {
                fprintf(tempClientes, "%s;%s\n",
                        novoUsuario, restante);
                encontrouCliente = 1;
            } else {
                fprintf(tempClientes, "%s\n", linha);
            }

        } else {
            fprintf(tempClientes, "%s\n", linha);
        }
    }

    fclose(arquivo);

    if (fclose(tempClientes) != 0 || !encontrouCliente) {
        remove("clientes_temp.txt");
        printf("\nCadastro do cliente nao encontrado ou erro ao gravar.\n");
        return 0;
    }

    /* PREPARAR NOVO USUARIOS.TXT */

    arquivo = fopen("usuarios.txt", "r");

    if (arquivo == NULL) {
        remove("clientes_temp.txt");
        return 0;
    }

    tempUsuarios = fopen("usuarios_temp.txt", "w");

    if (tempUsuarios == NULL) {
        fclose(arquivo);
        remove("clientes_temp.txt");
        return 0;
    }

    while (fgets(linha, sizeof(linha), arquivo) != NULL) {

        if (sscanf(linha, "%49[^;];%149[^;];%d",
                   usuarioArquivo, senhaArquivo, &perfil) == 3) {

            if (strcmp(usuarioArquivo, usuario) == 0 && perfil == 3) {
                fprintf(tempUsuarios, "%s;%s;%d\n",
                        novoUsuario, senhaArquivo, perfil);
                encontrouUsuario = 1;
            } else {
                fprintf(tempUsuarios, "%s", linha);
            }

        } else {
            fprintf(tempUsuarios, "%s", linha);
        }
    }

    fclose(arquivo);

    if (fclose(tempUsuarios) != 0 || !encontrouUsuario) {
        remove("clientes_temp.txt");
        remove("usuarios_temp.txt");
        printf("\nLogin do cliente nao encontrado ou erro ao gravar.\n");
        return 0;
    }

    /* CRIAR COPIAS DE SEGURANCA */

    remove("clientes_backup.txt");
    remove("usuarios_backup.txt");

    if (rename("clientes.txt", "clientes_backup.txt") != 0) {
        remove("clientes_temp.txt");
        remove("usuarios_temp.txt");
        printf("\nErro ao proteger clientes.txt.\n");
        return 0;
    }

    if (rename("usuarios.txt", "usuarios_backup.txt") != 0) {
        rename("clientes_backup.txt", "clientes.txt");
        remove("clientes_temp.txt");
        remove("usuarios_temp.txt");
        printf("\nErro ao proteger usuarios.txt.\n");
        return 0;
    }

    /* SUBSTITUIR OS DOIS ARQUIVOS */

    if (rename("clientes_temp.txt", "clientes.txt") != 0) {
        rename("clientes_backup.txt", "clientes.txt");
        rename("usuarios_backup.txt", "usuarios.txt");
        remove("usuarios_temp.txt");
        printf("\nErro ao atualizar clientes.txt.\n");
        return 0;
    }

    if (rename("usuarios_temp.txt", "usuarios.txt") != 0) {
        remove("clientes.txt");
        rename("clientes_backup.txt", "clientes.txt");
        rename("usuarios_backup.txt", "usuarios.txt");
        printf("\nErro ao atualizar usuarios.txt.\n");
        return 0;
    }

    remove("clientes_backup.txt");
    remove("usuarios_backup.txt");

    /* ATUALIZAR O USUARIO NA SESSAO ATUAL */

    strcpy(usuario, novoUsuario);

    return 1;
}

/* =====================================
   AREA DO CLIENTE
   ===================================== */

void areaCliente(char usuario[]) {

    int opcao;
    int resultado;
    int valido;

    char novoValor[150];
    char senha[50];
    char confirmar[50];

    while (1) {

        cabecalho();
        tituloPagina("MINHA CONTA");

        printf("\nUsuario: %s\n", usuario);

        printf("\n1 - Ver meus dados cadastrais\n");
        printf("2 - Alterar nome completo\n");
        printf("3 - Alterar telefone\n");
        printf("4 - Alterar email\n");
        printf("5 - Alterar endereco\n");
        printf("6 - Alterar senha\n");
        printf("7 - Alterar nome de usuario\n");

        printf("\n0 - Voltar ao menu principal\n");
        printf("\nEscolha uma opcao: ");

        if (scanf("%d", &opcao) != 1) {
            limparTeclado();
            printf("\nOpcao invalida!\n");
            continue;
        }

        limparTeclado();

        if (opcao == 0) {
            return;
        }

        if (opcao < 1 || opcao > 7) {
            printf("\nOpcao invalida!\n");
            continue;
        }

        if (opcao == 1) {
            consultarDadosCliente(usuario);
            system("pause");
            continue;
        }

        resultado = 0;

        /* ALTERAR SENHA */

        if (opcao == 6) {

            valido = 0;

            while (!valido) {

                if (!lerTexto(senha, sizeof(senha),
                              "Digite a nova senha: ")) {
                    break;
                }

                valido = validarSenha(senha, usuario);
            }

            if (!valido) {
                continue;
            }

            if (!lerTexto(confirmar, sizeof(confirmar),
                          "Confirme a nova senha: ")) {
                continue;
            }

            if (strcmp(senha, confirmar) != 0) {
                printf("\nAs senhas nao conferem!\n");
                continue;
            }

            resultado = alterarSenhaCliente(usuario, senha);
        }

        /* ALTERAR NOME DE USUARIO */

        else if (opcao == 7) {

            if (!lerTexto(novoValor, sizeof(novoValor),
                          "Digite o novo nome de usuario: ")) {
                continue;
            }

            if (!campoValido(novoValor)) {
                continue;
            }

            if (strlen(novoValor) >= 50) {
                printf("\nO usuario deve ter no maximo 49 caracteres.\n");
                continue;
            }

            resultado = alterarUsuarioCliente(usuario, novoValor);
        }

        /* ALTERAR OUTROS DADOS */

        else {

            if (opcao == 2) {

                if (!lerTexto(novoValor, sizeof(novoValor),
                              "Digite o novo nome completo: ")) {
                    continue;
                }

                if (strlen(novoValor) >= 100 ||
                    !campoValido(novoValor)) {
                    printf("\nNome invalido!\n");
                    continue;
                }
            }

            else if (opcao == 3) {

                valido = 0;

                while (!valido) {

                    if (!lerTexto(novoValor, sizeof(novoValor),
                                  "Digite o novo telefone com DDD: ")) {
                        break;
                    }

                    if (!campoValido(novoValor)) {
                        break;
                    }

                    valido = validarTelefone(novoValor);
                }

                if (!valido) {
                    continue;
                }
            }

            else if (opcao == 4) {

                valido = 0;

                while (!valido) {

                    if (!lerTexto(novoValor, sizeof(novoValor),
                                  "Digite o novo email: ")) {
                        break;
                    }

                    if (!campoValido(novoValor)) {
                        break;
                    }

                    valido = validarEmail(novoValor);
                }

                if (!valido) {
                    continue;
                }
            }

            else if (opcao == 5) {

                if (!lerEnderecoSeparado(
                        novoValor,
                        sizeof(novoValor))) {
                    continue;
                }
            }

            resultado = atualizarDadosCliente(
                usuario,
                opcao,
                novoValor
            );
        }

        if (resultado) {

            printf("\nDados atualizados com sucesso!\n");

            if (opcao == 7) {
                printf("Seu novo nome de usuario: %s\n", usuario);
            }

        } else {
            printf("\nNao foi possivel atualizar os dados.\n");
        }

        system("pause");
    }
}


/* =====================================
   AREA DO BALCONISTA
   ===================================== */

void areaBalconista(char usuario[]) {

    int opcao;
    int resultado;
    int valido;

    char novoValor[150];
    char senha[50];
    char confirmar[50];

    while (1) {

        cabecalho();
        tituloPagina("MINHA CONTA - BALCONISTA");

        printf("\nUsuario: %s\n", usuario);

        printf("\n1 - Ver meus dados cadastrais\n");
        printf("2 - Alterar nome completo\n");
        printf("3 - Alterar telefone\n");
        printf("4 - Alterar email\n");
        printf("5 - Alterar endereco\n");
        printf("6 - Alterar senha\n");
        printf("7 - Alterar nome de usuario\n");

        printf("\n0 - Voltar ao menu principal\n");
        printf("\nEscolha uma opcao: ");

        if (scanf("%d", &opcao) != 1) {
            limparTeclado();
            printf("\nOpcao invalida!\n");
            continue;
        }

        limparTeclado();

        if (opcao == 0) {
            return;
        }

        if (opcao < 1 || opcao > 7) {
            printf("\nOpcao invalida!\n");
            continue;
        }

        if (opcao == 1) {
            consultarDadosBalconista(usuario);
            system("pause");
            continue;
        }

        resultado = 0;

        /* ALTERAR SENHA */

        if (opcao == 6) {

            valido = 0;

            while (!valido) {

                if (!lerTexto(senha, sizeof(senha),
                              "Digite a nova senha: ")) {
                    break;
                }

                valido = validarSenha(senha, usuario);
            }

            if (!valido) {
                continue;
            }

            if (!lerTexto(confirmar, sizeof(confirmar),
                          "Confirme a nova senha: ")) {
                continue;
            }

            if (strcmp(senha, confirmar) != 0) {
                printf("\nAs senhas nao conferem!\n");
                continue;
            }

            resultado = alterarSenhaBalconista(usuario, senha);
        }

        /* ALTERAR NOME DE USUARIO */

        else if (opcao == 7) {

            if (!lerTexto(novoValor, sizeof(novoValor),
                          "Digite o novo nome de usuario: ")) {
                continue;
            }

            if (!campoValido(novoValor)) {
                continue;
            }

            if (strlen(novoValor) >= 50) {
                printf("\nO usuario deve ter no maximo 49 caracteres.\n");
                continue;
            }

            resultado = alterarUsuarioBalconista(usuario, novoValor);
        }

        /* ALTERAR OUTROS DADOS */

        else {

            if (opcao == 2) {

                if (!lerTexto(novoValor, sizeof(novoValor),
                              "Digite o novo nome completo: ")) {
                    continue;
                }

                if (strlen(novoValor) >= 100 ||
                    !campoValido(novoValor)) {
                    printf("\nNome invalido!\n");
                    continue;
                }
            }

            else if (opcao == 3) {

                valido = 0;

                while (!valido) {

                    if (!lerTexto(novoValor, sizeof(novoValor),
                                  "Digite o novo telefone com DDD: ")) {
                        break;
                    }

                    if (!campoValido(novoValor)) {
                        break;
                    }

                    valido = validarTelefone(novoValor);
                }

                if (!valido) {
                    continue;
                }
            }

            else if (opcao == 4) {

                valido = 0;

                while (!valido) {

                    if (!lerTexto(novoValor, sizeof(novoValor),
                                  "Digite o novo email: ")) {
                        break;
                    }

                    if (!campoValido(novoValor)) {
                        break;
                    }

                    valido = validarEmail(novoValor);
                }

                if (!valido) {
                    continue;
                }
            }

            else if (opcao == 5) {

                if (!lerEnderecoSeparado(
                        novoValor,
                        sizeof(novoValor))) {
                    continue;
                }
            }

            resultado = atualizarDadosBalconista(
                usuario,
                opcao,
                novoValor
            );
        }

        if (resultado) {

            printf("\nDados atualizados com sucesso!\n");

            if (opcao == 7) {
                printf("Seu novo nome de usuario: %s\n", usuario);
            }

        } else {
            printf("\nNao foi possivel atualizar os dados.\n");
        }

        system("pause");
    }
}


/* =====================================
   CRIAR ARQUIVO SEM O USUARIO INFORMADO
   ===================================== */

int criarArquivoSemUsuario(
    char arquivoOriginal[],
    char arquivoTemporario[],
    char usuarioRemover[],
    int perfilAlvo
) {
    FILE *arquivo;
    FILE *temporario;

    char linha[600];
    char usuarioArquivo[50];
    char senhaArquivo[50];

    int perfil;
    int encontrado = 0;

    arquivo = fopen(arquivoOriginal, "r");

    if (arquivo == NULL) {
        printf("\nErro ao abrir %s!\n", arquivoOriginal);
        return 0;
    }

    temporario = fopen(arquivoTemporario, "w");

    if (temporario == NULL) {
        fclose(arquivo);
        printf("\nErro ao criar arquivo temporario!\n");
        return 0;
    }

    while (fgets(linha, sizeof(linha), arquivo) != NULL) {

        int remover = 0;

        if (perfilAlvo == 0) {

            if (sscanf(linha, "%49[^;]", usuarioArquivo) == 1 &&
                strcmp(usuarioArquivo, usuarioRemover) == 0) {
                remover = 1;
            }

        } else {

            if (sscanf(linha, "%49[^;];%49[^;];%d",
                       usuarioArquivo, senhaArquivo, &perfil) == 3) {

                if (strcmp(usuarioArquivo, usuarioRemover) == 0 &&
                    perfil == perfilAlvo) {
                    remover = 1;
                }
            }
        }

        if (remover) {
            encontrado = 1;
        } else {
            fprintf(temporario, "%s", linha);
        }
    }

    fclose(arquivo);

    if (fclose(temporario) != 0) {
        remove(arquivoTemporario);
        return 0;
    }

    return encontrado;
}


/* =====================================
   GERENTE - REMOVER CONTA
   Remove somente balconistas e clientes
   ===================================== */

void removerConta(void) {

    FILE *arquivo;

    char usuario[50];
    char usuarioArquivo[50];
    char senhaArquivo[50];
    char linha[200];

    char arquivoDados[50];
    char arquivoDadosTemp[50];
    char arquivoDadosBackup[50];

    int perfil;
    int perfilEncontrado = 0;
    int encontrouUsuario = 0;
    int encontrouDados = 0;

    char confirmacao;

    printf("\n====================================\n");
    printf("          REMOVER CONTA\n");
    printf("====================================\n");

    printf("\n1 - Remover balconista\n");
    printf("2 - Remover cliente\n");
    printf("0 - Voltar\n");

    printf("\nEscolha o tipo de conta: ");

    if (scanf("%d", &perfil) != 1) {
        limparTeclado();
        printf("\nOpcao invalida!\n");
        return;
    }

    limparTeclado();

    if (perfil == 0) {
        return;
    }

    if (perfil == 1) {

        perfilEncontrado = 2;

        strcpy(arquivoDados, "balconistas.txt");
        strcpy(arquivoDadosTemp, "balconistas_temp.txt");
        strcpy(arquivoDadosBackup, "balconistas_backup.txt");

    } else if (perfil == 2) {

        perfilEncontrado = 3;

        strcpy(arquivoDados, "clientes.txt");
        strcpy(arquivoDadosTemp, "clientes_temp.txt");
        strcpy(arquivoDadosBackup, "clientes_backup.txt");

    } else {

        printf("\nOpcao invalida!\n");
        return;
    }

    printf("\nDigite 0 para cancelar.\n");

    if (!lerTexto(usuario, sizeof(usuario),
                  "\nDigite o nome de usuario da conta: ")) {
        return;
    }

    if (!campoValido(usuario)) {
        return;
    }

    /* VERIFICAR SE A CONTA EXISTE */

    arquivo = fopen("usuarios.txt", "r");

    if (arquivo == NULL) {
        printf("\nErro ao abrir usuarios.txt!\n");
        return;
    }

    while (fgets(linha, sizeof(linha), arquivo) != NULL) {

        if (sscanf(linha, "%49[^;];%49[^;];%d",
                   usuarioArquivo, senhaArquivo, &perfil) == 3) {

            if (strcmp(usuario, usuarioArquivo) == 0 &&
                perfil == perfilEncontrado) {

                encontrouUsuario = 1;
                break;
            }
        }
    }

    fclose(arquivo);

    if (!encontrouUsuario) {
        printf("\nConta nao encontrada nesse tipo de cadastro.\n");
        return;
    }

    printf("\nTem certeza que deseja remover a conta '%s'?\n", usuario);
    printf("Digite S para confirmar ou N para cancelar: ");

    if (scanf(" %c", &confirmacao) != 1) {
        limparTeclado();
        return;
    }

    limparTeclado();

    if (confirmacao != 'S' && confirmacao != 's') {
        printf("\nRemocao cancelada.\n");
        return;
    }

    /* PREPARAR ARQUIVO DE USUARIOS */

    encontrouUsuario = criarArquivoSemUsuario(
        "usuarios.txt",
        "usuarios_temp.txt",
        usuario,
        perfilEncontrado
    );

    /* PREPARAR ARQUIVO DE DADOS */

    encontrouDados = criarArquivoSemUsuario(
        arquivoDados,
        arquivoDadosTemp,
        usuario,
        0
    );

    if (!encontrouUsuario || !encontrouDados) {

        remove("usuarios_temp.txt");
        remove(arquivoDadosTemp);

        printf("\nNao foi possivel localizar todos os dados da conta.\n");
        printf("Nenhuma conta foi removida.\n");

        return;
    }

    /* CRIAR COPIAS DE SEGURANCA */

    remove("usuarios_backup.txt");
    remove(arquivoDadosBackup);

    if (rename("usuarios.txt", "usuarios_backup.txt") != 0) {

        remove("usuarios_temp.txt");
        remove(arquivoDadosTemp);

        printf("\nErro ao proteger usuarios.txt!\n");
        return;
    }

    if (rename(arquivoDados, arquivoDadosBackup) != 0) {

        rename("usuarios_backup.txt", "usuarios.txt");

        remove("usuarios_temp.txt");
        remove(arquivoDadosTemp);

        printf("\nErro ao proteger os dados da conta!\n");
        return;
    }

    /* SUBSTITUIR OS ARQUIVOS */

    if (rename("usuarios_temp.txt", "usuarios.txt") != 0) {

        rename("usuarios_backup.txt", "usuarios.txt");
        rename(arquivoDadosBackup, arquivoDados);

        remove(arquivoDadosTemp);

        printf("\nErro ao atualizar usuarios.txt!\n");
        return;
    }

    if (rename(arquivoDadosTemp, arquivoDados) != 0) {

        remove("usuarios.txt");

        rename("usuarios_backup.txt", "usuarios.txt");
        rename(arquivoDadosBackup, arquivoDados);

        printf("\nErro ao atualizar os dados da conta!\n");
        return;
    }

    /* REMOVER BACKUPS */

    remove("usuarios_backup.txt");
    remove(arquivoDadosBackup);

    printf("\n====================================\n");
    printf("     CONTA REMOVIDA COM SUCESSO!\n");
    printf("====================================\n");

    printf("\nUsuario removido: %s\n", usuario);

}

