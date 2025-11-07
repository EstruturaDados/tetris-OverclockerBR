#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// --- Definições Globais ---
#define CAPACIDADE_FILA 5 // A fila deve conter exatamente 5 elementos
#define NOME_PECA_MAX 3   // Ex: 'I', 'O', 'T', 'L' + '\0'

// Estrutura que representa uma peça do jogo
typedef struct {
    int id;
    char nome[NOME_PECA_MAX];
} Peca;

// Estrutura da Fila Circular
typedef struct {
    Peca pecas[CAPACIDADE_FILA];
    int frente; // Índice da frente da fila
    int tras;   // Índice da traseira da fila (onde o próximo elemento será inserido)
    int tamanho; // Número atual de elementos na fila
    int proximo_id; // Gerador de ID exclusivo para peças
} FilaCircular;

// --- Protótipos das Funções ---
void inicializarFila(FilaCircular *fila);
Peca gerarPeca();
void exibirFila(FilaCircular *fila);
int filaCheia(FilaCircular *fila);
int filaVazia(FilaCircular *fila);
void inserirPeca(FilaCircular *fila, Peca novaPeca);
Peca removerPeca(FilaCircular *fila);
void menuPrincipal(FilaCircular *fila);

// --- Implementação das Funções ---

/**
 * @brief Inicializa a fila circular e preenche com 5 peças geradas automaticamente.
 * @param fila Ponteiro para a estrutura FilaCircular.
 */
void inicializarFila(FilaCircular *fila) {
    fila->frente = 0;
    fila->tras = 0;
    fila->tamanho = 0;
    fila->proximo_id = 1;
    srand(time(NULL)); // Inicializa o gerador de números aleatórios

    // Preenche a fila com 5 peças iniciais
    for (int i = 0; i < CAPACIDADE_FILA; i++) {
        inserirPeca(fila, gerarPeca());
    }
}

/**
 * @brief Gera uma nova peça com ID exclusivo e tipo aleatório ('I', 'O', 'T', 'L').
 * @return A nova estrutura Peca.
 */
Peca gerarPeca(FilaCircular *fila) {
    Peca novaPeca;
    
    // Gerar ID exclusivo
    novaPeca.id = fila->proximo_id++;

    // Tipos de peças disponíveis
    const char *tipos[] = {"I", "O", "T", "L", "J", "Z", "S"};
    int num_tipos = sizeof(tipos) / sizeof(tipos[0]);

    // Escolhe um tipo aleatório
    int indice_aleatorio = rand() % num_tipos;
    strcpy(novaPeca.nome, tipos[indice_aleatorio]);

    return novaPeca;
}

/**
 * @brief Verifica se a fila circular está cheia.
 * @param fila Ponteiro para a estrutura FilaCircular.
 * @return 1 se cheia, 0 caso contrário.
 */
int filaCheia(FilaCircular *fila) {
    return fila->tamanho == CAPACIDADE_FILA;
}

/**
 * @brief Verifica se a fila circular está vazia.
 * @param fila Ponteiro para a estrutura FilaCircular.
 * @return 1 se vazia, 0 caso contrário.
 */
int filaVazia(FilaCircular *fila) {
    return fila->tamanho == 0;
}

/**
 * @brief Insere uma nova peça no final da fila (enqueue).
 * @param fila Ponteiro para a estrutura FilaCircular.
 * @param novaPeca A peça a ser inserida.
 */
void inserirPeca(FilaCircular *fila, Peca novaPeca) {
    if (filaCheia(fila)) {
        printf("🚨 Erro: A fila está cheia. Não é possível inserir mais peças.\n");
        return;
    }
    fila->pecas[fila->tras] = novaPeca;
    fila->tras = (fila->tras + 1) % CAPACIDADE_FILA; // Avança o índice da traseira
    fila->tamanho++;
}

/**
 * @brief Remove a peça da frente da fila (dequeue).
 * @param fila Ponteiro para a estrutura FilaCircular.
 * @return A peça removida. Se a fila estiver vazia, retorna uma peça com ID 0.
 */
