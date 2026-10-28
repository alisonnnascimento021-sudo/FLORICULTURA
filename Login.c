int cadastroELogin(char usuarioLogado[], int tamanho) {
	
	
char usuarioCadastrado[50];
char senhaCadastrada[50];
char usuarioDigitado[50];
char senhaDigitada[50];

int tentativas = 3;

printf("FLORICULTURA - CADASTRO \n");

printf("Crie seu usuario: ");
scanf("%49s", usuarioCadastrado);

printf("Crie sua senha: ");
scanf("%49s", senhaCadastrada);

printf("\nCadastro realizado com sucesso!\n");
printf("\n=== TELA DE LOGIN ===\n");

while (tentativas > 0) {
    printf("Usuario: ");
    scanf("%49s", usuarioDigitado);

    printf("Senha: ");
    scanf("%49s", senhaDigitada);

    if (strcmp(usuarioDigitado, usuarioCadastrado) == 0 &&
        strcmp(senhaDigitada, senhaCadastrada) == 0) {

        printf("\nLogin realizado com sucesso!\n");

        strncpy(usuarioLogado, usuarioCadastrado, tamanho - 1);
        usuarioLogado[tamanho - 1] = '\0';

        return 1;
    }

    tentativas--;

    printf("\nUsuario ou senha incorretos!\n");
    printf("Tentativas restantes: %d\n", tentativas);
}

printf("\nUsuario bloqueado. Tente novamente.\n");

return 0;

}

