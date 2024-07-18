`inotify` è un'API del kernel di Linux che fornisce un'interfaccia per monitorare i cambiamenti nel file system, come la creazione, la modifica, l'eliminazione di file e directory. Permette di ricevere notifiche in tempo reale su questi eventi, il che è utile per molte applicazioni, come il monitoraggio dei log, la sincronizzazione dei file, la gestione degli eventi di sistema e altro.

### Descrizione di `inotify`

Le principali funzioni di `inotify` sono:

1. **`inotify_init` / `inotify_init1`**: Creano un'istanza di inotify.
2. **`inotify_add_watch`**: Aggiungono una watch per un file o una directory.
3. **`inotify_rm_watch`**: Rimuovono una watch.

### Esempi di utilizzo di `inotify`

Ecco un esempio di codice che utilizza `inotify` per monitorare vari eventi nel file system:

```c
#include <stdio.h>
#include <stdlib.h>
#include <sys/inotify.h>
#include <unistd.h>
#include <limits.h>
#include <errno.h>
#include <string.h>

void handle_events(int fd);

int main() {
    // Crea un'istanza di inotify
    int fd = inotify_init();
    if (fd < 0) {
        perror("inotify_init");
        exit(EXIT_FAILURE);
    }

    // Aggiungi una watch su una directory
    int wd = inotify_add_watch(fd, "/path/to/dir", IN_CREATE | IN_DELETE | IN_MODIFY);
    if (wd < 0) {
        perror("inotify_add_watch");
        close(fd);
        exit(EXIT_FAILURE);
    }

    // Gestisci gli eventi
    handle_events(fd);

    // Rimuovi la watch e chiudi l'istanza di inotify
    inotify_rm_watch(fd, wd);
    close(fd);

    return 0;
}

void handle_events(int fd) {
    char buffer[4096]
        __attribute__ ((aligned(__alignof__(struct inotify_event))));
    const struct inotify_event *event;
    ssize_t len;
    char *ptr;

    // Leggi gli eventi
    while ((len = read(fd, buffer, sizeof(buffer))) > 0) {
        for (ptr = buffer; ptr < buffer + len;
             ptr += sizeof(struct inotify_event) + event->len) {
            event = (const struct inotify_event *) ptr;

            if (event->mask & IN_CREATE) {
                printf("File %s created.\n", event->name);
            } else if (event->mask & IN_DELETE) {
                printf("File %s deleted.\n", event->name);
            } else if (event->mask & IN_MODIFY) {
                printf("File %s modified.\n", event->name);
            }
        }
    }

    if (len == -1 && errno != EAGAIN) {
        perror("read");
    }
}
```

### Esempi di utilizzo specifici

1. **Monitorare la creazione di file in una directory:**

    ```c
    int wd = inotify_add_watch(fd, "/path/to/dir", IN_CREATE);
    ```

    Questo esempio monitora la creazione di file nella directory specificata.

2. **Monitorare la modifica di file in una directory:**

    ```c
    int wd = inotify_add_watch(fd, "/path/to/dir", IN_MODIFY);
    ```

    Questo esempio monitora le modifiche ai file nella directory specificata.

3. **Monitorare la cancellazione di file in una directory:**

    ```c
    int wd = inotify_add_watch(fd, "/path/to/dir", IN_DELETE);
    ```

    Questo esempio monitora la cancellazione di file nella directory specificata.

4. **Monitorare l'apertura di file in una directory:**

    ```c
    int wd = inotify_add_watch(fd, "/path/to/dir", IN_OPEN);
    ```

    Questo esempio monitora l'apertura di file nella directory specificata.

5. **Monitorare la chiusura di file in una directory:**

    ```c
    int wd = inotify_add_watch(fd, "/path/to/dir", IN_CLOSE);
    ```

    Questo esempio monitora la chiusura di file nella directory specificata.

6. **Monitorare la chiusura di file dopo modifica in una directory:**

    ```c
    int wd = inotify_add_watch(fd, "/path/to/dir", IN_CLOSE_WRITE);
    ```

    Questo esempio monitora la chiusura di file dopo una modifica nella directory specificata.

7. **Monitorare la chiusura di file senza modifica in una directory:**

    ```c
    int wd = inotify_add_watch(fd, "/path/to/dir", IN_CLOSE_NOWRITE);
    ```

    Questo esempio monitora la chiusura di file senza modifiche nella directory specificata.

