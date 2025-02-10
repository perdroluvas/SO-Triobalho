#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>

#define USER 256

// Função para remover o caractere '\n' do final da string
void Rm_Enter(char *str);

// Função para remover espaços em branco do início da string
char* Excsp(char *str);

// Função para comparar as primeiras n posições de duas strings
int ComparaStrings(const char *stra, const char *strb, int n);

int main() {
    char cmd[USER];

    while (1) {
        printf("$ ");
        if (!fgets(cmd, USER, stdin))
            continue;

        Rm_Enter(cmd); // Remove o '\n' da entrada

        // Verifica se o comando é "fzdir"
        if (ComparaStrings(cmd, "fzdir", 5) == 0) {
            char *di = Excsp(cmd + 5); // Pula "fzdir" e espaços à frente
            if (*di == '\0') {
                printf("Como usar: fzdir <nome_do_diretorio>\n");
                continue;
            }
            
            // Verifica se o nome do diretório está vazio ou tem apenas espaços
            if (di[0] == '\0') {
                printf("Comando inválido!\n");
                continue;
            }

            // Criação do diretório
            if (mkdir(di, 0777) != 0)
                perror("mkdir"); // mostra um erro se falhar
            continue;
        }
        
        // Verifica se o comando é "flw"
        if (ComparaStrings(cmd, "flw", 3) == 0) {
            puts("Saindo...");
            break; 
        }

        // Caso não satisfaça as condições anteriores
        printf("Comando desconhecido, tente novamente\n");
    }
    
    return 0;
}

void Rm_Enter(char *su) {
    while (*su) {
        if (*su == '\n') {
            *su = '\0';  // Substitui o '\n' por '\0' (null)
            break;      
        }
        su++;
    }
}

int ComparaStrings(const char *stra, const char *strb, int n) {
    for (int i = 0; i < n; i++) {
        if (stra[i] != strb[i]) {    
            return stra[i] - strb[i]; // Caso os elementos sejam diferentes, retorna a diferença a - b pra cada i++
        }
        if (stra[i] == '\0') {
            break; // Se na posição i de stra tiver o '\n', interrompe a comparação
        }
    }
    return 0; // Caso as strings sejam iguais, retorna 0
}

char* Excsp(char *s) {
    while (*s == ' ')  
        s++; // Enquanto o caractere s for espaço, incrementa 1
    return s;
}
