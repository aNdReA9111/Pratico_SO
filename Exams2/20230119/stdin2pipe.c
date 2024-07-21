#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

#define BUFFER_SIZE 1024

int main() {
    char cmd1[BUFFER_SIZE];
    char cmd2[BUFFER_SIZE];

    // Leggi i due comandi 
    if (fgets(cmd1, sizeof(cmd1), stdin) == NULL) {
        perror("fgets");
        exit(EXIT_FAILURE);
    }
    if (fgets(cmd2, sizeof(cmd2), stdin) == NULL) {
        perror("fgets");
        exit(EXIT_FAILURE);
    }

    // Rimuovi il newline alla fine dei comandi
    cmd1[strcspn(cmd1, "\n")] = '\0';
    cmd2[strcspn(cmd2, "\n")] = '\0';

    // Tokenizzazione dei comandi
    char *argv1[BUFFER_SIZE];
    char *argv2[BUFFER_SIZE];

    int i = 0;

    argv1[i] = strtok(cmd1, " ");
    while (argv1[i] != NULL) {
        i++;
        argv1[i] = strtok(NULL, " ");
    }

    i = 0;
    argv2[i] = strtok(cmd2, " ");
    while (argv2[i] != NULL) {
        i++;
        argv2[i] = strtok(NULL, " ");
    }

    // apro la pipe per la comunicazione dei comandi
    int pipefd[2];
    if (pipe(pipefd) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    pid_t pid1 = fork();
    if (pid1 == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (pid1 == 0) {
        // processso figlio
        close(pipefd[0]); // Close unused read end

        // viene duplicato sul file descriptor STDOUT_FILENO. Da questo momento in poi, 
        // ogni operazione di output standard (printf, puts, ecc.) sarà reindirizzata 
        // all'estremità di scrittura della pipe rappresentata da pipefd[1]
        dup2(pipefd[1], STDOUT_FILENO);
        close(pipefd[1]);

        execvp(argv1[0], argv1);
        perror("execvp");
        exit(EXIT_FAILURE);
    }

    pid_t pid2 = fork();
    if (pid2 == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (pid2 == 0) {
        // Child process for the second command
        close(pipefd[1]); // Close unused write end
        dup2(pipefd[0], STDIN_FILENO);
        close(pipefd[0]);

        execvp(argv2[0], argv2);
        perror("execvp");
        exit(EXIT_FAILURE);
    }

    // Chiudo le pipe
    close(pipefd[0]);
    close(pipefd[1]);

    // Aspetto che i due processi figli terminino
    waitpid(pid1, NULL, 0);
    waitpid(pid2, NULL, 0);

    return 0;
}
