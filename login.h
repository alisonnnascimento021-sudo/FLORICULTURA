
#ifndef LOGIN_H
#define LOGIN_H

/* =====================================
   LOGIN E CADASTRO
   ===================================== */

int cadastroELogin(
    char usuarioLogado[],
    int tamanho,
    int *perfilLogado
);

void cadastrarBalconista(void);
void cadastrarCliente(void);

/* =====================================
   RECUPERACAO DE SENHA
   ===================================== */

int recuperarSenha(void);
void cadastrarRecuperacaoGerente(char usuario[]);

/* =====================================
   AREAS DOS USUARIOS
   ===================================== */

void areaCliente(char usuario[]);
void areaBalconista(char usuario[]);

/* =====================================
   ALTERACAO DE USUARIO
   ===================================== */

int alterarUsuarioCliente(
    char usuario[],
    char novoUsuario[]
);

int alterarUsuarioBalconista(
    char usuario[],
    char novoUsuario[]
);

/* =====================================
   ATUALIZAR DADOS CADASTRAIS
   ===================================== */

int atualizarDadosCliente(
    char usuario[],
    int opcao,
    char novoValor[]
);

int atualizarDadosBalconista(
    char usuario[],
    int opcao,
    char novoValor[]
);

/* =====================================
   ALTERACAO DE SENHA
   ===================================== */

int alterarSenhaCliente(
    char usuario[],
    char novaSenha[]
);

int alterarSenhaBalconista(
    char usuario[],
    char novaSenha[]
);

/* =====================================
   VALIDACOES
   ===================================== */

int validarSenha(char senha[], char nome[]);
int validarCPF(char cpf[]);
int validarTelefone(char telefone[]);
int validarEmail(char email[]);
int validarEndereco(char endereco[]);

/* =====================================
   ADMINISTRACAO
   ===================================== */

void removerConta(void);

/* =====================================
   FUNCOES AUXILIARES
   ===================================== */

void limparTeclado(void);

#endif

