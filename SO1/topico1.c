#include <stdio.h>
#include <unistd.h>		// Biblioteca para as chamadas do sistema
#include <sys/types.h>		// Biblioteca para definir os tipos de dados nas chamadas do sistema
#include <sys/wait.h>

int main(){
	pid_t pid_pai, pid_filho;	// pid_t para armazenar os IDs dos processos

	printf("Avô: Meu PID é %d\n", getpid() );

	// Para criar o processo filho - fork()
	pid_pai = fork();

	if (pid_pai < 0) {
		perror("Erro ao criar o processo Pai");
		
		return 1;
	} else if (pid_pai == 0) {
		// Está no processo pai
		printf("Pai: Meu PID é %d e meu Pai (Avô) tem %d\n", getpid(), getppid() );
		// O getpid() obtém o ID do processo atual e o getppid() obtém o ID do processo pai

		// Cria o processo Filho (Neto)
		pid_filho = fork();

		if (pid_filho < 0){
			perror("Erro ao criar o processo Filho (Neto)");
			
			return 1;
		} else if (pid_filho == 0) {
			// Está no processo filho (Neto)
			printf("Filho: Meu PID é %d e meu pai tem PID %d\n", getpid(), getppid() );
			
		} else {
			// Processo Pai aguardando
			wait(NULL);	//Aguardar o filho terminar

		}	//Fim else

	} else {
		// Processo Avô aguardando o filho (pai) terminar
		wait(NULL);	// Aguarda o filho (pai) terminar
	}	// Fim else

	return 0;

}