8. **Monitorare la rinomina di file in una directory:**

    ```c
    int wd = inotify_add_watch(fd, "/path/to/dir", IN_MOVE);
    ```

    Questo esempio monitora la rinomina di file nella directory specificata.

9. **Monitorare la lettura di file in una directory:**

    ```c
    int wd = inotify_add_watch(fd, "/path/to/dir", IN_ACCESS);
    ```

    Questo esempio monitora l'accesso in lettura ai file nella directory specificata.

10. **Monitorare i cambiamenti negli attributi di file in una directory:**

    ```c
    int wd = inotify_add_watch(fd, "/path/to/dir", IN_ATTRIB);
    ```

    Questo esempio monitora i cambiamenti negli attributi dei file (come i permessi) nella directory specificata.

### Esempio Completo per Monitorare Diversi Eventi

Di seguito è riportato un esempio completo che monitora la creazione, la modifica e la cancellazione di file in una directory:

```c
#include <stdio.h>
#include <stdlib.h>
#include <sys/inotify.h>
#include <unistd.h>
#include <limits.h>
#include <errno.h>
#include <string.h>

void handle_events(int fd);

int main() {
    // Crea un'istanza di inotify
    int fd = inotify_init();
    if (fd < 0) {
        perror("inotify_init");
        exit(EXIT_FAILURE);
    }

    // Aggiungi una watch su una directory
    int wd = inotify_add_watch(fd, "/path/to/dir", IN_CREATE | IN_DELETE | IN_MODIFY);
    if (wd < 0) {
        perror("inotify_add_watch");
        close(fd);
        exit(EXIT_FAILURE);
    }

    // Gestisci gli eventi
    handle_events(fd);

    // Rimuovi la watch e chiudi l'istanza di inotify
    inotify_rm_watch(fd, wd);
    close(fd);

    return 0;
}

void handle_events(int fd) {
    char buffer[4096]
        __attribute__ ((aligned(__alignof__(struct inotify_event))));
    const struct inotify_event *event;
    ssize_t len;
    char *ptr;

    // Leggi gli eventi
    while ((len = read(fd, buffer, sizeof(buffer))) > 0) {
        for (ptr = buffer; ptr < buffer + len;
             ptr += sizeof(struct inotify_event) + event->len) {
            event = (const struct inotify_event *) ptr;

            if (event->mask & IN_CREATE) {
                printf("File %s created.\n", event->name);
            } else if (event->mask & IN_DELETE) {
                printf("File %s deleted.\n", event->name);
            } else if (event->mask & IN_MODIFY) {
                printf("File %s modified.\n", event->name);
            }
        }
    }

    if (len == -1 && errno != EAGAIN) {
        perror("read");
    }
}
```

In questo esempio, il programma crea un'istanza di `inotify`, aggiunge una watch su una directory per monitorare la creazione, la modifica e la cancellazione di file, gestisce gli eventi leggendo dal file descriptor di `inotify` e stampa un messaggio quando si verifica uno degli eventi monitorati.

Ecco una lista delle costanti che possono essere monitorate con `inotify` insieme alle rispettive maschere:

- **`IN_ACCESS`**: File is accessed.
- **`IN_MODIFY`**: File is modified.
- **`IN_ATTRIB`**: Metadata changed (permissions, timestamps, extended attributes, etc.).
- **`IN_CLOSE_WRITE`**: File opened for writing is closed.
- **`IN_CLOSE_NOWRITE`**: File not opened for writing is closed.
- **`IN_OPEN`**: File is opened.
- **`IN_MOVED_FROM`**: File/directory is moved from the watched directory.
- **`IN_MOVED_TO`**: File/directory is moved to the watched directory.
- **`IN_CREATE`**: File/directory is created in the watched directory.
- **`IN_DELETE`**: File/directory is deleted from the watched directory.
- **`IN_DELETE_SELF`**: Watched file/directory itself is deleted.
- **`IN_MOVE_SELF`**: Watched file/directory itself is moved.
- **`IN_UNMOUNT`**: Filesystem containing watched object is unmounted.
- **`IN_Q_OVERFLOW`**: Event queue overflowed.
- **`IN_IGNORED`**: Watch was removed explicitly or automatically.
- **`IN_ISDIR`**: Subject of this event is a directory.
- **`IN_ONLYDIR`**: Only watch the path if it is a directory.
- **`IN_DONT_FOLLOW`**: Do not follow a symbolic link.
- **`IN_EXCL_UNLINK`**: Do not generate events for children after they have been unlinked from the watched directory.
- **`IN_MASK_ADD`**: Add to the mask of an already existing watch.
- **`IN_ONESHOT`**: Monitor the object only once.
- **`IN_ALL_EVENTS`**: Bitmask of all the above events.

