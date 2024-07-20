I segnali (signals) sono uno strumento di comunicazione interprocesso utilizzato in Unix e nei sistemi operativi Unix-like, come Linux. I segnali consentono ai processi di notificare eventi importanti l'uno all'altro, come interruzioni, errori o la terminazione di un processo.

Ecco una panoramica dei segnali e delle funzioni correlate, inclusi `signal`, `sigaction`, e altre chiamate di sistema utili.

### Segnali Comuni

Alcuni segnali comuni e il loro significato:

- `SIGINT`: Interruzione (Ctrl+C)
- `SIGTERM`: Richiesta di terminazione
- `SIGKILL`: Terminazione forzata
- `SIGSEGV`: Violazione di segmentazione (errore di memoria)
- `SIGALRM`: Timer scaduto
- `SIGCHLD`: Figlio terminato

### `signal`

La funzione `signal` è una chiamata di sistema semplice per gestire i segnali. Consente di specificare una funzione di gestione per un determinato segnale.

```c
#include <stdio.h>
#include <signal.h>

void handle_signal(int signal) {
    printf("Caught signal %d\n", signal);
}

int main() {
    signal(SIGINT, handle_signal);
    while (1) {
        printf("Running...\n");
        sleep(1);
    }
    return 0;
}
```

### `sigaction`

`sigaction` è una chiamata di sistema più avanzata e flessibile per la gestione dei segnali. Consente di specificare una struttura di gestione del segnale con opzioni aggiuntive.

```c
#include <stdio.h>
#include <signal.h>
#include <unistd.h>

void handle_signal(int signal) {
    printf("Caught signal %d\n", signal);
}

int main() {
    struct sigaction sa;
    sa.sa_handler = handle_signal;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;

    sigaction(SIGINT, &sa, NULL);

    while (1) {
        printf("Running...\n");
        sleep(1);
    }
    return 0;
}
```

### `sigprocmask`

`sigprocmask` è usato per bloccare o sbloccare segnali. Questo è utile quando si desidera evitare che determinati segnali vengano gestiti durante sezioni critiche del codice.

```c
#include <stdio.h>
#include <signal.h>
#include <unistd.h>

void handle_signal(int signal) {
    printf("Caught signal %d\n", signal);
}

int main() {
    struct sigaction sa;
    sa.sa_handler = handle_signal;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;

    sigaction(SIGINT, &sa, NULL);

    sigset_t new_set, old_set;
    sigemptyset(&new_set);
    sigaddset(&new_set, SIGINT);

    sigprocmask(SIG_BLOCK, &new_set, &old_set);

    printf("SIGINT blocked for 5 seconds\n");
    sleep(5);

    sigprocmask(SIG_SETMASK, &old_set, NULL);
    printf("SIGINT unblocked\n");

    while (1) {
        printf("Running...\n");
        sleep(1);
    }
    return 0;
}
```

### `kill`

La chiamata di sistema `kill` è usata per inviare segnali a un processo.

```c
#include <stdio.h>
#include <signal.h>
#include <unistd.h>

int main() {
    pid_t pid = fork();
    if (pid == 0) {
        // Processo figlio
        while (1) {
            printf("Child running...\n");
            sleep(1);
        }
    } else {
        // Processo padre
        sleep(3);
        kill(pid, SIGINT);
        wait(NULL); // Attende che il processo figlio termini
        printf("Child process terminated\n");
    }
    return 0;
}
```

### `raise`

`raise` invia un segnale al processo chiamante.

```c
#include <stdio.h>
#include <signal.h>

void handle_signal(int signal) {
    printf("Caught signal %d\n", signal);
}

int main() {
    signal(SIGINT, handle_signal);
    printf("Raising SIGINT\n");
    raise(SIGINT);
    return 0;
}
```

### `alarm`

`alarm` imposta un timer che invia un segnale `SIGALRM` al processo dopo un numero specificato di secondi.

```c
#include <stdio.h>
#include <signal.h>
#include <unistd.h>

void handle_signal(int signal) {
    if (signal == SIGALRM) {
        printf("Timer expired\n");
    }
}

int main() {
    signal(SIGALRM, handle_signal);
    alarm(5); // Imposta un timer di 5 secondi
    while (1) {
        printf("Running...\n");
        sleep(1);
    }
    return 0;
}
```

### `pause`

`pause` sospende l'esecuzione del processo fino a quando non viene ricevuto un segnale.

```c
#include <stdio.h>
#include <signal.h>
#include <unistd.h>

void handle_signal(int signal) {
    printf("Caught signal %d\n", signal);
}

int main() {
    signal(SIGINT, handle_signal);
    printf("Waiting for signal...\n");
    pause(); // Attende un segnale
    printf("Signal received, exiting...\n");
    return 0;
}
```

### Segnali in Tempo Reale

I segnali in tempo reale (real-time signals) forniscono segnali aggiuntivi (da `SIGRTMIN` a `SIGRTMAX`) che possono essere utilizzati per la comunicazione tra processi con priorità e informazioni aggiuntive.

```c
#include <stdio.h>
#include <signal.h>
#include <unistd.h>

void handle_rt_signal(int signal, siginfo_t *info, void *context) {
    printf("Caught real-time signal %d with value %d\n", signal, info->si_value.sival_int);
}

int main() {
    struct sigaction sa;
    sa.sa_sigaction = handle_rt_signal;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_SIGINFO;

    sigaction(SIGRTMIN, &sa, NULL);

    union sigval value;
    value.sival_int = 42;
    sigqueue(getpid(), SIGRTMIN, value);

    sleep(1); // Attende che il segnale venga gestito

    return 0;
}
```