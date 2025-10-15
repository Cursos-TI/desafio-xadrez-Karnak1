#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ------------------------ Structs ------------------------

typedef struct Sala {
    char nome[50];
    char pista[100];
    struct Sala* esquerda;
    struct Sala* direita;
} Sala;

typedef struct PistaNode {
    char pista[100];
    struct PistaNode* esquerda;
    struct PistaNode* direita;
} PistaNode;

// ------------------------ Funções de criação ------------------------

/**
 * Cria uma sala com nome e pista (opcional).
 */
Sala* criarSala(const char* nome, const char* pista) {
    Sala* novaSala = (Sala*) malloc(sizeof(Sala));
    strcpy(novaSala->nome, nome);
    if (pista)
        strcpy(novaSala->pista, pista);
    else
        novaSala->pista[0] = '\0';
    novaSala->esquerda = NULL;
    novaSala->direita = NULL;
    return novaSala;
}

/**
 * Cria um novo nó de pista na árvore de pistas.
 */
PistaNode* inserirPista(PistaNode* raiz, const char* pista) {
    if (raiz == NULL) {
        PistaNode* nova = (PistaNode*) malloc(sizeof(PistaNode));
        strcpy(nova->pista, pista);
        nova->esquerda = nova->direita = NULL;
        return nova;
    }
    if (strcmp(pista, raiz->pista) < 0)
        raiz->esquerda = inserirPista(raiz->esquerda, pista);
    else if (strcmp(pista, raiz->pista) > 0)
        raiz->direita = inserirPista(raiz->direita, pista);
    return raiz;
}

// ------------------------ Funções de exploração ------------------------

/**
 * Permite ao jogador explorar a mansão e coleta as pistas.
 */
void explorarSalasComPistas(Sala* salaAtual, PistaNode** arvorePistas) {
    char escolha;
    while (salaAtual != NULL) {
        printf("\nVocê está no cômodo: %s\n", salaAtual->nome);

        if (strlen(salaAtual->pista) > 0) {
            printf("Você encontrou uma pista: %s\n", salaAtual->pista);
            *arvorePistas = inserirPista(*arvorePistas, salaAtual->pista);
        } else {
            printf("Não há nenhuma pista neste cômodo.\n");
        }

        printf("Escolha o caminho - esquerda (e), direita (d), sair (s): ");
        scanf(" %c", &escolha);

        if (escolha == 'e')
            salaAtual = salaAtual->esquerda;
        else if (escolha == 'd')
            salaAtual = salaAtual->direita;
        else if (escolha == 's')
            break;
        else
            printf("Opção inválida! Tente novamente.\n");
    }
}

/**
 * Exibe todas as pistas coletadas em ordem alfabética (ordem infixa).
 */
void exibirPistas(PistaNode* raiz) {
    if (raiz != NULL) {
        exibirPistas(raiz->esquerda);
        printf("- %s\n", raiz->pista);
        exibirPistas(raiz->direita);
    }
}

// ------------------------ Função principal (main) ------------------------

int main() {
    // Criando a mansão (árvore binária fixa)
    Sala* hallEntrada = criarSala("Hall de Entrada", "Pegadas misteriosas");
    Sala* salaEstar   = criarSala("Sala de Estar", "Um copo quebrado");
    Sala* cozinha     = criarSala("Cozinha", "");
    Sala* biblioteca  = criarSala("Biblioteca", "Um livro fora do lugar");
    Sala* jardim      = criarSala("Jardim", "Pegadas na lama");

    // Conectando as salas
    hallEntrada->esquerda = salaEstar;
    hallEntrada->direita = cozinha;
    salaEstar->esquerda = biblioteca;
    salaEstar->direita = jardim;

    // Árvore de pistas coletadas (BST)
    PistaNode* arvorePistas = NULL;

    // Iniciar exploração
    printf("=== BEM-VINDO AO DETECTIVE QUEST ===\n");
    printf("Explore a mansão e colete pistas para resolver o mistério!\n");

    explorarSalasComPistas(hallEntrada, &arvorePistas);

    // Exibir pistas coletadas
    printf("\n=== PISTAS COLETADAS ===\n");
    if (arvorePistas)
        exibirPistas(arvorePistas);
    else
        printf("Nenhuma pista foi coletada.\n");

    return 0;
}
