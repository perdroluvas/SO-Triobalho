section .bss  ; aqui fica as variaveis não inicializadas

    entrada_usuario: resb 256  ;Variavel que ficará a resposta/entrada do usuario
                               ; O resb serve para dizer que será armazenado 256 bytes pra variavel entrada_usuario


section .data ; aqui fica as variaveis inicializadas
              ; o db serve pra definir o que terá 1 byte, como é uma string, o assembly vai deixar cada coluna com 1 byte, de forma 'individual'
            
    terminal: db "$ ", 0        ; os 0 servem para dizer que a string termina
    cmd_fzdir: db "fzdir", 0     
    cmd_flw: db "flw", 0        
    help_usuario: db "Como usar: fzdir <nome_do_diretorio>", 10, 0  ; os 10 servem pra quebrar linha
    erro_fzdir: db "Erro: Nao foi possivel criar o diretorio!", 10, 0  
    erro_comando: db "Erro: Comando não reconhecido", 10, 0  

section .text ; Aqui faz o codigo
    global _start 

_start:
    ;Obs: todos os comandos meio que são dependentes do que carregamos ao acumulador

    ; escrever o que tá em terminal
    mov rax, 1          ; chamada de sistema pra 'write'
    mov rdi, 1          ; mostrar a coisa escrita no rax (stdout)
    lea rsi, [terminal] ; o 'lea' serve só pra ler o endereço da variavel, no caso, vai ler o endereço de terminal
    mov rdx, 2          ; aqui damos o tamanho (em byte) da string, no caso é 2 pq contamos o espaço
    syscall             ; chama o sistema pra chamada do sistema 'write' 

    ; Ler o comando do usuario
    mov rax, 0                    ; chamada de systema pra 'read'
    mov rdi, 0                    ; 0 ao rdi quando rax é 0 (read) é um 'input' ou stdin
    lea rsi, [entrada_usuario]    ; o endereço da entrada do usuario é guardado no rsi
    mov rdx, 256                  ; tamanho maximo em bytes da entrada do usuario
    syscall                       ; chama o sistema pra chamada do sistema 'read'

    ; Remove o \n do final, pra evitar o problema de não reconhecer a função 'fzdir' pelo '\n' do enter 'fzdir\n'
    lea rdi, [entrada_usuario]           ; o endereço da entrada do usuario fica armazenada em rdi (parametro 1)
    call remove_enter                     

    ; ver se o comando é 'fzdir'
    lea rdi, entrada_usuario     ; move o endereço entrada do usuario pro rdi (parametro 1)
    lea rsi, [cmd_fzdir]         ; move o endereço da variavel q tá nome/comando 'fzdir' pro rsi (parametro 2)
    call compstring              ; chamar a função compstring
    cmp rax, 0                   ; Se as strings (oque o usuario digitar com 'fzdir') forem iguais no compstring, o rax será 0; compara 0 com 0 e dá 0. (0-0=0)
    jz Construir_fzdir           ; se for 0 vai ter um 'goto' para a função 'Construir_fzdir'
                                 ; se não for, vai simplismente pular esse 'goto' e vai pro 'mini programa' de baixo

    ; ver se o comando é "sair"
    lea rdi, [entrada_usuario]    ; move o endereço da entrada do usuario pro rdi 
    lea rsi, [cmd_flw]           ; move o endereço da variavel q tá o nome/comando 'flw' pro rsi
    call compstring              ;comparar os valores que estão nos endereços q tão agr em rdi e rsi
    cmp rax, 0                   ; compara rax com 0 
    jz sair                      ; caso for 0, terá o 'goto' pra a função sair
                                 
    jmp cmd_desconhecido ;  se veio pra cá é pq não é nenhum dos dois comandos, então vai ter o 'goto' para a função que tá ai



; construção do diretorio
Construir_fzdir:
    
    ; deixar só o espaço + nome da pasta e verificar se não foi dado o nome (tipo 'fzdir ', só espaço e sem argumento)
    lea rdi, [entrada_usuario + 5]; Pula "fzdir" (isso pq ele pula 5 bytes da entrada_usuario) e deixa só a parte do endereço do espaço+pasta como parametro pra função
    call pular_espaco             ; tirar a 'sujeira' que é o espaço

    cmp byte [rdi], 0 ; compara o primeiro byte de rdi com 0 
    je erro_fzdir_help     ; 'se for igual vai ter um goto' para a função dita
                    
     
    ; construir a pasta
    mov rax,83          ; 83 ao acumulador é a chamada de sistema pra mkdir 
    mov rsi, 0o666      ; mover pra 0o666 na chamada 83 sigifica que vamos dar as permissões '666' pro diretorio
    syscall             ; chamada do sistema pra mkdir
    test rax, rax         ; verifica se rax é positivo/0 (tudo certo) ou negativo (tudo errado)
    js erro_fzdir_desconhecido   ; se o resultado for negativo vai ter um salto 
    jmp _start                   ; se não saltar pra 'start'

