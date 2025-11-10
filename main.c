#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

struct Usuario {
    char nome[50];
    char senha[50];
    char endereco[100];
    char pagamento[50];
};

struct Carrinho {
    char itens[50][100];
    float precos[50];
    int total_itens;
};

void limpar_terminal() { system("cls"); }
void salvar_usuario(struct Usuario *u);
void buscar_restaurantes(struct Usuario *u, struct Carrinho *c);
void mostrar_todos();
void formas_de_pagamento(struct Usuario *u);
void meus_enderecos(struct Usuario *u);
void logo();
void ver_avaliacoes(struct Usuario *u);
void fazer_pedido(struct Usuario *u, struct Carrinho *c);
int contem_texto(char *linha, char *busca);

int main() {
    SetConsoleOutputCP(65001);
    int opcao;
    struct Usuario usuario_logado;
    struct Carrinho carrinho;
    int logado = 0;
    carrinho.total_itens = 0;

    strcpy(usuario_logado.nome, "");
    strcpy(usuario_logado.senha, "");
    strcpy(usuario_logado.endereco, "Nenhum");
    strcpy(usuario_logado.pagamento, "Nenhum");

    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    while (1) {
        limpar_terminal();

        if (logado) {
            printf("\n=== MENU FEIFOOD ===\n");
            printf("bem-vindo, %s!\n", usuario_logado.nome);
            printf("1 - adicionar ao carrinho\n");
            printf("2 - ver carrinho\n");
            printf("3 - fazer pedido\n");
            printf("4 - mostrar todos os restaurantes\n");
            printf("5 - formas de pagamento\n");
            printf("6 - meus enderecos\n");
            printf("7 - minhas avaliacoes\n");
            printf("8 - logout\n");
            printf("escolha uma opcao: ");

            if (scanf("%d", &opcao) != 1) {
                while(getchar() != '\n');
                continue;
            }
            while(getchar() != '\n');

            switch (opcao) {
                case 1:
                    limpar_terminal();
                    buscar_restaurantes(&usuario_logado, &carrinho);
                    break;
                case 2: {
                    limpar_terminal();
                    printf("=== CARRINHO ===\n");
                    if (carrinho.total_itens == 0)
                        printf("seu carrinho esta vazio.\n");
                    else {
                        float subtotal = 0;
                        for (int i = 0; i < carrinho.total_itens; i++) {
                            printf("%d - %s (R$ %.2f)\n", i + 1, carrinho.itens[i], carrinho.precos[i]);
                            subtotal += carrinho.precos[i];
                        }
                        printf("\nsubtotal: R$ %.2f\n", subtotal);

                        int rem;
                        printf("\n0 - voltar\n");
                        printf("digite o numero pra remover ou 0 pra sair: ");
                        scanf("%d", &rem);
                        while(getchar() != '\n');
                        if (rem > 0 && rem <= carrinho.total_itens) {
                            for (int i = rem - 1; i < carrinho.total_itens - 1; i++) {
                                strcpy(carrinho.itens[i], carrinho.itens[i + 1]);
                                carrinho.precos[i] = carrinho.precos[i + 1];
                            }
                            carrinho.total_itens--;
                            printf("item removido!\n");
                            Sleep(1500);
                        }
                    }
                    printf("\npressione enter pra voltar...");
                    getchar();
                    break;
                }
                case 3:
                    limpar_terminal();
                    fazer_pedido(&usuario_logado, &carrinho);
                    break;
                case 4:
                    limpar_terminal();
                    mostrar_todos();
                    break;
                case 5:
                    limpar_terminal();
                    formas_de_pagamento(&usuario_logado);
                    break;
                case 6:
                    limpar_terminal();
                    meus_enderecos(&usuario_logado);
                    break;
                case 7:
                    limpar_terminal();
                    ver_avaliacoes(&usuario_logado);
                    break;
                case 8:
					logado = 0;
					carrinho.total_itens = 0;
					for (int i = 0; i < 50; i++) {
					    strcpy(carrinho.itens[i], "");
					    carrinho.precos[i] = 0.0;
					}
					    printf("até mais\n");
					    Sleep(1500);
					    break;

                default:
                    break;
            }
            continue;
        }

        limpar_terminal();
        SetConsoleTextAttribute(hConsole, 12);
        logo();
        SetConsoleTextAttribute(hConsole, 7);
        printf("\n\nbem-vindo ao feifood!\n\n");
        printf("1 - cadastro\n");
        printf("2 - login\n");
        printf("3 - sair\n");
        printf("digite sua escolha: ");

        if (scanf("%d", &opcao) != 1) {
            while(getchar() != '\n');
            continue;
        }
        while(getchar() != '\n');

        if (opcao == 1) {
            limpar_terminal();
            struct Usuario novo;
            FILE *f = fopen("usuarios.txt", "a");

            printf("nome de usuario: ");
            scanf(" %[^\n]", novo.nome);
            printf("senha: ");
            scanf(" %[^\n]", novo.senha);

            strcpy(novo.endereco, "Nenhum");
            strcpy(novo.pagamento, "Nenhum");

            fprintf(f, "%s %s %s %s\n", novo.nome, novo.senha, novo.endereco, novo.pagamento);
            fclose(f);
            printf("usuario cadastrado com sucesso!\n");
            Sleep(2000);
        } 
        else if (opcao == 2) {
            limpar_terminal();
            char nome[50], senha[50];
            struct Usuario temp;
            int encontrado = 0;
            FILE *f = fopen("usuarios.txt", "r");

            if (!f) {
                printf("nenhum usuario cadastrado ainda.\n");
                Sleep(2000);
                continue;
            }

            printf("usuario: ");
            scanf(" %[^\n]", nome);
            printf("senha: ");
            scanf(" %[^\n]", senha);

            while (fscanf(f, "%s %s %s %s", temp.nome, temp.senha, temp.endereco, temp.pagamento) != EOF) {
                if (strcmp(nome, temp.nome) == 0 && strcmp(senha, temp.senha) == 0) {
                    usuario_logado = temp;
                    encontrado = 1;
                    break;
                }
            }
            fclose(f);

            if (encontrado) {
                printf("login realizado com sucesso!\n");
                Sleep(1500);
                logado = 1;
            } else {
                printf("usuario ou senha incorretos!\n");
                Sleep(1500);
            }
        } 
        else if (opcao == 3) {
            exit(0);
        }
    }
    return 0;
}

