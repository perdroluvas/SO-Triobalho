#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

void *threadFunc(void *arg) {
    int c = *(int *)arg;  // Recebe o valor passado (valor da iteração)
    sleep(3);             // Simula uma tarefa (aguarda 3 segundos)
    c += 1;               // Incrementa o valor
    printf("Total de threads %d\n", c);
    free(arg);            // Libera a memória alocada para o argumento
    pthread_exit(NULL);   // Termina a thread
}

int main() {
    int var = 1;  // Variável de exemplo, semelhante ao seu código de processos
    printf("MAIN (antes de criar threads): var = %d\n", var);
    
    // Loop para criar 10 threads (para testar, você pode aumentar esse número)
    for (int c = 0; c < 10; c++) {
        pthread_t tid;
        // Aloca memória para passar o valor da iteração para a thread
        int *arg = malloc(sizeof(int));
        if (arg == NULL) {
            perror("Erro na alocação de memória");
            exit(EXIT_FAILURE);
        }
        *arg = c;
        
        // Cria a thread
        if (pthread_create(&tid, NULL, threadFunc, arg) != 0) {
            perror("Erro ao criar thread");
            exit(EXIT_FAILURE);
        }
        
        // Aguarda a thread terminar antes de criar a próxima,
        // garantindo uma criação linear de threads, semelhante ao exit(1) no código de processos
        pthread_join(tid, NULL);
    }
    
    return 0;
}