Puoi monitorare diversi file o directory per vari eventi simultaneamente usando diversi watch, ognuno dei quali è associato a un file descriptor `inotify`, che può monitorare vari eventi su più file o directory.

Ecco come puoi fare per aggiungere più watch:

### Esempio di codice

Questo esempio mostra come aggiungere più watch per monitorare diversi eventi su diverse directory:

```c
#include <stdio.h>
#include <stdlib.h>
#include <sys/inotify.h>
#include <unistd.h>
#include <limits.h>
#include <errno.h>
#include <string.h>

void handle_events(int fd);

int main() {
    // Crea un'istanza di inotify
    int fd = inotify_init();
    if (fd < 0) {
        perror("inotify_init");
        exit(EXIT_FAILURE);
    }

    // Aggiungi una watch per la creazione e cancellazione di file nella directory 1
    int wd1 = inotify_add_watch(fd, "/path/to/dir1", IN_CREATE | IN_DELETE);
    if (wd1 < 0) {
        perror("inotify_add_watch /path/to/dir1");
        close(fd);
        exit(EXIT_FAILURE);
    }

    // Aggiungi una watch per la modifica di file nella directory 2
    int wd2 = inotify_add_watch(fd, "/path/to/dir2", IN_MODIFY);
    if (wd2 < 0) {
        perror("inotify_add_watch /path/to/dir2");
        inotify_rm_watch(fd, wd1);
        close(fd);
        exit(EXIT_FAILURE);
    }

    // Aggiungi una watch per l'apertura di file nella directory 3
    int wd3 = inotify_add_watch(fd, "/path/to/dir3", IN_OPEN);
    if (wd3 < 0) {
        perror("inotify_add_watch /path/to/dir3");
        inotify_rm_watch(fd, wd1);
        inotify_rm_watch(fd, wd2);
        close(fd);
        exit(EXIT_FAILURE);
    }

    // Gestisci gli eventi
    handle_events(fd);

    // Rimuovi le watch e chiudi l'istanza di inotify
    inotify_rm_watch(fd, wd1);
    inotify_rm_watch(fd, wd2);
    inotify_rm_watch(fd, wd3);
    close(fd);

    return 0;
}

void handle_events(int fd) {
    char buffer[4096]
        __attribute__ ((aligned(__alignof__(struct inotify_event))));
    const struct inotify_event *event;
    ssize_t len;
    char *ptr;

    // Leggi gli eventi
    while ((len = read(fd, buffer, sizeof(buffer))) > 0) {
        for (ptr = buffer; ptr < buffer + len;
             ptr += sizeof(struct inotify_event) + event->len) {
            event = (const struct inotify_event *) ptr;

            if (event->mask & IN_CREATE) {
                printf("File %s created in watched directory.\n", event->name);
            } else if (event->mask & IN_DELETE) {
                printf("File %s deleted from watched directory.\n", event->name);
            } else if (event->mask & IN_MODIFY) {
                printf("File %s modified in watched directory.\n", event->name);
            } else if (event->mask & IN_OPEN) {
                printf("File %s opened in watched directory.\n", event->name);
            }
        }
    }

    if (len == -1 && errno != EAGAIN) {
        perror("read");
    }
}
```

### Spiegazione del codice

- **Creazione dell'istanza `inotify`**: La funzione `inotify_init` crea una nuova istanza di `inotify`.
- **Aggiunta di più watch**: Utilizziamo `inotify_add_watch` per aggiungere watch su diverse directory, ognuna monitorando eventi specifici.
    - `wd1` monitora la creazione e cancellazione di file in `/path/to/dir1`.
    - `wd2` monitora la modifica di file in `/path/to/dir2`.
    - `wd3` monitora l'apertura di file in `/path/to/dir3`.
- **Gestione degli eventi**: La funzione `handle_events` legge gli eventi dal file descriptor `inotify` e gestisce ogni tipo di evento registrato.
- **Rimozione delle watch e chiusura dell'istanza `inotify`**: Le watch vengono rimosse con `inotify_rm_watch` e l'istanza di `inotify` viene chiusa con `close`.

Puoi aggiungere ulteriori watch per altri file o directory e monitorare diversi eventi secondo le tue necessità, rendendo `inotify` uno strumento molto flessibile per il monitoraggio del file system.

