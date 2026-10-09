#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* run this program using the console pauser or add your own getch, system("pause") or input loop */

int main(int argc, char *argv[]) {	
	//login
	char usuario[20];
	char senha[20];
	printf("===Floricultura===\n");
	
	printf("Digite seu usuario: ");
	scanf("%19s", &usuario);
	
	printf("digite sua senha: ");
	scanf("%19s", &senha);
	
	//verificando usuario e senha
	if (strcmp(usuario, "Login") ==  0 && strcmp(senha, "1234") == 0){
		printf("\nLogin realizado com sucesso! \n");
		printf("Bem vindo, %s\n", usuario);
	}	
	else{
		printf("\nUsuario incorreto ou senha incorreto!\n");

	return 0;
}

//==========
//carrinho
//==========

int opcao;
int quantidade;

float total = 0;

int rosa = 0;
int girassol = 0;
int tulipa = 0;
int Orquidea = 0;


char continuar;
//==========
//menu
//==========

do{

printf("\n================================\n");
printf("            MENU FLORICULTURA\n");
printf("\n=================================\n");

printf("\n1 Rosa    - R$25.00");
printf("\n2 Girassol   - R$20.00");
printf("\n3 Tulipa     - R$30.00");
printf("\n4 Orquidea  - R$45.00");
printf("\n5 - Sair");

printf("\n\n Escolha uma opcao: ");
scanf("%d", &opcao);

//escolha da flor
//================

switch(opcao){
	case 1:
		printf("\nVoce escolheu rosa!");
		printf("\nQuantas deseja comprar? ");
		scanf("%d", &quantidade);
		
		rosa = rosa + quantidade;
		printf("\nRosas adicionadas no carrinho!");
		break;
		
		case 2:
			printf("\nVoce escolheu Girassol!");
			printf("\nQuantas deseja comprar?");
			scanf("%d", &quantidade);
			
			girassol = girassol + quantidade;
			printf("\nGirassol adicionado no carrinho!");
			break;
			
			case 3:
				printf("\nVoce escolheu a tulipa!");
				printf("\nQuantas deseja comprar?");
				scanf("%d", &quantidade);
				
				tulipa = tulipa + quantidade;
				printf("\Tulipa adicionado no carrinho!");
				
			   break;
				
				case 4:
				printf("\nVoce escolheu a orquidea!");
				printf("\nQuantas deseja compras?");
				scanf("%d", &quantidade);
				
				Orquidea = Orquidea + quantidade;
				printf("\nOrquidea adicionado no carrinho");
				break;
				
				case 5:
					printf("\nFinalizando compra...");
				
				default:
				printf("\nOpcao invalida!\n ");
				continuar = 'n';
				break; 
}

// =====================
// CONTINUAR COMPRA
// =====================

if (opcao >= 1 && opcao <=4){
	printf("\n\nDeseja compra outra flor? (s/n): ");
	scanf(" %c", &continuar);
}
else{
	continuar = 'n';
}


}while (continuar == 's' || continuar == 'S');

//================
//calculando total
//================

total = (rosa * 25.00) +
(girassol * 20.00) +
(tulipa * 30.00) +
(Orquidea * 45.00);

//==================
//carrinho
//==================

printf("\n\n=======================\n");
printf("              SEU CARRINHO\n");
printf("===========================\n");

if (rosa > 0){
	
	printf("\nRosas: ");
	printf("\n%d x R$ 25.00 = R$ %.2f\n",
	rosa, rosa * 25.00);
}
if (girassol > 0){
	printf("\nGirassol: ");
	printf("\n%d x R$ 20.00 = R$ %.2f\n",
	girassol, girassol * 20.00);
}
if (tulipa >0){
	printf("\nTulipa: ");
	printf("\n%d x R$30.00 = R$.2f\n",
	tulipa, tulipa * 30.00);
}
if (Orquidea > 0){
	printf("\nOrquideas: ");
	printf("\n%d x R$ 45.00 = R$.2f\n",
	Orquidea, Orquidea * 45.00);
}

//================
//resumo compra
printf("\n===========================\n");
printf("               RESUMO DA COMPRA");
printf("\n===========================\n");

printf("\nCliente: %s", usuario);
printf("\nQuantidade: %d", 	quantidade);
printf("\nTotal de compra: %.2f", total);

printf("\n==================================\n");
printf("\nFloricultura agradece pela sua compra, Volte sempre!\n");

return 0;
}
