#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <limits.h>

typedef struct {
  int pid;              // ID do processo
  int arrival_time;     // Tempo de chegada
  int burst_time;       // Tempo de execução (burst)
  int remaining_time;   // Tempo restante (para SRTF e Round Robin)
  int priority;         // Prioridade (quanto menor, maior a prioridade)
  int completion_time;  // Tempo de conclusão
  int waiting_time;     // Tempo de espera
  int turnaround_time;  // Tempo de turnaround (CT - AT)
  int completed_flag;   // Flag: 0 = não concluído, 1 = concluído
} Process;

/* Protótipos das funções de simulação (agora com parâmetro extra "escolha") */
void simulate_SJF(Process proc[], int n, int escolha);
void simulate_SRTF(Process proc[], int n, int escolha);
void simulate_Priority(Process proc[], int n, int escolha);
void simulate_RoundRobin(Process proc[], int n, int time_slice, int escolha);
void printResults(Process proc[], int n);
void resetProcesses(Process proc[], int n);
/* Função para impressão da comparação entre algoritmos */
void printCompare(Process *proc_sjf, Process *proc_srtf, Process *proc_priority, Process *proc_rr, int n);

int main() {
  int n = 30, i, escolha, time_slice;
  int burst_time_values[3] = {5, 8, 12};

  /* Inicializa a semente do gerador de números aleatórios */
  srand(time(NULL));

  printf("Gerando %d processos...\n", n);

  /* Aloca vetor de processos (dados originais) */
  Process *proc = (Process*)malloc(n * sizeof(Process));
  if (proc == NULL) { /* Check */
    printf("Erro de alocação!\n");
    return 1;
  }

  /* Geração aleatória dos dados de cada processo */
  for (i = 0; i < n; i++) {
    proc[i].pid = i;
    /* Tempo de chegada aleatório (0 a 9) */
    proc[i].arrival_time = rand() % 20;
    printf("\n--- Processo %d ---\n", i);
    printf("Tempo de chegada: %d\n", proc[i].arrival_time);

    /* Burst time aleatório dentre {5, 8, 12} */
    proc[i].burst_time = burst_time_values[rand() % 3];
    proc[i].remaining_time = proc[i].burst_time;  // Inicializa o tempo restante
    printf("Tempo de burst: %d\n", proc[i].burst_time);

    /* Prioridade aleatória entre 1 e 5 (1 = maior prioridade) */
    proc[i].priority = (rand() % 5) + 1;
    printf("Prioridade: %d\n", proc[i].priority);

    /* Inicializa os demais campos */
    proc[i].completion_time = 0;
    proc[i].waiting_time = 0;
    proc[i].turnaround_time = 0;
    proc[i].completed_flag = 0;
  }

  printf("\nEscolha o algoritmo de escalonamento:\n");
  printf("1 - SJF (Shortest Job First - não preemptivo)\n");
  printf("2 - SRTF (Shortest Remaining Time First - preemptivo)\n");
  printf("3 - Prioridade (não preemptivo)\n");
  printf("4 - Round Robin\n");
  printf("5 - Comparar todos os algoritmos\n");
  printf("Escolha: ");
  scanf("%d", &escolha);

  if (escolha >= 1 && escolha <= 4) {
    /* Para escolhas 1 a 4, usa o vetor original e, se necessário, reinicializa os campos */
    resetProcesses(proc, n);
    switch (escolha) {
      case 1:
        printf("\n---- Simulação SJF (não preemptivo) ----\n");
        simulate_SJF(proc, n, escolha);
        break;
      case 2:
        printf("\n---- Simulação SRTF (preemptivo) ----\n");
        simulate_SRTF(proc, n, escolha);
        break;
      case 3:
        printf("\n---- Simulação de Prioridade (não preemptivo) ----\n");
        simulate_Priority(proc, n, escolha);
        break;
      case 4:
        printf("Digite o time-slice para Round Robin: ");
        scanf("%d", &time_slice);
        printf("\n---- Simulação Round Robin ----\n");
        simulate_RoundRobin(proc, n, time_slice, escolha);
        break;
    }
    /* Para escolhas 1 a 4, já é impresso o resultado individual (via printResults) */
  }
  else if (escolha == 5) {
    printf("\n---- Comparação dos algoritmos ----\n");
    /* Cria cópias dos processos para cada algoritmo (para usar os mesmos dados de entrada) */
    Process *proc_sjf = malloc(n * sizeof(Process));
    Process *proc_srtf = malloc(n * sizeof(Process));
    Process *proc_priority = malloc(n * sizeof(Process));
    Process *proc_rr = malloc(n * sizeof(Process));
    if (!proc_sjf || !proc_srtf || !proc_priority || !proc_rr) {
      printf("Erro de alocação nas cópias!\n");
      return 1;
    }
    for (i = 0; i < n; i++) {
      proc_sjf[i]      = proc[i];
      proc_srtf[i]     = proc[i];
      proc_priority[i] = proc[i];
      proc_rr[i]       = proc[i];
    }

    /* Executa cada simulação com o mesmo conjunto de processos.
       Para Round Robin, usaremos time-slice fixo de 2 unidades. */
    simulate_SJF(proc_sjf, n, escolha);
    simulate_SRTF(proc_srtf, n, escolha);
    simulate_Priority(proc_priority, n, escolha);
    simulate_RoundRobin(proc_rr, n, 2, escolha);

    /* Imprime a comparação entre os algoritmos */
    printCompare(proc_sjf, proc_srtf, proc_priority, proc_rr, n);

    /* Libera as cópias */
    free(proc_sjf);
    free(proc_srtf);
    free(proc_priority);
    free(proc_rr);
  }
  else {
    printf("Opção inválida!\n");
  }

  free(proc);
  return 0;
}

