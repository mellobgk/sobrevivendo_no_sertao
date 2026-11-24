/* ==================================================
 *   SOBREVIVENDO NO SERTAO
 *   Aguente 10 dias no sertao nordestino cuidando
 *   da sua agua, comida, energia e dinheiro!
 * ================================================== */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define DIAS_PARA_VENCER 10
#define ARQUIVO_SAVE "save_sertao.txt"

/* Resultados possiveis de uma partida */
#define FIM_ENTRADA   0   /* a entrada acabou (EOF) */
#define FIM_PARTIDA   1   /* ganhou ou perdeu */
#define FIM_SALVOU    2   /* saiu e salvou */

/* Estrutura que guarda os recursos do jogador */
typedef struct {
    int agua;
    int comida;
    int energia;
    int dinheiro;
} Jogador;

/* Sorteia um numero entre min e max (incluindo os dois) */
int sortear(int min, int max) {
    return min + rand() % (max - min + 1);
}

/* Le uma linha digitada pelo usuario e remove o "Enter" do final.
 * Se o usuario digitar mais do que cabe, descarta o resto da linha
 * para nao "sobrar" texto para a proxima leitura.
 * Retorna 1 se leu algo, 0 se a entrada acabou (EOF). */
int ler_linha(char *texto, int tamanho) {
    if (fgets(texto, tamanho, stdin) == NULL) {
        texto[0] = '\0';
        return 0;
    }

    if (strchr(texto, '\n') == NULL) {
        int c;
        while ((c = getchar()) != '\n' && c != EOF) {
            /* descarta o excesso */
        }
    }

    texto[strcspn(texto, "\n")] = '\0';
    return 1;
}

void linha(char simbolo) {
    int i;
    for (i = 0; i < 50; i++) {
        printf("%c", simbolo);
    }
    printf("\n");
}

void mostrar_titulo(void) {
    linha('=');
    printf("            SOBREVIVENDO NO SERTAO\n");
    linha('=');
    printf("Voce e um sertanejo e precisa aguentar\n");
    printf("%d dias de seca. Se a agua, a comida ou a\n", DIAS_PARA_VENCER);
    printf("energia chegarem a zero, voce perde!\n\n");
}

void mostrar_status(int dia, Jogador *j) {
    linha('-');
    printf("DIA %d de %d\n", dia, DIAS_PARA_VENCER);
    printf("Agua: %d   Comida: %d   Energia: %d   Dinheiro: R$%d\n",
           j->agua, j->comida, j->energia, j->dinheiro);
    linha('-');
}

/* Retorna a acao escolhida (1-6) ou 0 se a entrada acabou */
int escolher_acao(void) {
    char entrada[20];

    printf("O que voce vai fazer hoje?\n");
    printf("  1) Buscar agua no acude      (+agua, -energia)\n");
    printf("  2) Trabalhar na roca         (+comida, -energia)\n");
    printf("  3) Vender rapadura na feira  (+dinheiro, -energia)\n");
    printf("  4) Comprar mantimentos       (-R$10, +agua e +comida)\n");
    printf("  5) Descansar na rede         (+energia)\n");
    printf("  6) Sair e salvar o jogo\n");

    /* Fica pedindo ate o jogador digitar um numero valido */
    while (1) {
        printf("Escolha (1-6): ");
        if (!ler_linha(entrada, sizeof(entrada))) {
            return 0;   /* entrada acabou: evita loop infinito */
        }
        if (strlen(entrada) == 1 && entrada[0] >= '1' && entrada[0] <= '6') {
            return entrada[0] - '0';
        }
        printf("Oxe! Digite um numero de 1 a 6.\n");
    }
}

