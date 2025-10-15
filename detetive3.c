#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define HASH_SIZE 101

// ------------------ Structs ------------------

typedef struct Sala {
    char nome[50];
    struct Sala* esquerda;
    struct Sala* direita;
} Sala;

typedef struct Pista {
    char descricao[100];
    struct Pista* esquerda;
    struct Pista* direita;
} Pista;

typedef struct HashNode {
    char pista[100];
    char suspeito[50];
    struct HashNode* prox;
} HashNode;

HashNode* hashTable[HASH_SIZE]; // tabela hash global

// ------------------ Utilitários ------------------

/**
 * Função de hash para strings.
 */
unsigned int hash(char* str) {
    unsigned int h = 0;
    while (*str) h = (h << 5) + *str++;
    return h % HASH_SIZE;
}

// ------------------ Funções principais ------------------

/**
 * criaSala() – cria dinamicamente um cômodo.
 */
Sala* criarSala(char* nome) {
    Sala* nova = (Sala*) malloc(sizeof(Sala));
    strcpy(nova->nome, nome);
    nova->esquerda = nova->direita = NULL;
    return nova;
}

/**
 * obterPistaDaSala() – retorna a pista fixa associada ao nome da sala.
 */
char* obterPistaDaSala(char* nome) {
    if (strcmp(nome, "Biblioteca") == 0) return "Luvas ensanguentadas";
    if (strcmp(nome, "Cozinha") == 0) return "Faca desaparecida";
    if (strcmp(nome, "Sala de Estar") == 0) return "Relógio parado às 21h";
    if (strcmp(nome, "Quarto") == 0) return "Perfume masculino";
    if (strcmp(nome, "Porão") == 0) return "Pegadas lamacentas";
    return NULL;
}

/**
 * inserirPista() – insere a pista coletada na árvore de pistas (BST).
 */
Pista* inserirPista(Pista* raiz, char* descricao) {
    if (!raiz) {
        Pista* nova = (Pista*) malloc(sizeof(Pista));
        strcpy(nova->descricao, descricao);
        nova->esquerda = nova->direita = NULL;
        return nova;
    }

    if (strcmp(descricao, raiz->descricao) < 0)
        raiz->esquerda = inserirPista(raiz->esquerda, descricao);
    else if (strcmp(descricao, raiz->descricao) > 0)
        raiz->direita = inserirPista(raiz->direita, descricao);

    return raiz;
}

/**
 * inserirNaHash() – insere associação pista/suspeito na tabela hash.
 */
void inserirNaHash(char* pista, char* suspeito) {
    int idx = hash(pista);
    HashNode* novo = (HashNode*) malloc(sizeof(HashNode));
    strcpy(novo->pista, pista);
    strcpy(novo->suspeito, suspeito);
    novo->prox = hashTable[idx];
    hashTable[idx] = novo;
}

/**
 * encontrarSuspeito() – consulta o suspeito correspondente a uma pista.
 */
char* encontrarSuspeito(char* pista) {
    int idx = hash(pista);
    HashNode* atual = hashTable[idx];
    while (atual) {
        if (strcmp(atual->pista, pista) == 0)
            return atual->suspeito;
        atual = atual->prox;
    }
    return NULL;
}

/**
 * explorarSalas() – navega pela árvore e ativa o sistema de pistas.
 */
void explorarSalas(Sala* atual, Pista** arvorePistas) {
    if (!atual) return;

    printf("\nVocê está na sala: %s\n", atual->nome);
    char* pista = obterPistaDaSala(atual->nome);

    if (pista) {
        printf("🔎 Você encontrou a pista: %s\n", pista);
        *arvorePistas = inserirPista(*arvorePistas, pista);
    } else {
        printf("Nenhuma pista encontrada nesta sala.\n");
    }

    char escolha;
    printf("Deseja ir para a (e)squerda, (d)ireita ou (s)air da exploração? ");
    scanf(" %c", &escolha);

    if (escolha == 'e') explorarSalas(atual->esquerda, arvorePistas);
    else if (escolha == 'd') explorarSalas(atual->direita, arvorePistas);
    else if (escolha == 's') printf("🔚 Exploração encerrada.\n");
    else {
        printf("Opção inválida. Encerrando.\n");
    }
}

/**
 * Lista as pistas coletadas em ordem (BST in-order traversal).
 */
void listarPistas(Pista* raiz) {
    if (!raiz) return;
    listarPistas(raiz->esquerda);
    printf("- %s\n", raiz->descricao);
    listarPistas(raiz->direita);
}

/**
 * Conta quantas pistas apontam para determinado suspeito.
 */
int contarPistas(Pista* raiz, char* suspeito) {
    if (!raiz) return 0;

    int cont = 0;
    char* suspeitoPista = encontrarSuspeito(raiz->descricao);
    if (suspeitoPista && strcmp(suspeitoPista, suspeito) == 0)
        cont++;

    return cont + contarPistas(raiz->esquerda, suspeito) + contarPistas(raiz->direita, suspeito);
}

/**
 * verificarSuspeitoFinal() – conduz à fase de julgamento final.
 */
void verificarSuspeitoFinal(Pista* pistas) {
    char suspeito[50];
    printf("\nDigite o nome do suspeito que deseja acusar: ");
    scanf(" %[^\n]", suspeito);

    int count = contarPistas(pistas, suspeito);

    if (count >= 2)
        printf("\n✅ Acusação aceita! %s foi considerado CULPADO com %d pistas.\n", suspeito, count);
    else
        printf("\n❌ Acusação fraca. %s foi considerado INOCENTE (apenas %d pista(s)).\n", suspeito, count);
}

// ------------------ Programa Principal ------------------

int main() {
    // Monta o mapa fixo da mansão
    Sala* raiz = criarSala("Sala de Estar");
    raiz->esquerda = criarSala("Biblioteca");
    raiz->direita = criarSala("Cozinha");
    raiz->esquerda->esquerda = criarSala("Quarto");
    raiz->direita->direita = criarSala("Porão");

    // Associações pista/suspeito
    inserirNaHash("Luvas ensanguentadas", "Carlos");
    inserirNaHash("Faca desaparecida", "Ana");
    inserirNaHash("Relógio parado às 21h", "Carlos");
    inserirNaHash("Perfume masculino", "Bruno");
    inserirNaHash("Pegadas lamacentas", "Ana");

    // Inicializa árvore de pistas
    Pista* arvorePistas = NULL;

    printf("🕵️‍♂️ Bem-vindo ao Detective Quest!\nDescubra quem é o culpado coletando pistas pela mansão.\n");

    // Começa exploração
    explorarSalas(raiz, &arvorePistas);

    // Lista pistas coletadas
    printf("\n📜 Pistas coletadas:\n");
    listarPistas(arvorePistas);

    // Julgamento final
    verificarSuspeitoFinal(arvorePistas);

    printf("\nObrigado por jogar Detective Quest!\n");

    return 0;
}
