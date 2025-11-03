/*
 * Atividade Avaliativa: Threads em C
 * 
 * Este programa demonstra o uso de threads POSIX em C com três tipos de operações:
 * 1. Operações apenas de CPU (cálculos matemáticos intensivos)
 * 2. Operações de I/O (leitura/escrita de arquivos)
 * 3. Operações mistas de CPU e I/O
 * 
 * Objetivo: Observar o comportamento das threads em diferentes cenários
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>
#include <math.h>
#include <string.h>

#define NUM_ITERACOES 1000000
#define NUM_LINHAS_IO 10000
#define NUM_ITERACOES_MISTA 100
#define TAMANHO_BUFFER 1024

// Estrutura para passar dados para as threads
typedef struct {
    int thread_id;
    char nome[50];
} ThreadData;

/*
 * Thread 1: Operações apenas de CPU
 * Realiza cálculos matemáticos intensivos (números primos, fatoriais)
 */
void* funcao_cpu(void* arg) {
    ThreadData* data = (ThreadData*)arg;
    printf("[Thread %d - %s] Iniciando operações de CPU...\n", data->thread_id, data->nome);
    
    clock_t inicio = clock();
    
    // Cálculo intensivo: encontrar números primos
    long long soma = 0;
    for (int i = 2; i < NUM_ITERACOES; i++) {
        int eh_primo = 1;
        for (int j = 2; j <= sqrt(i); j++) {
            if (i % j == 0) {
                eh_primo = 0;
                break;
            }
        }
        if (eh_primo) {
            soma += i;
        }
    }
    
    clock_t fim = clock();
    double tempo_cpu = ((double)(fim - inicio)) / CLOCKS_PER_SEC;
    
    printf("[Thread %d - %s] Operações de CPU concluídas!\n", data->thread_id, data->nome);
    printf("[Thread %d - %s] Soma dos primos: %lld\n", data->thread_id, data->nome, soma);
    printf("[Thread %d - %s] Tempo de CPU: %.2f segundos\n\n", data->thread_id, data->nome, tempo_cpu);
    
    pthread_exit(NULL);
}

/*
 * Thread 2: Operações apenas de I/O
 * Realiza leitura e escrita de arquivos
 */
void* funcao_io(void* arg) {
    ThreadData* data = (ThreadData*)arg;
    printf("[Thread %d - %s] Iniciando operações de I/O...\n", data->thread_id, data->nome);
    
    clock_t inicio = clock();
    char nome_arquivo[100];
    sprintf(nome_arquivo, "thread_%d_io.txt", data->thread_id);
    
    // Escrita em arquivo
    FILE* arquivo = fopen(nome_arquivo, "w");
    if (arquivo == NULL) {
        perror("Erro ao abrir arquivo para escrita");
        pthread_exit(NULL);
    }
    
    for (int i = 0; i < NUM_LINHAS_IO; i++) {
        fprintf(arquivo, "Thread %d - Linha %d: Esta é uma operação de I/O\n", data->thread_id, i);
    }
    fclose(arquivo);
    
    // Leitura do arquivo
    arquivo = fopen(nome_arquivo, "r");
    if (arquivo == NULL) {
        perror("Erro ao abrir arquivo para leitura");
        pthread_exit(NULL);
    }
    
    char buffer[TAMANHO_BUFFER];
    int linhas_lidas = 0;
    while (fgets(buffer, TAMANHO_BUFFER, arquivo) != NULL) {
        linhas_lidas++;
    }
    fclose(arquivo);
    
    // Remove o arquivo temporário
    remove(nome_arquivo);
    
    clock_t fim = clock();
    double tempo_cpu = ((double)(fim - inicio)) / CLOCKS_PER_SEC;
    
    printf("[Thread %d - %s] Operações de I/O concluídas!\n", data->thread_id, data->nome);
    printf("[Thread %d - %s] Linhas escritas e lidas: %d\n", data->thread_id, data->nome, linhas_lidas);
    printf("[Thread %d - %s] Tempo total: %.2f segundos\n\n", data->thread_id, data->nome, tempo_cpu);
    
    pthread_exit(NULL);
}

/*
 * Thread 3: Operações mistas de CPU e I/O
 * Alterna entre cálculos e operações de arquivo
 */