void realizar_acao(int escolha, Jogador *j) {
    int ganho;

    switch (escolha) {
        case 1:
            ganho = sortear(2, 4);
            j->agua += ganho;
            j->energia -= 2;
            printf("\nVoce andou ate o acude e trouxe %d de agua.\n", ganho);
            break;

        case 2:
            ganho = sortear(2, 4);
            j->comida += ganho;
            j->energia -= 2;
            printf("\nVoce colheu milho e feijao: +%d de comida.\n", ganho);
            break;

        case 3:
            ganho = sortear(5, 12);
            j->dinheiro += ganho;
            j->energia -= 1;
            printf("\nVoce vendeu rapadura na feira e ganhou R$%d!\n", ganho);
            break;

        case 4:
            if (j->dinheiro >= 10) {
                j->dinheiro -= 10;
                j->agua += 3;
                j->comida += 3;
                printf("\nVoce comprou mantimentos na bodega: +3 agua, +3 comida.\n");
            } else {
                printf("\nVixe! Nao tem dinheiro suficiente. Perdeu o dia!\n");
            }
            break;

        case 5:
            j->energia += 3;
            printf("\nVoce tirou um cochilo na rede e recuperou energia.\n");
            break;
    }
}

/* As vezes acontece algo inesperado no sertao */
void evento_aleatorio(Jogador *j) {
    int sorteio = sortear(1, 10);

    if (sorteio == 1) {
        printf("Choveu no sertao! Alegria geral! +4 de agua.\n");
        j->agua += 4;
    } else if (sorteio == 2) {
        printf("O sol ta de rachar! Voce perdeu 2 de agua.\n");
        j->agua -= 2;
    } else if (sorteio == 3) {
        printf("Um carcara atacou suas galinhas! -2 de comida.\n");
        j->comida -= 2;
    } else if (sorteio == 4) {
        printf("Teve forro na vila! Voce dancou e ficou animado: +2 energia.\n");
        j->energia += 2;
    } else if (sorteio == 5) {
        printf("Um compadre te deu um pouco de cuscuz! +2 de comida.\n");
        j->comida += 2;
    } else {
        printf("O dia passou tranquilo no sertao.\n");
    }
}

/* Todo dia o sertanejo gasta um pouco de agua e comida */
void gasto_diario(Jogador *j) {
    j->agua -= 2;
    j->comida -= 1;
    printf("(No fim do dia voce gastou 2 de agua e 1 de comida.)\n\n");
}

/* Retorna 1 se o jogador perdeu, 0 se ainda esta vivo */
int perdeu(Jogador *j) {
    if (j->agua <= 0) {
        printf("Sua agua acabou... A seca venceu desta vez.\n");
        return 1;
    }
    if (j->comida <= 0) {
        printf("Sua comida acabou... Nao deu pra aguentar.\n");
        return 1;
    }
    if (j->energia <= 0) {
        printf("Voce ficou sem energia e desmaiou de cansaco.\n");
        return 1;
    }
    return 0;
}

/* ---------- SALVAR E CARREGAR ---------- */

/* Grava nome, dia e recursos no arquivo. Retorna 1 se deu certo. */
int salvar_jogo(const char *nome, int dia, Jogador *j) {
    FILE *arq = fopen(ARQUIVO_SAVE, "w");
    if (arq == NULL) {
        return 0;
    }
    fprintf(arq, "%s\n", nome);
    fprintf(arq, "%d %d %d %d %d\n", dia, j->agua, j->comida, j->energia, j->dinheiro);
    fclose(arq);
    return 1;
}

/* Le o arquivo de save. Retorna 1 se encontrou um save valido. */
int carregar_jogo(char *nome, int tamanho, int *dia, Jogador *j) {
    FILE *arq = fopen(ARQUIVO_SAVE, "r");
    int ok;

    if (arq == NULL) {
        return 0;   /* nao tem jogo salvo */
    }
    ok = fgets(nome, tamanho, arq) != NULL;
    if (ok) {
        nome[strcspn(nome, "\n")] = '\0';
        ok = fscanf(arq, "%d %d %d %d %d", dia, &j->agua, &j->comida,
                    &j->energia, &j->dinheiro) == 5;
    }
    fclose(arq);

    /* confere se os dados fazem sentido (arquivo pode estar estragado) */
    if (!ok || *dia < 1 || *dia > DIAS_PARA_VENCER || nome[0] == '\0') {
        return 0;
    }
    return 1;
}