Peca removerPeca(FilaCircular *fila) {
    if (filaVazia(fila)) {
        printf("🚨 Erro: A fila está vazia. Não há peças para jogar.\n");
        Peca pecaVazia = {0, ""};
        return pecaVazia;
    }
    
    Peca pecaRemovida = fila->pecas[fila->frente];
    fila->frente = (fila->frente + 1) % CAPACIDADE_FILA; // Avança o índice da frente
    fila->tamanho--;

    return pecaRemovida;
}

/**
 * @brief Exibe o estado atual da fila, indicando a frente e a traseira.
 * @param fila Ponteiro para a estrutura FilaCircular.
 */
void exibirFila(FilaCircular *fila) {
    printf("\n--- 🕹️ Fila de Peças Futuras (Capacidade: %d) ---\n", CAPACIDADE_FILA);

    if (filaVazia(fila)) {
        printf("A fila está vazia.\n");
        return;
    }
    
    // Calcula o índice inicial e o fim para a iteração (do elemento mais antigo ao mais novo)
    int i = fila->frente;
    int count = 0;
    
    printf("Fila: [");
    while (count < fila->tamanho) {
        printf(" (%s #%d) ", fila->pecas[i].nome, fila->pecas[i].id);
        i = (i + 1) % CAPACIDADE_FILA;
        count++;
    }
    printf("]\n");

    // Indica a peça da frente (a próxima a ser jogada)
    printf("-> Próxima Peça (FRENTE): (%s #%d)\n", 
           fila->pecas[fila->frente].nome, fila->pecas[fila->frente].id);
    // Indica a posição da traseira (onde a nova peça será inserida)
    printf("-> Posição de Inserção (TRÁS): Índice %d\n", fila->tras);
    printf("-------------------------------------------------\n");
}


/**
 * @brief Lógica principal do menu de interação com o usuário.
 * @param fila Ponteiro para a estrutura FilaCircular.
 */
void menuPrincipal(FilaCircular *fila) {
    int opcao;
    
    do {
        exibirFila(fila);

        printf("\n--- ⚙️ Ações do Sistema ByteBros ---\n");
        printf("1. Jogar (remover) a peça da frente\n");
        printf("2. Visualizar a fila (atualizar)\n");
        printf("0. Sair\n");
        printf("Escolha uma opção: ");
        
        if (scanf("%d", &opcao) != 1) {
            // Limpa o buffer de entrada em caso de falha (entrada não numérica)
            while (getchar() != '\n');
            opcao = -1; // Valor inválido para forçar o loop
        }
        
        printf("\n");

        switch (opcao) {
            case 1: {
                // Tenta remover a peça
                Peca pecaJogada = removerPeca(fila);
                if (pecaJogada.id != 0) { // Se a peça foi removida com sucesso
                    printf("✅ Peça JOGADA: (%s #%d). \n", pecaJogada.nome, pecaJogada.id);
                    
                    // Inserção automática de uma nova peça para manter o tamanho
                    Peca novaPeca = gerarPeca(fila);
                    inserirPeca(fila, novaPeca);
                    printf("➕ Peça INSERIDA automaticamente: (%s #%d).\n", novaPeca.nome, novaPeca.id);
                }
                break;
            }
            case 2:
                // A visualização já ocorre no início do loop, mas permite um "refresh" manual.
                printf("🔄 Fila atualizada. \n");
                break;
            case 0:
                printf("👋 Saindo do sistema ByteBros. Até mais!\n\n");
                break;
            default:
                printf("❌ Opção inválida. Por favor, tente novamente.\n");
        }
        printf("\n");

    } while (opcao != 0);
}

/**
 * @brief Função principal.
 */
int main() {
    FilaCircular filaDePecas;
    
    inicializarFila(&filaDePecas);
    
    // Inicia o loop do menu
    menuPrincipal(&filaDePecas);
    
    return 0;
}