void* funcao_mista(void* arg) {
    ThreadData* data = (ThreadData*)arg;
    printf("[Thread %d - %s] Iniciando operações mistas (CPU + I/O)...\n", data->thread_id, data->nome);
    
    clock_t inicio = clock();
    char nome_arquivo[100];
    sprintf(nome_arquivo, "thread_%d_misto.txt", data->thread_id);
    
    FILE* arquivo = fopen(nome_arquivo, "w");
    if (arquivo == NULL) {
        perror("Erro ao abrir arquivo");
        pthread_exit(NULL);
    }
    
    long long total_calculos = 0;
    
    // Alterna entre CPU e I/O
    for (int iteracao = 0; iteracao < NUM_ITERACOES_MISTA; iteracao++) {
        // Parte CPU: Cálculo de fatorial
        long long fatorial = 1;
        for (int i = 1; i <= 20; i++) {
            fatorial *= i;
        }
        total_calculos += fatorial;
        
        // Parte I/O: Escrever resultado no arquivo
        fprintf(arquivo, "Iteração %d: Fatorial de 20 = %lld\n", iteracao, fatorial);
        
        // Simula mais processamento
        for (int i = 0; i < 10000; i++) {
            total_calculos += sqrt(i);
        }
    }
    
    fclose(arquivo);
    
    // Leitura do arquivo
    arquivo = fopen(nome_arquivo, "r");
    if (arquivo != NULL) {
        char buffer[TAMANHO_BUFFER];
        int linhas = 0;
        while (fgets(buffer, TAMANHO_BUFFER, arquivo) != NULL) {
            linhas++;
        }
        fclose(arquivo);
        printf("[Thread %d - %s] Linhas processadas: %d\n", data->thread_id, data->nome, linhas);
    }
    
    // Remove o arquivo temporário
    remove(nome_arquivo);
    
    clock_t fim = clock();
    double tempo_cpu = ((double)(fim - inicio)) / CLOCKS_PER_SEC;
    
    printf("[Thread %d - %s] Operações mistas concluídas!\n", data->thread_id, data->nome);
    printf("[Thread %d - %s] Total de cálculos: %lld\n", data->thread_id, data->nome, total_calculos);
    printf("[Thread %d - %s] Tempo total: %.2f segundos\n\n", data->thread_id, data->nome, tempo_cpu);
    
    pthread_exit(NULL);
}

int main(int argc, char* argv[]) {
    pthread_t threads[3];
    ThreadData thread_data[3];
    int rc;
    
    printf("=================================================\n");
    printf("   ATIVIDADE: Demonstração de Threads em C\n");
    printf("=================================================\n\n");
    
    printf("Este programa cria 3 threads diferentes:\n");
    printf("1. Thread CPU: Operações intensivas de processamento\n");
    printf("2. Thread I/O: Operações de leitura/escrita em arquivo\n");
    printf("3. Thread Mista: Combinação de CPU e I/O\n\n");
    
    time_t tempo_inicio = time(NULL);
    
    // Prepara dados da Thread 1 (CPU)
    thread_data[0].thread_id = 1;
    strcpy(thread_data[0].nome, "CPU");
    
    // Prepara dados da Thread 2 (I/O)
    thread_data[1].thread_id = 2;
    strcpy(thread_data[1].nome, "I/O");
    
    // Prepara dados da Thread 3 (Mista)
    thread_data[2].thread_id = 3;
    strcpy(thread_data[2].nome, "Mista");
    
    printf("Criando threads...\n\n");
    
    // Cria a Thread 1 (CPU)
    rc = pthread_create(&threads[0], NULL, funcao_cpu, (void*)&thread_data[0]);
    if (rc) {
        fprintf(stderr, "Erro ao criar thread 1: %d\n", rc);
        exit(EXIT_FAILURE);
    }
    
    // Cria a Thread 2 (I/O)
    rc = pthread_create(&threads[1], NULL, funcao_io, (void*)&thread_data[1]);
    if (rc) {
        fprintf(stderr, "Erro ao criar thread 2: %d\n", rc);
        exit(EXIT_FAILURE);
    }
    
    // Cria a Thread 3 (Mista)
    rc = pthread_create(&threads[2], NULL, funcao_mista, (void*)&thread_data[2]);
    if (rc) {
        fprintf(stderr, "Erro ao criar thread 3: %d\n", rc);
        exit(EXIT_FAILURE);
    }
    
    // Aguarda todas as threads terminarem
    for (int i = 0; i < 3; i++) {
        pthread_join(threads[i], NULL);
    }
    
    time_t tempo_fim = time(NULL);
    double tempo_total = difftime(tempo_fim, tempo_inicio);
    
    printf("=================================================\n");
    printf("Todas as threads foram concluídas!\n");
    printf("Tempo total de execução: %.0f segundos\n", tempo_total);
    printf("=================================================\n");
    
    return 0;
}