/* Apaga o save quando a partida termina, pra nao dar pra "voltar no tempo" */
void apagar_save(void) {
    remove(ARQUIVO_SAVE);
}

/* Retorna FIM_ENTRADA, FIM_PARTIDA ou FIM_SALVOU */
int jogar(void) {
    char nome[50];
    char resposta[10];
    int dia = 1, escolha;
    int continuou = 0;
    Jogador jogador = {6, 6, 6, 5};   /* agua, comida, energia, dinheiro */
    Jogador salvo;
    int dia_salvo;

    mostrar_titulo();

    /* Se existe um jogo salvo, pergunta se quer continuar */
    if (carregar_jogo(nome, sizeof(nome), &dia_salvo, &salvo)) {
        printf("Achei um jogo salvo de %s no dia %d.\n", nome, dia_salvo);
        printf("Quer continuar de onde parou? (s/n): ");
        if (!ler_linha(resposta, sizeof(resposta))) {
            return FIM_ENTRADA;
        }
        if (resposta[0] == 's' || resposta[0] == 'S') {
            jogador = salvo;
            dia = dia_salvo;
            continuou = 1;
            printf("\nBem-vindo de volta, %s! Bora continuar!\n\n", nome);
        } else {
            apagar_save();
            printf("\nTudo bem, vamos comecar do zero.\n\n");
        }
    }

    if (!continuou) {
        printf("Qual e o seu nome, cabra? ");
        if (!ler_linha(nome, sizeof(nome))) {
            return FIM_ENTRADA;
        }
        if (nome[0] == '\0') {
            strcpy(nome, "Sertanejo");   /* nome padrao se nao digitar nada */
        }
        printf("\nBoa sorte, %s! O sertao e duro, mas o sertanejo e forte!\n\n", nome);
    }

    for (; dia <= DIAS_PARA_VENCER; dia++) {
        mostrar_status(dia, &jogador);
        escolha = escolher_acao();
        if (escolha == 0) {
            return FIM_ENTRADA;
        }
        if (escolha == 6) {
            if (salvar_jogo(nome, dia, &jogador)) {
                printf("\nJogo salvo em \"%s\"! Volte quando quiser, %s.\n", ARQUIVO_SAVE, nome);
                return FIM_SALVOU;
            }
            printf("\nVixe! Nao consegui salvar o jogo. Escolha outra acao.\n\n");
            dia--;   /* repete o mesmo dia */
            continue;
        }
        realizar_acao(escolha, &jogador);
        evento_aleatorio(&jogador);
        gasto_diario(&jogador);

        if (perdeu(&jogador)) {
            /* o dia em que perdeu nao conta como dia aguentado */
            printf("Voce aguentou %d dia(s). Tente de novo, %s!\n", dia - 1, nome);
            apagar_save();
            return FIM_PARTIDA;
        }
    }

    linha('=');
    printf("ARRETADO, %s! Voce sobreviveu %d dias no sertao!\n", nome, DIAS_PARA_VENCER);
    printf("E pra comemorar... finalmente chegou a chuva!\n");
    linha('=');
    apagar_save();
    return FIM_PARTIDA;
}

int main(void) {
    char resposta[10];

    srand((unsigned int) time(NULL));   /* inicia os sorteios */

    /* Programa principal: permite jogar de novo */
    while (1) {
        int resultado = jogar();
        if (resultado != FIM_PARTIDA) {
            break;   /* salvou e saiu, ou a entrada acabou */
        }
        printf("\nQuer jogar de novo? (s/n): ");
        if (!ler_linha(resposta, sizeof(resposta)) ||
            (resposta[0] != 's' && resposta[0] != 'S')) {
            break;
        }
        printf("\n");
    }

    printf("\nValeu por jogar! Ate mais, visse?\n");
    return 0;
}