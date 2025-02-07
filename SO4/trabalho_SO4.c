#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define numero_de_alunos 18
#define numero_de_vagas 10

sem_t vagas;
pthread_mutex_t mutex;
int alunos_matriculados = 0;

void* aluno(void* id) {
	int aluno_id = *(int*)id;
	printf("Aluno %d solicitou matricula em FÍSICA II!\n", aluno_id);
	
	while (1) {
		if(sem_trywait(&vagas) == 0 ) {	//Tentar vaga
			pthread_mutex_lock(&mutex);
			alunos_matriculados++ ;
			pthread_mutex_unlock(&mutex);

			printf("Aluno %d conseguiu a matrícula!\n", aluno_id);
			sleep(15);	//Tempo cursando a disciplina
			printf("Aluno %d concluiu a disciplina e liberou a vaga.\n", aluno_id);
			printf("____________________________________________________________\n");
			sem_post(&vagas);	// Libera a vaga
			break;
		} else {
			printf("Aluno %d está esperando uma vaga.\n", aluno_id);
			printf("_____________________________________________\n");
			sleep(2);
		}
	}
	return NULL;

}

int main() {
	pthread_t alunos[numero_de_alunos];
	int id[numero_de_alunos];

	sem_init(&vagas, 0, numero_de_vagas);
	pthread_mutex_init(&mutex, NULL);

	for (int i = 0; i < numero_de_alunos; i++) {
		id[i] = i + 1;
		pthread_create(&alunos[i], NULL, aluno, &id[i] );
		sleep(1);	//Tempo para cada aluno tentar se matricular
	}

	for (int i = 0; i < numero_de_alunos; i++) {
		pthread_join(alunos[i], NULL);
	}
	printf("__________________________________________________\n");
	printf("Todos os alunos conseguiram cursar a disciplina.\n");
	printf("__________________________________________________\n");
	sem_destroy(&vagas);
	pthread_mutex_destroy(&mutex);
	return 0;
}
