#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>

int main() {
    int var; // Variável visível ao Pai e ao Filho
    
    var = 1; // Inicialização no Pai
    printf("PAI (antes do fork): var = %d\n", var);

    pid_t pid = fork();
    
    if (pid == 0) { // Processo Filho
        var = 5;
        printf("FILHO: var alterada para %d\n", var);
        exit(0); // Termina o processo Filho
    } 
    else { // Processo Pai
        waitpid(pid, NULL, 0); // Aguarda o Filho terminar
        printf("PAI (após waitpid): var = %d\n", var);
    }

    return 0;
}
