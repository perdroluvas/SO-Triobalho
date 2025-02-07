#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h> // Incluído para usar kill() e SIGKILL

int main() {
  printf("Avô: Meu PID é %d\n", getpid());

  pid_t pid_pai = fork();

  if (pid_pai < 0) {
    perror("Erro ao criar o processo Pai");
    return 1;
  } else if (pid_pai == 0) { // Processo Pai
    printf("Pai: Meu PID é %d e meu Pai (Avô) tem PID %d\n", getpid(), getppid());

    pid_t pid_filho = fork();

    if (pid_filho < 0) {
      perror("Erro ao criar o processo Filho");
      return 1;
    } else if (pid_filho == 0) { // Processo Filho
      printf("Filho: Meu PID é %d e meu Pai tem PID %d\n", getpid(), getppid());
      pid_t pid_pai_atual = getppid();
      printf("Agora matarei meu Pai! (PID %d)\n", pid_pai_atual);

      // Envia sinal SIGKILL para o Pai
      kill(pid_pai_atual, SIGKILL);

      // Dá tempo para o sistema atualizar o processo pai
      sleep(1);

      printf("Meu novo Pai é o processo com PID %d\n", getppid());
      exit(0);
    } else { // Processo Pai espera pelo Filho
      wait(NULL);
      exit(0); // Encerra o Pai após o Filho terminar
    }
  } else { // Processo Avô espera pelo Pai
    wait(NULL);
  }

  return 0;
}
