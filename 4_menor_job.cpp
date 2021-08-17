#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#define MAX 10 
#define RANGE_RNG 4
#define BEGIN_ASCII 65

int pTime[MAX]; // Lista de tempo de execução dos processos
char pID[MAX]; // Lista de ID dos processos 
int tam = 0; // Tamanho da fila de processos
int ID_generator = 0; // Controla o ID alfabético dos processos 
int p_generator = 13; // Controla a geração de novos processos

// Gerador de números aleatórios
int rng (int range) {
    return rand()%range + 1;
}

// Se a lista de processos não estiver cheia, cria um novo
void createProcess (bool verbose) {
    if(tam < MAX) {
        tam ++;
        pTime[tam - 1] = rng(RANGE_RNG);
        pID[tam - 1] = ID_generator + BEGIN_ASCII;
        ID_generator ++;
        p_generator --;

        if(verbose) {
            printf("O novo processo %c de %d segundos chegou no escalonador.\n\n", pID[tam - 1], pTime[tam - 1]);
        }
    }
}

// Organiza a lista de processo por tempo de execução DECRESCENTEMENTE
void sortProcesses () {
    if(tam > 0) {
        for(int j = 0; j < tam; j ++) {
            for(int i = 0; i < tam - 1; i ++) {
                if(pTime[i + 1] >= pTime[i]) {
                    // Atualiza a lista de tempos
                    int tempT = pTime[i + 1];
                    pTime[i + 1] = pTime[i];
                    pTime[i] = tempT;

                    // Atualiza a lista de IDs
                    char tempC = pID[i + 1];
                    pID[i + 1] = pID[i];
                    pID[i] = tempC;
                } 
            }
        }
    }
}

// Aloca novo processo na CPU, espera que ele finalize e o remove da lista de processos prontos
void allocNextProcess() {
    // Utiilza-se o último processo da lista, o menor
    int processTime = pTime[tam - 1];
    char processID = pID[tam - 1];

    printf("Processo %c esta usando a CPU por %d segundos.\n", processID, processTime);

    printf("-- Rodando...\n");
    // Executa no seu tempo estipulado
    sleep(processTime);
    printf("-- Finalizou.\n\n");

    tam --;
}

// Faz o cálculo de probabilidade de 50%
bool verifyNewProcesses () {
    int prob = rand()%2 + 1;

    if(prob == 1) return true;
    else return false;
}

// Lista os processos e seus tempos de execução
void listProcesses () {
    printf("Processos prontos na fila:\n");
    for(int i = tam - 1; i > -1; i --) {
        printf("  - Processo %c: %d segundos\n", pID[i], pTime[i]);
    }
    printf("\n");
}


int main () {
    srand(time(NULL));

    // Gera 3 processos
    for(int i = 0; i < 3; i ++) {
        createProcess(false);
    }

    while(1) {
        // Organiza a fila de processos pelo menor
        sortProcesses();

        // Lista os processos prontos
        listProcesses();

        // Aloca o menor processo para a CPU
        allocNextProcess();

        // Tenta criar 3 processos, probabilidade de 50% para cada um ser criado
        if(verifyNewProcesses() && p_generator > 0) createProcess(true);
        if(verifyNewProcesses() && p_generator > 0) createProcess(true);
        if(verifyNewProcesses() && p_generator > 0) createProcess(true);

        // Se não houver mais processos na lista do escalonador, para.
        if (tam == 0) {
            printf("Nao ha mais processos para escalonar.\n");
            break;
        }
    }

    printf("Fim.\n");

    return 0;
}