; exceção pra caso de o usuario digitar só o o fzdir
erro_fzdir_help:
    mov rax, 1  ; chamada de sistema para write        
    mov rdi, 1  ; mostrar a saida 
    lea rsi, [help_usuario] ; o endereço de memoria passado pro rsi
    mov rdx, 38          ; tamanho de help_usuario em bytes 
    syscall              ; chama o sistema pra write
    jmp _start         

; exceção pra caso tenha um erro não esperado  
erro_fzdir_desconhecido:
    mov rax, 1          ; chamada de sistema pra write
    mov rdi, 1          ; mostrar a saida do que tá em rax
    lea rsi, [erro_fzdir] ; endereço é passado pro rsi
    mov rdx, 42           ; tamanho em bytes da mensagem
    syscall               
    jmp _start            ; salta pra start 

; exceção para um comando desconhecido
cmd_desconhecido:
    mov rax, 1          ; chamada de sistema pra write
    mov rdi, 1          ; mostrar a saida do que tá em rax
    lea rsi, [erro_comando] ; endereço passada pro rsi
    mov rdx, 31         ; tamanho em bytes da mensagem
    syscall             
    jmp _start          ; salta pra start

sair:
    mov rax, 60         ; chamada de sistema para exit, desde que o rdi tenha 0
    mov rdi, 0          ; como moveu 0 ao rdi, satisfez a condição e pode sair       
    syscall             

; remover o caractere do teclado '\n'
remove_enter:
    mov rsi, rdi        ; o endereço que tava em rdi (entrada_usuario) é movido para rsi
loop_remove_enter:      
    cmp byte [rsi], 0   ; compara um byte da entrada_usuario com 0 (entrada_usuario - 0), e a unica forma de dar zero é sendo 0 (null)
    je remove_enter_acabado ; caso sejam iguais, termina o loop, se não vai pra proxima linha
    cmp byte [rsi], 10  ;  compara um byte da entrada_usuario com 10 ('\n' no ascii 1 byte)
    jne incrementador      ; se não for igual salta pra função incrementador 
    mov byte [rsi], 0   ; se vir pra cá é pq aquilo era um '\n', então vai mover 'null' (0) pra ela
remove_enter_acabado:  
    ret  ;
incrementador:  
    inc rsi  ; incrementa (+1) ao endereço de rsi, e assim vai até achar o '\n'
    jmp loop_remove_enter ; volta novamente pro loop, só que incrementada em 1,

; pular espaços em branco na hora do comando do usario
pular_espaco:
    cmp byte [rdi], ' ' ; compara cada byte do conteudo apontado por rdi com '' 
    jne fim_pular_espaco ; se não for igual salta. se não vai pra proxima linha
    inc rdi              ; incrementa o rdi
    jmp pular_espaco       ; volta pro inicio da função
fim_pular_espaco:
    ret                  ; retorna pra uma linha dps da ultima vez que foi chamado a 'call'

; Compara duas strings, a da entrada do usuario e a string do 'comando'
compstring:
    mov rax, 0              ; 0 foi movido pro rax, como não tem syscall é o valor literal
    mov rcx, 5              ;o 'loop' será de decremento e que serão 5 char que serão comparados. (a maior string é 'fzdir' com 5  caracteres)
loop_compstring:
    mov al, [rsi]           ;  move o conteudo do endereço que tá no rsi ao registrador al
    mov bl, [rdi]           ; move o conteido do endereço que tá no rdi  pro registrador bl
    cmp al, bl              ; compara as strings, byte a byte. se for a mesma o resultado dará 0 em cada comparação
    jne compstring_acabado  ; caso as 2 primeiras a1 e a2 não sejam iguais vai pois logo de cara é diferente
    cmp al, 0               ; comparar pra ver se a1 vale 0 (null)
    jz compstring_acabado   ; se for 0, então tem o salto 
    inc rsi                 ; incrementa rsi e compara o elemento 'n+1'
    inc rdi                ; incrementa rdi e compara o elemento 'm+1'
    loop loop_compstring   ; volta ao topo do loop, até zerar o contador ou satisfazer algum salto

compstring_acabado:
    sub al, bl          ; subtrai a1 - b1
    movsx rax, al       ; como o al (1byte) vai pra rax (64 bits), será usada movsx que é melhor pra caso de passar o valor de um menor pra um maior registrador
    ret                 ; retorna pra proxima linha de onde teve a ultima 'call'