/* Reinicializa os campos dos processos para simulações sucessivas */
void resetProcesses(Process proc[], int n) {
  int i;
  for (i = 0; i < n; i++) {
    proc[i].remaining_time = proc[i].burst_time;
    proc[i].completion_time = 0;
    proc[i].waiting_time = 0;
    proc[i].turnaround_time = 0;
    proc[i].completed_flag = 0;
  }
}

/* SJF não preemptivo */
void simulate_SJF(Process proc[], int n, int escolha) {
  int current_time = 0, completed = 0, i, idx, min_burst;
  (void)escolha; /* Parâmetro não utilizado na lógica */

  while (completed < n) {
    idx = -1;
    min_burst = INT_MAX;
    /* Procura os processos já chegados e não concluídos */
    for (i = 0; i < n; i++) {
      if (proc[i].arrival_time <= current_time && proc[i].completed_flag == 0) {
        if (proc[i].burst_time < min_burst) {
          min_burst = proc[i].burst_time;
          idx = i;
        }
      }
    }
    if (idx == -1)
      current_time++;
    else {
      proc[idx].waiting_time = current_time - proc[idx].arrival_time;
      current_time += proc[idx].burst_time;
      proc[idx].completion_time = current_time;
      proc[idx].turnaround_time = proc[idx].completion_time - proc[idx].arrival_time;
      proc[idx].remaining_time = 0;
      proc[idx].completed_flag = 1;
      completed++;
      // Para simulações individuais, pode imprimir cada conclusão:
      if (escolha != 5)
        printf("Processo %d concluído em %d\n", proc[idx].pid, proc[idx].completion_time);
    }
  }
  if (escolha != 5)
    printResults(proc, n);
}

/* SRTF – Preemptive SJF */
void simulate_SRTF(Process proc[], int n, int escolha) {
  int current_time = 0, completed = 0, i, idx, min_remaining;
  (void)escolha;

  while (completed < n) {
    idx = -1;
    min_remaining = INT_MAX;
    /* Procura o processo com o menor tempo restante dentre os já chegados */
    for (i = 0; i < n; i++) {
      if (proc[i].arrival_time <= current_time && proc[i].completed_flag == 0) {
        if (proc[i].remaining_time < min_remaining) {
          min_remaining = proc[i].remaining_time;
          idx = i;
        }
      }
    }
    if (idx == -1)
      current_time++;
    else {
      proc[idx].remaining_time--;
      current_time++;
      if (proc[idx].remaining_time == 0) {
        proc[idx].completed_flag = 1;
        completed++;
        proc[idx].completion_time = current_time;
        proc[idx].turnaround_time = proc[idx].completion_time - proc[idx].arrival_time;
        proc[idx].waiting_time = proc[idx].turnaround_time - proc[idx].burst_time;
        if (escolha != 5)
          printf("Processo %d concluído em %d\n", proc[idx].pid, proc[idx].completion_time);
      }
    }
  }
  if (escolha != 5)
    printResults(proc, n);
}

/* Escalonamento por Prioridade (não preemptivo) */
void simulate_Priority(Process proc[], int n, int escolha) {
  int current_time = 0, completed = 0, i, idx, highest_priority;
  (void)escolha;

  while (completed < n) {
    idx = -1;
    highest_priority = INT_MAX;
    /* Procura o processo com a maior prioridade (menor valor) dentre os já chegados */
    for (i = 0; i < n; i++) {
      if (proc[i].arrival_time <= current_time && proc[i].completed_flag == 0) {
        if (proc[i].priority < highest_priority) {
          highest_priority = proc[i].priority;
          idx = i;
        }
      }
    }
    if (idx == -1)
      current_time++;
    else {
      proc[idx].waiting_time = current_time - proc[idx].arrival_time;
      current_time += proc[idx].burst_time;
      proc[idx].completion_time = current_time;
      proc[idx].turnaround_time = proc[idx].completion_time - proc[idx].arrival_time;
      proc[idx].remaining_time = 0;
      proc[idx].completed_flag = 1;
      completed++;
      if (escolha != 5)
        printf("Processo %d concluído em %d\n", proc[idx].pid, proc[idx].completion_time);
    }
  }
  if (escolha != 5)
    printResults(proc, n);
}