void logo() {
    printf("  .-.                      .-.                            ___  \n");
    printf(" /    \\             .-.   /    \\                         (   ) \n");
    printf(" | .`. ;    .--.   ( __)  | .`. ;    .--.     .--.     .-.| |  \n");
    printf(" | |(___)  /    \\  (''\")  | |(___)  /    \\   /    \\   /   \\ |  \n");
    printf(" | |_     |  .-. ;  | |   | |_     |  .-. ; |  .-. ; |  .-. |  \n");
    printf("(   __)   |  | | |  | |  (   __)   | |  | | | |  | | | |  | |  \n");
    printf(" | |      |  |/  |  | |   | |      | |  | | | |  | | | |  | |  \n");
    printf(" | |      |  ' _.'  | |   | |      | |  | | | |  | | | |  | |  \n");
    printf(" | |      |  .'.-.  | |   | |      | '  | | | '  | | | '  | |  \n");
    printf(" | |      '  `-' /  | |   | |      '  `-' / '  `-' / ' `-'  /  \n");
    printf("(___)      `.__.'  (___) (___)      `.__.'   `.__.'   `.__,'   \n");
}


int contem_texto(char *linha, char *busca) {
    int i, j, lenL = strlen(linha), lenB = strlen(busca);
    for (i = 0; i <= lenL - lenB; i++) {
        for (j = 0; j < lenB; j++) {
            if (linha[i + j] != busca[j])
                break;
        }
        if (j == lenB)
            return 1; 
    }
    return 0;
}

void buscar_restaurantes(struct Usuario *u, struct Carrinho *c) {
    FILE *f = fopen("restaurantes.txt", "r");

    char busca[50];
    printf("digite o nome do restaurante ou comida: ");
    scanf(" %[^\n]", busca);

    char linha[150];
    char resultados[50][100];
    float precos[50];
    int total = 0;

    while (fgets(linha, sizeof(linha), f) != NULL) {
        if (contem_texto(linha, busca)) {
            char nome[50], comida[50];
            float preco;
            sscanf(linha, "%[^:]: %[^:]: %f", nome, comida, &preco);
            printf("%d - %s - %s (R$ %.2f)\n", total + 1, nome, comida, preco);
            sprintf(resultados[total], "%s - %s", nome, comida);
            precos[total] = preco;
            total++;
        }
    }
    fclose(f);

    if (total == 0) {
        printf("nenhum resultado encontrado.\n");
        printf("pressione enter pra voltar...");
        getchar();
        return;
    }

    int escolha;
    printf("\nescolha (1-%d) ou 0 pra voltar: ", total);
    scanf("%d", &escolha);
    while(getchar() != '\n');

    if (escolha <= 0 || escolha > total) return;

    strcpy(c->itens[c->total_itens], resultados[escolha - 1]);
    c->precos[c->total_itens] = precos[escolha - 1];
    c->total_itens++;

    printf("adicionado ao carrinho!\n");
    Sleep(1500);
}

void mostrar_todos() {
    FILE *f = fopen("restaurantes.txt", "r");

	char linha[100];
	while (fgets(linha, sizeof(linha), f) != NULL) {
    	printf("- %s", linha);
	}		
    fclose(f);
    printf("\npressione enter pra voltar...");
    getchar();
}

