#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <pthread.h>
#include <time.h>
#include <unistd.h>

#define tamanho 20

int vetor[tamanho];

int verifica_par_impar(int num){
	for (int i = 0; i < tamanho; i++) {
		if (num % 2 == 0){
			return 0;

		} else {
			return 1;

		}
	}
}


int contador_par(){
	int contPar = 0;

	for (int i = 0; i < tamanho; i++) {
		if (verifica_par_impar(vetor[i]) == 0) {
			contPar++;
		}
	}

	return contPar;
}


int contador_impar() {
	int contImpar = 0;

	for (int i = 0; i < tamanho; i++) {
		if (verifica_par_impar(vetor[i]) == 1) {
			contImpar++;
		}
	}

	return contImpar;

}


//Função executada na thread
void* contar_pares_impares_thread(void* arg) {
	int* resultado = malloc(2 * sizeof(int) );	//Array com [pares, ímpares]
	resultado[0] = contador_par();
	resultado[1] = contador_impar();

	pthread_exit(resultado);
}


double medir_tempo_fork() {
	clock_t inicio = clock();
	double tempo_total = 0.00;

	pid_t pid = fork();
	if (pid < 0) {
		perror("Erro ao criar processo");
		exit(1);

	} else if (pid == 0) {	// Processo filho
		int pares = contador_par();
		int impares = contador_impar();
		printf("[Fork] Pares: %d | Ímpares: %d\n", pares, impares);
		exit(0);

	} else {	// Processo pai
		wait(NULL);
	}

	clock_t fim = clock();
	tempo_total = ((double)(fim - inicio) ) / CLOCKS_PER_SEC;

	return tempo_total;
}


double medir_tempo_thread() {
	clock_t inicio = clock();
	pthread_t thread;
	int* resultado;
	double tempo_total = 0.000;

	pthread_create(&thread, NULL, contar_pares_impares_thread, NULL);
	pthread_join(thread, (void**) &resultado);

	printf("[Threads] Pares: %d | Ímpares: %d\n", resultado[0], resultado[1] );
	free(resultado);

	clock_t fim = clock();
	tempo_total = ((double)(fim - inicio) ) / CLOCKS_PER_SEC;

	return tempo_total;
}

void exibir_vetor(const char *mensagem) {
	printf("\n%s:\n", mensagem);

	for (int i = 0; i < tamanho; i++) {
		printf("%d ", vetor[i]);
	}

	printf("\n");
}

void preencher_vetor() {
	srand(time(NULL) );
	for (int i = 0; i < tamanho; i++) {
		vetor[i] = rand() % 1000;
	}
}


int main() {
	double tempo_fork_total = 0, tempo_thread_total = 0;

	for (int i = 0; i < tamanho; i++){
		preencher_vetor();
		tempo_fork_total += medir_tempo_fork();
		tempo_thread_total += medir_tempo_thread();
	}

	printf("Média de tempo com: \n");
	printf("Fork:  %.7f segundos\n", tempo_fork_total / tamanho);
	printf("Thread:  %7f segundos\n", tempo_thread_total / tamanho);

	return 0;
}