/* Round Robin */
void simulate_RoundRobin(Process proc[], int n, int time_slice, int escolha) {
  int current_time = 0, completed = 0, i, executed;
  (void)escolha;

  while (completed < n) {
    executed = 0;  /* Flag para verificar se algum processo foi executado nesta rodada */
    for (i = 0; i < n; i++) {
      if (proc[i].arrival_time <= current_time && proc[i].completed_flag == 0) {
        if (proc[i].remaining_time > 0) {
          executed = 1;
          int exec_time = (proc[i].remaining_time < time_slice) ? proc[i].remaining_time : time_slice;
          proc[i].remaining_time -= exec_time;
          current_time += exec_time;
          if (proc[i].remaining_time == 0) {
            proc[i].completed_flag = 1;
            completed++;
            proc[i].completion_time = current_time;
            proc[i].turnaround_time = proc[i].completion_time - proc[i].arrival_time;
            proc[i].waiting_time = proc[i].turnaround_time - proc[i].burst_time;
            if (escolha != 5)
              printf("Processo %d concluído em %d\n", proc[i].pid, proc[i].completion_time);
          }
        }
      }
    }
    if (!executed)
      current_time++;
  }
  if (escolha != 5)
    printResults(proc, n);
}

/* Função para exibir os resultados de um único algoritmo */
void printResults(Process proc[], int n) {
  int i;
  double total_waiting = 0, total_turnaround = 0;
  printf("\n-------------------------------------------------\n");
  printf("PID\tAT\tBT\tCT\tTAT\tWT\n");
  for (i = 0; i < n; i++) {
    printf("%d\t%d\t%d\t%d\t%d\t%d\n", 
           proc[i].pid, proc[i].arrival_time, proc[i].burst_time, 
           proc[i].completion_time, proc[i].turnaround_time, proc[i].waiting_time);
    total_waiting += proc[i].waiting_time;
    total_turnaround += proc[i].turnaround_time;
  }
  printf("-------------------------------------------------\n");
  printf("Tempo médio de espera: %.2f\n", total_waiting / n);
  printf("Tempo médio de turnaround: %.2f\n", total_turnaround / n);
}

/* Função para comparar os resultados dos 4 algoritmos em uma única tabela */
void printCompare(Process *proc_sjf, Process *proc_srtf, Process *proc_priority, Process *proc_rr, int n) {
  int i;
  double total_wait_sjf = 0, total_tat_sjf = 0;
  double total_wait_srtf = 0, total_tat_srtf = 0;
  double total_wait_pri = 0, total_tat_pri = 0;
  double total_wait_rr = 0, total_tat_rr = 0;

  printf("\nComparação dos Algoritmos (Valores individuais por processo):\n");
  printf("PID\t| SJF: CT TAT WT\t| SRTF: CT TAT WT\t| Pri: CT TAT WT\t| RR: CT TAT WT\n");
  printf("-----------------------------------------------------------------------------------------\n");
  for (i = 0; i < n; i++) {
    printf("%d\t| %3d %3d %3d\t| %3d %3d %3d\t| %3d %3d %3d\t| %3d %3d %3d\n",
           proc_sjf[i].pid,
           proc_sjf[i].completion_time, proc_sjf[i].turnaround_time, proc_sjf[i].waiting_time,
           proc_srtf[i].completion_time, proc_srtf[i].turnaround_time, proc_srtf[i].waiting_time,
           proc_priority[i].completion_time, proc_priority[i].turnaround_time, proc_priority[i].waiting_time,
           proc_rr[i].completion_time, proc_rr[i].turnaround_time, proc_rr[i].waiting_time);
    total_wait_sjf  += proc_sjf[i].waiting_time;
    total_tat_sjf   += proc_sjf[i].turnaround_time;
    total_wait_srtf += proc_srtf[i].waiting_time;
    total_tat_srtf  += proc_srtf[i].turnaround_time;
    total_wait_pri  += proc_priority[i].waiting_time;
    total_tat_pri   += proc_priority[i].turnaround_time;
    total_wait_rr   += proc_rr[i].waiting_time;
    total_tat_rr    += proc_rr[i].turnaround_time;
  }
  printf("\nMédias:\n");
  printf("Algoritmo\t\tTempo Médio de Espera\tTempo Médio de Turnaround\n");
  printf("SJF\t\t\t%.2f\t\t\t%.2f\n", total_wait_sjf/n, total_tat_sjf/n);
  printf("SRTF\t\t\t%.2f\t\t\t%.2f\n", total_wait_srtf/n, total_tat_srtf/n);
  printf("Prioridade\t\t%.2f\t\t\t%.2f\n", total_wait_pri/n, total_tat_pri/n);
  printf("Round Robin\t\t%.2f\t\t\t%.2f\n", total_wait_rr/n, total_tat_rr/n);
}