void formas_de_pagamento(struct Usuario *u) {
    int opcao;
    printf("\n--- formas de pagamento ---\n");
    printf("forma atual: %s\n", u->pagamento);
    printf("1 - credito\n2 - debito\n3 - pix\n4 - dinheiro\n");
    printf("escolha: ");
    scanf("%d", &opcao);
    while(getchar() != '\n');

    switch (opcao) {
        case 1: strcpy(u->pagamento, "Credito"); break;
        case 2: strcpy(u->pagamento, "Debito"); break;
        case 3: strcpy(u->pagamento, "Pix"); break;
        case 4: strcpy(u->pagamento, "Dinheiro"); break;
        default: return;
    }

    salvar_usuario(u);
    printf("forma atualizada: %s\n", u->pagamento);
    printf("pressione enter pra voltar...");
    getchar();
}

void meus_enderecos(struct Usuario *u) {
    char novo_endereco[100];
    printf("endereco atual: %s\n", u->endereco);
    printf("digite novo (sem espacos, use _): ");
    scanf("%s", novo_endereco);
    if (strlen(novo_endereco) == 0) return;
    strcpy(u->endereco, novo_endereco);
    salvar_usuario(u);
    printf("endereco atualizado pra: %s\n", u->endereco);
    printf("pressione enter pra voltar...");
    getchar();
}

void salvar_usuario(struct Usuario *u) {
    FILE *f = fopen("usuarios.txt", "r");
    if (!f) return;

    struct Usuario usuarios[100];
    int total = 0;
    while (fscanf(f, "%s %s %s %s", usuarios[total].nome, usuarios[total].senha, usuarios[total].endereco, usuarios[total].pagamento) != EOF)
        total++;
    fclose(f);

    for (int i = 0; i < total; i++)
        if (strcmp(usuarios[i].nome, u->nome) == 0)
            usuarios[i] = *u;

    f = fopen("usuarios.txt", "w");
    for (int i = 0; i < total; i++)
        fprintf(f, "%s %s %s %s\n", usuarios[i].nome, usuarios[i].senha, usuarios[i].endereco, usuarios[i].pagamento);
    fclose(f);
}

void fazer_pedido(struct Usuario *u, struct Carrinho *c) {
    if (c->total_itens == 0) {
        printf("seu carrinho esta vazio.\n");
        printf("pressione enter pra voltar...");
        getchar();
        return;
    }

    if (strcmp(u->endereco, "Nenhum") == 0) {
        printf("voce precisa cadastrar um endereco!\n");
        Sleep(2000);
        return;
    }

    if (strcmp(u->pagamento, "Nenhum") == 0) {
        printf("voce precisa cadastrar uma forma de pagamento!\n");
        Sleep(2000);
        return;
    }

    printf("=== PEDIDO ===\n");
    float total = 0;
    for (int i = 0; i < c->total_itens; i++) {
        printf("%d - %s (R$ %.2f)\n", i + 1, c->itens[i], c->precos[i]);
        total += c->precos[i];
    }
    printf("\ntotal: R$ %.2f\n", total);

    printf("confirmar pedido? (s/n): ");
    char r;
    scanf(" %c", &r);
    while(getchar() != '\n');

    if (r != 's' && r != 'S') return;

    printf("\npedido sendo preparado...\n");
    Sleep(3000);
    printf("motoboy a caminho!\n");
    Sleep(3000);
    printf("pedido entregue!\n");

    int estrelas;
    char comentario[100];
    printf("\navalie seu pedido (1 a 5): ");
    scanf("%d", &estrelas);
    while(getchar() != '\n');
    if (estrelas < 1) estrelas = 1;
    if (estrelas > 5) estrelas = 5;

    printf("comentario: ");
    scanf(" %[^\n]", comentario);

    FILE *f = fopen("avaliacoes.txt", "a");
    fprintf(f, "%s %d %s\n", u->nome, estrelas, comentario);
    fclose(f);

    printf("avaliacao salva, obrigado!\n");
    Sleep(1500);

    c->total_itens = 0;
}

void ver_avaliacoes(struct Usuario *u) {
    FILE *f = fopen("avaliacoes.txt", "r");
    if (!f) {
        printf("nenhuma avaliacao encontrada.\n");
        printf("pressione enter pra voltar...");
        getchar();
        return;
    }

    char nome[50];
    int estrelas;
    char comentario[100];
    int achou = 0;

    while (fscanf(f, "%s %d %[^\n]", nome, &estrelas, comentario) != EOF) {
        if (strcmp(nome, u->nome) == 0) {
            achou = 1;
            for (int i = 0; i < estrelas; i++) printf("★");
            for (int i = estrelas; i < 5; i++) printf("☆");
            printf(" - %s\n", comentario);
        }
    }
    fclose(f);

    if (!achou) printf("voce ainda nao avaliou nada.\n");
    printf("\npressione enter pra voltar...");
    getchar();
}
