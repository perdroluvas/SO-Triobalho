#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>

int main() {
  int var; // Variável visível ao Pai e ao Filho
  var = 1; // Inicialização no Pai
  printf("PAI (antes do fork): var = %d\n", var);
  
  for (int c = 0; c < 10; c++) {
    pid_t pid = fork();
    if (pid == 0) {
      sleep(3);
      c += 1;
      printf("Total de procs %d\n", c);
      exit(1);
    }
  }
  return 0;
}
