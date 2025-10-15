#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Estrutura de uma sala (nó da árvore)
typedef struct Sala {
    char nome[50];
    struct Sala *esquerda;
    struct Sala *direita;
} Sala;

/*
 * Função: criarSala
 * -----------------
 * Cria uma nova sala com nome fornecido e aloca dinamicamente.
 */
Sala* criarSala(const char* nome) {
    Sala* novaSala = (Sala*)malloc(sizeof(Sala));
    if (novaSala == NULL) {
        printf("Erro ao alocar memória para a sala.\n");
        exit(1);
    }
    strcpy(novaSala->nome, nome);
    novaSala->esquerda = NULL;
    novaSala->direita = NULL;
    return novaSala;
}

/*
 * Função: explorarSalas
 * ---------------------
 * Permite que o jogador navegue pela mansão interativamente.
 * Caminhos: 'e' para esquerda, 'd' para direita, 's' para sair.
 */
void explorarSalas(Sala* atual) {
    char opcao;

    printf("Bem-vindo à Mansão do Mistério!\n");

    while (atual != NULL) {
        printf("\nVocê está na sala: **%s**\n", atual->nome);

        // Verifica se é um nó-folha
        if (atual->esquerda == NULL && atual->direita == NULL) {
            printf("Esta sala não possui mais caminhos. Fim da exploração.\n");
            break;
        }

        // Exibe opções disponíveis
        printf("Escolha um caminho:\n");
        if (atual->esquerda != NULL) printf("[e] Ir para a sala à esquerda\n");
        if (atual->direita != NULL) printf("[d] Ir para a sala à direita\n");
        printf("[s] Sair da exploração\n");
        printf("Opção: ");
        scanf(" %c", &opcao);

        // Navegação
        if (opcao == 'e' && atual->esquerda != NULL) {
            atual = atual->esquerda;
        } else if (opcao == 'd' && atual->direita != NULL) {
            atual = atual->direita;
        } else if (opcao == 's') {
            printf("Você decidiu encerrar a exploração.\n");
            break;
        } else {
            printf("Opção inválida ou caminho inexistente. Tente novamente.\n");
        }
    }
}

/*
 * Função: main
 * ------------
 * Cria o mapa da mansão e inicia a exploração.
 */
int main() {
    // Construção da mansão (árvore binária)
    Sala* hallEntrada = criarSala("Hall de Entrada");
    Sala* salaEstar = criarSala("Sala de Estar");
    Sala* cozinha = criarSala("Cozinha");
    Sala* biblioteca = criarSala("Biblioteca");
    Sala* jardim = criarSala("Jardim");
    Sala* escritorio = criarSala("Escritório");
    Sala* adega = criarSala("Adega");

    // Ligando as salas (estrutura da árvore)
    hallEntrada->esquerda = salaEstar;
    hallEntrada->direita = cozinha;
    salaEstar->esquerda = biblioteca;
    salaEstar->direita = jardim;
    cozinha->direita = escritorio;
    jardim->esquerda = adega;

    // Início da exploração
    explorarSalas(hallEntrada);

    // Liberação de memória (boa prática, mas não essencial nesse exemplo curto)
    free(adega);
    free(jardim);
    free(biblioteca);
    free(escritorio);
    free(salaEstar);
    free(cozinha);
    free(hallEntrada);

    return 0;
}
