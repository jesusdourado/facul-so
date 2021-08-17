#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#define MAX 10 // Tamanho máximo da fila
#define BEGIN_ASCII 65 // Para gerar IDs em ordem alfabética

// Estrutura de um processo
typedef struct {
    char ID;
    int priority;
}Process;

Process processes[MAX]; // Fila de processos
int tam = 0; // Tamanho atual da fila
int id_generator = 0; // Controle de gerador de IDs
int p_generator = 10; // Controle de geração de novos processos

// Gerador de números aleatórios
int rng (int range) {
    return rand()%range + 1;
}

// Organiza a fila DECRESCENTEMENTE
void sortProcesses () {
    // Bubble sort
    for(int j = 0; j < tam ; j ++) {
        for(int i = 0; i < tam - 1; i ++) {
            if(processes[i].priority >= processes[i + 1].priority) {
                Process temp = processes[i];
                processes[i] = processes[i + 1];
                processes[i + 1] = temp;
            }
        }
    }
}

// Lista todos os processos
void listProcesses () {
    printf("Fila de processos.\n");
    for (int i = tam - 1; i > -1 ; i -- ){
        printf("- Processo: %c. Prioridade: %d\n", processes[i].ID, processes[i].priority);
    }
    printf("\n");
}

// Cria um novo processo e coloca na fila
void createProcess(bool verbose) {
    if(tam < MAX) {
        tam ++;
        Process novoP;
        novoP.ID = BEGIN_ASCII + id_generator;
        novoP.priority = rng(10);

        // Aloca-o na fila
        processes[tam - 1] = novoP;
        id_generator ++;
        p_generator --;

        if(verbose) {
            printf("Processo %c com prioridade %d criado.\n\n", processes[tam - 1].ID, processes[tam - 1].priority);
        }
    }
}

// Aloca o próximo processo
void allocNextProcess () {
    Process pAlocado;

    // Utiliza o último processo da fila
    pAlocado = processes[tam - 1];

    printf("Eh a vez do processo %c executar.\n", pAlocado.ID);
    printf("  Executando...\n");
    sleep(2);
    printf("  Finalizado.\n\n");

    tam --;
}


int main () {
    srand(time(NULL));

    // Gera 5 processos iniciais
    for(int i = 0; i < 5; i ++) {
        createProcess(false);
    }

    // Laço do escalonador
    while(1) {
        // Organiza todos os processos pela sua prioridade
        sortProcesses();

        // Lista todos os processos da fila
        listProcesses();

        // Escalonador aloca o próximo processo com maior prioridade para a CPU
        allocNextProcess();

        // Simula novos processos chegando para o escalonador 
        for(int i = 0; i < 2; i  ++) { // Probabilidade de 50% para os 2
            if(rng(2) == 1 && p_generator > 0) {
                createProcess(true);
            }
        }
        
        // Se não houver mais processos na fila, finaliza
        if(tam == 0) {
            printf("Fila de processos no escalonador vazia.\n");
            break;
        }
    }

    printf("Fim.\n");

    return 0;
}