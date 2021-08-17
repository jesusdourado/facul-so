#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#define MAX 10 // Máximo tamanho da fila
#define BASE_QUANTUM 2 // Quantum base do SO
#define RANGE_RNG 4 
#define BEGIN_ASCII 65 // Gerar IDs em ordem alfabética

typedef struct {
    char ID;
    int quantum;
}Process;

Process processes[MAX]; // Fila de processos
int tam = 0; // Tamanho atual da fila
int id_generator = 0; // Gerador de IDs alfabéticos
int p_generator = 10; // Controla até quantos novos processos entrarão na fila 

// Gerador de aleatórios
int rng (int range) {
    return rand()%range + 1;
}

// Se a lista de processos não estiver cheia, cria um novo
void createProcess(bool verbose) {
    if(tam < MAX) {
        tam ++;

        Process novoP;
        novoP.ID = BEGIN_ASCII + id_generator;
        novoP.quantum = rng(6);
        processes[tam - 1] = novoP;

        // Controladores
        id_generator ++;
        p_generator --;

        // Verbose do novo processo 
        if(verbose) {
            printf("Processo %c com quantum(s) %d chegou na fila.\n\n", processes[tam - 1].ID, processes[tam - 1].quantum);
        }
    }
}

// Lista os processos com seus IDs e quantums
void listProcesses () {
    printf("Fila de processos.\n");
    for (int i = 0; i < tam ; i ++ ){
        printf(" - Processo: %c. Quantum(s): %d\n", processes[i].ID, processes[i].quantum);
    }
    printf("\n");
}

// Remove o primeiro processo da fila
void removeFirst () {
    for(int i = 1; i < tam; i ++) {
        processes[i - 1] = processes[i];
    }

    tam --;
}

// Move o primeiro processo para o fim da fila
void moveFirst2Last () {
    Process temp = processes[0];

    removeFirst();
    
    processes[tam] = temp;
    tam ++;
}

// Escalonador aloca o próximo processo
void allocNextProcess () {
    if(tam > -1) {
        printf("Alocando proximo processo para CPU.\n");

        // Primeiro processo da fila é alocado 
        printf("O processo %c requer %d quantum(s).\n", processes[0].ID, processes[0].quantum);
        printf(" - Executando...\n");
        sleep(BASE_QUANTUM);

        // Caso os quantums do processo seja maior que o entregue pelo escalonador 
        if(processes[0].quantum > BASE_QUANTUM) {
            // Recalcula o quantum
            processes[0].quantum -= BASE_QUANTUM;

            printf(" - Preempcao foi realizada. Ainda resta(m) %d quantum(s) para o processo %c.\n\n", processes[0].quantum, processes[0].ID);
           
            // Move o processo para o fim da fila 
            moveFirst2Last();
        } else { // Caso os quantums forem suficientes
            printf(" - Finalizado.\n\n");

            // Processo executou por completo, remove-o da lista
            removeFirst();
        }
    }
}

int main () {

   // Gera 4 processos iniciais 
    for(int i = 0; i < 4; i  ++)  {
        createProcess(false);
    }

    // Laço do escalonador
    while(1) {
        // Lista todos os processos da fila
        listProcesses();

        // Aloca próximo processo para CPU
        allocNextProcess();

        // Simula se há novos processos esperando para entrar na fila
        for(int i = 0; i < 2; i ++) { // Porcentagem de 50% de existirem novos
            if(rng(2) == 1 && p_generator > 0) {
                createProcess(true);
            } 
        }

        // Se não houver mais processos na fila, finalize.  
        if(tam == 0) {
            printf("Nao ha mais processos na fila.\n");
            break;
        }
    }

    printf("Fim.\n");

    return 0;
}