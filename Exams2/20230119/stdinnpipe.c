#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

#define BUFFER_SIZE 1024
#define LINE_SIZE 128

// Funzione per la tokenizzazione di una stringa in un array di stringhe con NULL alla fine
void tokenize(char *line, char *argv[], int *len) {
    *len = 0;
    int in_quotes = 0;
    char *token_start = line;
    char *token;

    for (char *p = line; ; p++) {
        if (*p == '\'' || *p == '\"') {
            in_quotes = !in_quotes;
            continue;
        }
        
        if ((*p == ' ' && !in_quotes) || *p == '\0') {
            if (p > token_start) {
                size_t token_length = p - token_start;
                token = (char *)malloc(token_length + 1);
                strncpy(token, token_start, token_length);
                token[token_length] = '\0';
                argv[*len] = token;
                (*len)++;
            }
            if (*p == '\0') break;
            token_start = p + 1;
        }
    }
    argv[*len] = NULL;  // Aggiunge NULL alla fine dell'array argv
}

int main() {
    char *cmd[BUFFER_SIZE];
    char line[LINE_SIZE];
    size_t cmd_len = 0;

    // leggo i comandi dal file e li ripongo in un array di stringhe
    while (fgets(line, sizeof(line), stdin))
    {
        line[strcspn(line, "\n")] = '\0';
        cmd[cmd_len] = malloc(strlen(line) + 1);
        strcpy(cmd[cmd_len++], line);
    }

    // apro la pipe per la comunicazione dei comandi
    int pipefd[2];
    if (pipe(pipefd) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    // proseguo con la tokenizzazione dei comandi e la creazione dei processi
    char *argv[BUFFER_SIZE];
    int len;
    for (size_t i = 0; i < cmd_len; i++)
    {
        len = 0;
        tokenize(cmd[i], argv, &len);

        size_t j = 0;
        while (argv[j] != NULL)
        {
            printf("%s\n ", argv[j++]);
        }
        
        // comincio a creare i processi
        pid_t pid;

        switch (pid = fork())
        {
            case -1:
                perror("fork");
                _exit(EXIT_FAILURE);

            case 0:
                // processo figlio
                if (i == 0)
                // viene duplicato sul file descriptor STDOUT_FILENO. Da questo momento in poi, 
                // ogni operazione di output standard (printf, puts, ecc.) sarà reindirizzata 
                // all'estremità di scrittura della pipe rappresentata da pipefd[1]
                // dup2(pipefd[1], STDOUT_FILENO);
                {
                    // comandi dal primo al penultimo
                    close(pipefd[0]);               // chiudo la lettura
                    dup2(pipefd[1], STDOUT_FILENO); // metto stdout in pipefd[1]
                    close(pipefd[1]);               // chiudo pipefd[1] e verrà usato come stdout
                }
                else if (i == cmd_len - 1)
                {
                    // ultimo comando
                    close(pipefd[1]);               // chiudo la scrittura
                    dup2(pipefd[0], STDIN_FILENO);
                    close(pipefd[0]);
                } 

                execvp(argv[0], argv); 

                // se arrivo qui c'è stato un errore
                perror("execvp");
                _exit(EXIT_FAILURE);          
            
            default:
                wait(NULL);
                break;  
        }
    }

    printf("Processo padre\n");
    // comando intermedio
    close(pipefd[0]); // Close unused read end
    close(pipefd[1]); // Close unused write end

    return EXIT_SUCCESS;
}
