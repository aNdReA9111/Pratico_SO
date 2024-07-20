Le pipe sono un meccanismo di comunicazione interprocesso (IPC) in Unix e nei sistemi Unix-like che permette a un processo di mandare dati a un altro processo. Le pipe sono spesso usate per collegare l'output di un programma all'input di un altro, permettendo ai processi di lavorare insieme in modo sequenziale.

### Creazione e Utilizzo di una Pipe

La funzione `pipe()` crea una pipe unidirezionale, rappresentata da un array di due file descriptor:
- `pipefd[0]`: File descriptor per la lettura.
- `pipefd[1]`: File descriptor per la scrittura.

La firma della funzione `pipe()` è:

```c
int pipe(int pipefd[2]);
```

### Esempio di Utilizzo di Pipe

In questo esempio, un processo padre crea una pipe, scrive un messaggio nella pipe, e il processo figlio legge il messaggio dalla pipe.

```c
#include <stdio.h>
#include <unistd.h>
#include <string.h>

int main() {
    int pipefd[2];
    pid_t pid;
    char buffer[128];

    // Creazione della pipe
    if (pipe(pipefd) == -1) {
        perror("pipe");
        return 1;
    }

    // Creazione del processo figlio
    pid = fork();
    if (pid == -1) {
        perror("fork");
        return 1;
    }

    if (pid == 0) {
        // Codice del processo figlio
        close(pipefd[1]); // Chiude il lato scrittura della pipe
        read(pipefd[0], buffer, sizeof(buffer)); // Legge dalla pipe
        printf("Child received: %s\n", buffer);
        close(pipefd[0]); // Chiude il lato lettura della pipe
    } else {
        // Codice del processo padre
        close(pipefd[0]); // Chiude il lato lettura della pipe
        const char *message = "Hello from parent";
        write(pipefd[1], message, strlen(message) + 1); // Scrive nella pipe
        close(pipefd[1]); // Chiude il lato scrittura della pipe
        wait(NULL); // Attende che il processo figlio termini
    }

    return 0;
}
```

### Output dell'Esempio

Quando esegui questo programma, vedrai l'output:

```
Child received: Hello from parent
```

### Descrizione delle Funzioni e delle System Call Utili

1. **pipe()**
    - Crea una pipe unidirezionale.
    - `int pipe(int pipefd[2]);`

2. **fork()**
    - Crea un nuovo processo (figlio) duplicando il processo chiamante (padre).
    - `pid_t fork();`

3. **close()**
    - Chiude un file descriptor, rilasciando le risorse ad esso associate.
    - `int close(int fd);`

4. **read()**
    - Legge dati da un file descriptor.
    - `ssize_t read(int fd, void *buf, size_t count);`

5. **write()**
    - Scrive dati su un file descriptor.
    - `ssize_t write(int fd, const void *buf, size_t count);`

6. **wait()**
    - Attende che un processo figlio termini.
    - `pid_t wait(int *status);`

### Utilizzo delle Pipe per la Comunicazione Bidirezionale

Per la comunicazione bidirezionale tra due processi, è necessario creare due pipe: una per la comunicazione dal padre al figlio e una per la comunicazione dal figlio al padre.

```c
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

int main() {
    int pipefd1[2]; // Pipe dal padre al figlio
    int pipefd2[2]; // Pipe dal figlio al padre
    pid_t pid;
    char buffer[128];

    // Creazione delle pipe
    if (pipe(pipefd1) == -1 || pipe(pipefd2) == -1) {
        perror("pipe");
        return 1;
    }

    // Creazione del processo figlio
    pid = fork();
    if (pid == -1) {
        perror("fork");
        return 1;
    }

    if (pid == 0) {
        // Codice del processo figlio
        close(pipefd1[1]); // Chiude il lato scrittura della pipe1
        close(pipefd2[0]); // Chiude il lato lettura della pipe2

        // Legge dal padre
        read(pipefd1[0], buffer, sizeof(buffer));
        printf("Child received: %s\n", buffer);

        // Risponde al padre
        const char *response = "Hello from child";
        write(pipefd2[1], response, strlen(response) + 1);

        close(pipefd1[0]); // Chiude il lato lettura della pipe1
        close(pipefd2[1]); // Chiude il lato scrittura della pipe2
    } else {
        // Codice del processo padre
        close(pipefd1[0]); // Chiude il lato lettura della pipe1
        close(pipefd2[1]); // Chiude il lato scrittura della pipe2

        // Scrive al figlio
        const char *message = "Hello from parent";
        write(pipefd1[1], message, strlen(message) + 1);

        // Legge la risposta del figlio
        read(pipefd2[0], buffer, sizeof(buffer));
        printf("Parent received: %s\n", buffer);

        close(pipefd1[1]); // Chiude il lato scrittura della pipe1
        close(pipefd2[0]); // Chiude il lato lettura della pipe2

        wait(NULL); // Attende che il processo figlio termini
    }

    return 0;
}
```

### Output dell'Esempio

Quando esegui questo programma, vedrai l'output:

```
Child received: Hello from parent
Parent received: Hello from child
```

### Considerazioni

- Le pipe sono un mezzo semplice ed efficace per la comunicazione tra processi in Unix.
- Le pipe sono unidirezionali; per la comunicazione bidirezionale, sono necessarie due pipe.
- È importante chiudere i file descriptor non necessari per evitare deadlock e perdite di risorse.
- Le pipe sono generalmente utilizzate per la comunicazione tra processi correlati (ad esempio, padre-figlio). Per la comunicazione tra processi non correlati, è possibile utilizzare named pipes (FIFO) o altri meccanismi IPC come le code di messaggi, i semafori e la memoria condivisa.

Questi esempi coprono l'uso di base delle pipe in C e mostrano come utilizzarle per la comunicazione interprocesso, sia in modo unidirezionale che bidirezionale.