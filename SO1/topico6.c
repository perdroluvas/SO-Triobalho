#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>

void exibirVetor(int vetor[], int tamanho) {
  printf("[ ");
  for (int i = 0; i < tamanho; i++) {
    printf("%d ", vetor[i]);
  }
  printf("]\n");
}

void ordenarVetor(int vetor[], int tamanho) {
  for (int i = 0; i < tamanho - 1; i++) {
    for (int j = 0; j < tamanho - i - 1; j++) {
      if (vetor[j] > vetor[j + 1]) {
        int temp = vetor[j];
        vetor[j] = vetor[j + 1];
        vetor[j + 1] = temp;
      }
    }
  }
}

int main() {
  int var;
  int vetor[100];

  var = 1;
  printf("PAI (antes do fork): var = %d\n", var);

  // Inicializa o vetor no Pai (valores: i² + 1)
  for (int i = 0; i < 100; i++) {
    vetor[i] = -i + 1;
  }

  printf("PAI: Vetor antes do fork:\n");
  exibirVetor(vetor, 100);

  pid_t pid = fork();

  if (pid == 0) { // Processo Filho
    var = 5;
    printf("\nFILHO: var = %d. Ordenando o vetor...\n", var);
    ordenarVetor(vetor, 100);
    printf("FILHO: Vetor ordenado:\n");
    exibirVetor(vetor, 100);
    exit(0);
  } 
  else { // Processo Pai
    waitpid(pid, NULL, 0); // Aguarda o Filho
    printf("\nPAI (após waitpid): var = %d\n", var);
    printf("PAI: Vetor após filho terminar:\n");
    exibirVetor(vetor, 100);
  }

  return 0;
}