La struttura `inotify_event` viene utilizzata per descrivere un singolo evento generato da `inotify`. Quando si leggono gli eventi da un file descriptor `inotify`, i dati vengono restituiti sotto forma di array di queste strutture.

### Definizione della Struttura `inotify_event`

La struttura `inotify_event` è definita nel file di intestazione `<sys/inotify.h>` e ha il seguente formato:

```c
struct inotify_event {
    int      wd;       // Watch descriptor
    uint32_t mask;     // Mask of events
    uint32_t cookie;   // Unique cookie associating related events (for rename)
    uint32_t len;      // Length (including nulls) of the name field
    char     name[];   // Optional null-terminated name
};
```

### Descrizione dei Campi

- **`wd`**: Il watch descriptor, un identificatore univoco restituito da `inotify_add_watch` quando viene aggiunta una watch. Serve per identificare quale watch ha generato l'evento.
- **`mask`**: Una maschera di bit che descrive l'evento che si è verificato. Questa maschera può includere vari flag che descrivono il tipo di evento (come `IN_CREATE`, `IN_DELETE`, ecc.).
- **`cookie`**: Un valore unico utilizzato per associare eventi correlati tra loro, come le operazioni di rinomina (`IN_MOVED_FROM` e `IN_MOVED_TO`).
- **`len`**: La lunghezza del campo `name`, inclusi eventuali caratteri nulli.
- **`name`**: Un array opzionale di caratteri che contiene il nome del file o della directory che ha generato l'evento. Questo campo è presente solo se l'evento riguarda un file o una directory all'interno di una directory monitorata.

### Esempio di Utilizzo

Ecco un esempio di come leggere e gestire gli eventi `inotify` utilizzando la struttura `inotify_event`:

```c
#include <stdio.h>
#include <stdlib.h>
#include <sys/inotify.h>
#include <unistd.h>
#include <limits.h>
#include <errno.h>
#include <string.h>

void handle_events(int fd);

int main() {
    // Crea un'istanza di inotify
    int fd = inotify_init();
    if (fd < 0) {
        perror("inotify_init");
        exit(EXIT_FAILURE);
    }

    // Aggiungi una watch su una directory
    int wd = inotify_add_watch(fd, "/path/to/dir", IN_CREATE | IN_DELETE | IN_MODIFY);
    if (wd < 0) {
        perror("inotify_add_watch");
        close(fd);
        exit(EXIT_FAILURE);
    }

    // Gestisci gli eventi
    handle_events(fd);

    // Rimuovi la watch e chiudi l'istanza di inotify
    inotify_rm_watch(fd, wd);
    close(fd);

    return 0;
}

void handle_events(int fd) {
    char buffer[4096]
        __attribute__ ((aligned(__alignof__(struct inotify_event))));
    const struct inotify_event *event;
    ssize_t len;
    char *ptr;

    // Leggi gli eventi
    while ((len = read(fd, buffer, sizeof(buffer))) > 0) {
        for (ptr = buffer; ptr < buffer + len;
             ptr += sizeof(struct inotify_event) + event->len) {
            event = (const struct inotify_event *) ptr;

            // Stampa l'evento
            printf("Event on watch descriptor %d: ", event->wd);
            if (event->mask & IN_CREATE) {
                printf("File %s created.\n", event->name);
            } else if (event->mask & IN_DELETE) {
                printf("File %s deleted.\n", event->name);
            } else if (event->mask & IN_MODIFY) {
                printf("File %s modified.\n", event->name);
            }
        }
    }

    if (len == -1 && errno != EAGAIN) {
        perror("read");
    }
}
```

### Spiegazione del Codice

1. **Creazione dell'istanza `inotify`**: La funzione `inotify_init` crea un nuovo file descriptor `inotify`.
2. **Aggiunta di una watch**: `inotify_add_watch` aggiunge una watch sulla directory specificata per monitorare eventi di creazione, modifica e cancellazione di file.
3. **Gestione degli eventi**: La funzione `handle_events` legge gli eventi dal file descriptor `inotify` e li processa:
   - **Lettura degli eventi**: Gli eventi vengono letti in un buffer.
   - **Iterazione sugli eventi**: Ogni evento viene analizzato utilizzando un puntatore alla struttura `inotify_event`.
   - **Stampa degli eventi**: Gli eventi di creazione, modifica e cancellazione vengono stampati sulla console.

Questo esempio mostra come utilizzare `inotify` per monitorare eventi specifici su file e directory, utilizzando la struttura `inotify_event` per interpretare e gestire gli eventi generati.