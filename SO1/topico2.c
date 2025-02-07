#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(){
	pid_t pid_filho1, pid_filho2;

	printf("Pai: Olá, meu PID é %d\n", getpid() );

	pid_filho1 = fork();	// Cria o processo filho 1

	if (pid_filho1 < 0) {
		perror("Erro ao criar o processo do filho 1");
		
		return 1;
	} else if (pid_filho1 == 0) {
		// Está no processo filho 1
		printf("Filho 1: Meu PID é %d e meu pai tem %d", getpid(), getppid());
		printf(" safo? \n");
		return 0;	// Termina o processo do filho 1
	}


	pid_filho2 = fork();	// Cria o processo filho 2
	if (pid_filho2 < 0) {
		perror("Erro ao criar o processo do filho 2");
		
		return 1;
	} else if (pid_filho2 == 0) {
		// Está no processo filho 2
		printf("Filho 2: Meu PID é %d e meu pai tem %d", getpid(), getppid() );
		printf(" blz!\n");
		
		return 0;	// Termina o processo do filho 2
	}

	// Processo pai aguardando o filho 1 e 2 terminarem
	wait(NULL);
	wait(NULL);

	printf("Pai: Filhos terminaram. Finalizando o programa... \n");

	return 0;
}
