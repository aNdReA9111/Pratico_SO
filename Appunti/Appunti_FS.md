Ecco un elenco di 20 syscall comunemente utilizzate in C per lavorare su file system, inclusi esempi di utilizzo e brevi commenti sul codice.

### 1. `open`
Apre un file.
```c
#include <fcntl.h>
#include <unistd.h>

int main() {
    int fd = open("file.txt", O_RDONLY);
    if (fd == -1) {
        perror("open");
        return 1;
    }
    close(fd);
    return 0;
}
```

### 2. `close`
Chiude un file descriptor.
```c
#include <unistd.h>

int main() {
    int fd = open("file.txt", O_RDONLY);
    if (fd == -1) {
        perror("open");
        return 1;
    }
    close(fd);
    return 0;
}
```

### 3. `read`
Legge dati da un file.
```c
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

int main() {
    int fd = open("file.txt", O_RDONLY);
    if (fd == -1) {
        perror("open");
        return 1;
    }
    char buffer[100];
    ssize_t bytesRead = read(fd, buffer, sizeof(buffer) - 1);
    if (bytesRead == -1) {
        perror("read");
        close(fd);
        return 1;
    }
    buffer[bytesRead] = '\0';
    printf("Read: %s\n", buffer);
    close(fd);
    return 0;
}
```

### 4. `write`
Scrive dati su un file.
```c
#include <fcntl.h>
#include <unistd.h>

int main() {
    int fd = open("file.txt", O_WRONLY | O_CREAT, 0644);
    if (fd == -1) {
        perror("open");
        return 1;
    }
    const char *text = "Hello, World!";
    if (write(fd, text, strlen(text)) == -1) {
        perror("write");
        close(fd);
        return 1;
    }
    close(fd);
    return 0;
}
```

### 5. `lseek`
Cambia la posizione del file offset.
```c
#include <fcntl.h>
#include <unistd.h>

int main() {
    int fd = open("file.txt", O_RDONLY);
    if (fd == -1) {
        perror("open");
        return 1;
    }
    off_t offset = lseek(fd, 10, SEEK_SET);
    if (offset == -1) {
        perror("lseek");
        close(fd);
        return 1;
    }
    close(fd);
    return 0;
}
```

### 6. `stat`
Ottiene informazioni su un file.
```c
#include <sys/stat.h>
#include <stdio.h>

int main() {
    struct stat st;
    if (stat("file.txt", &st) == -1) {
        perror("stat");
        return 1;
    }
    printf("File size: %ld\n", st.st_size);
    return 0;
}
```

### 7. `fstat`
Ottiene informazioni su un file da un file descriptor.
```c
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

int main() {
    int fd = open("file.txt", O_RDONLY);
    if (fd == -1) {
        perror("open");
        return 1;
    }
    struct stat st;
    if (fstat(fd, &st) == -1) {
        perror("fstat");
        close(fd);
        return 1;
    }
    printf("File size: %ld\n", st.st_size);
    close(fd);
    return 0;
}
```

### 8. `mkdir`
Crea una directory.
```c
#include <sys/stat.h>

int main() {
    if (mkdir("newdir", 0755) == -1) {
        perror("mkdir");
        return 1;
    }
    return 0;
}
```

### 9. `rmdir`
Rimuove una directory.
```c
#include <unistd.h>

int main() {
    if (rmdir("newdir") == -1) {
        perror("rmdir");
        return 1;
    }
    return 0;
}
```

### 10. `unlink`
Rimuove un file.
```c
#include <unistd.h>

int main() {
    if (unlink("file.txt") == -1) {
        perror("unlink");
        return 1;
    }
    return 0;
}
```

### 11. `rename`
Rinomina un file o una directory.
```c
#include <stdio.h>

int main() {
    if (rename("oldfile.txt", "newfile.txt") == -1) {
        perror("rename");
        return 1;
    }
    return 0;
}
```

### 12. `chmod`
Cambia i permessi di un file.
```c
#include <sys/stat.h>

int main() {
    if (chmod("file.txt", 0644) == -1) {
        perror("chmod");
        return 1;
    }
    return 0;
}
```

### 13. `chown`
Cambia il proprietario di un file.
```c
#include <unistd.h>

int main() {
    if (chown("file.txt", 1000, 1000) == -1) {
        perror("chown");
        return 1;
    }
    return 0;
}
```

### 14. `ftruncate`
Tronca un file a una dimensione specificata.
```c
#include <unistd.h>
#include <fcntl.h>

int main() {
    int fd = open("file.txt", O_WRONLY);
    if (fd == -1) {
        perror("open");
        return 1;
    }
    if (ftruncate(fd, 100) == -1) {
        perror("ftruncate");
        close(fd);
        return 1;
    }
    close(fd);
    return 0;
}
```

### 15. `fsync`
Sincronizza un file con il disco.
```c
#include <unistd.h>
#include <fcntl.h>

int main() {
    int fd = open("file.txt", O_WRONLY);
    if (fd == -1) {
        perror("open");
        return 1;
    }
    if (fsync(fd) == -1) {
        perror("fsync");
        close(fd);
        return 1;
    }
    close(fd);
    return 0;
}
```

### 16. `opendir`
Apre una directory.
```c
#include <dirent.h>
#include <stdio.h>

int main() {
    DIR *dir = opendir(".");
    if (dir == NULL) {
        perror("opendir");
        return 1;
    }
    closedir(dir);
    return 0;
}
```

### 17. `readdir`
Legge un'entry da una directory.
```c
#include <dirent.h>
#include <stdio.h>

int main() {
    DIR *dir = opendir(".");
    if (dir == NULL) {
        perror("opendir");
        return 1;
    }
    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        printf("%s\n", entry->d_name);
    }
    closedir(dir);
    return 0;
}
```

### 18. `closedir`
Chiude una directory.
```c
#include <dirent.h>

int main() {
    DIR *dir = opendir(".");
    if (dir == NULL) {
        perror("opendir");
        return 1;
    }
    closedir(dir);
    return 0;
}
```

### 19. `link`
Crea un link fisico tra file.
```c
#include <unistd.h>

int main() {
    if (link("file.txt", "linkfile.txt") == -1) {
        perror("link");
        return 1;
    }
    return 0;
}
```

### 20. `symlink`
Crea un link simbolico.
```c
#include <unistd.h>

int main() {
    if (symlink("file.txt", "symlinkfile.txt") == -1) {
        perror("symlink");
        return 1;
    }
    return 0;
}
```

Navigare nel file system in C può essere fatto utilizzando diverse syscall e funzioni della libreria standard. Ecco alcune delle più comuni syscall usate per questo scopo, insieme a esempi di utilizzo.

### 1. `opendir`
Apre una directory e restituisce un puntatore a `DIR`.

```c
#include <dirent.h>
#include <stdio.h>

int main() {
    DIR *dir = opendir(".");
    if (dir == NULL) {
        perror("opendir");
        return 1;
    }
    closedir(dir);
    return 0;
}
```

### 2. `readdir`
Legge una voce della directory aperta con `opendir`.

```c
#include <dirent.h>
#include <stdio.h>

int main() {
    DIR *dir = opendir(".");
    if (dir == NULL) {
        perror("opendir");
        return 1;
    }
    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        printf("%s\n", entry->d_name);
    }
    closedir(dir);
    return 0;
}
```

### 3. `closedir`
Chiude una directory aperta con `opendir`.

```c
#include <dirent.h>

int main() {
    DIR *dir = opendir(".");
    if (dir == NULL) {
        perror("opendir");
        return 1;
    }
    closedir(dir);
    return 0;
}
```

### 4. `openat`
Apre un file o una directory relativo a un file descriptor di directory.

```c
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

int main() {
    int dirfd = open(".", O_RDONLY | O_DIRECTORY);
    if (dirfd == -1) {
        perror("open");
        return 1;
    }
    int fd = openat(dirfd, "file.txt", O_RDONLY);
    if (fd == -1) {
        perror("openat");
        close(dirfd);
        return 1;
    }
    close(fd);
    close(dirfd);
    return 0;
}
```

### 5. `fdopendir`
Converte un file descriptor di directory in un puntatore a `DIR`.

```c
#include <fcntl.h>
#include <unistd.h>
#include <dirent.h>
#include <stdio.h>

int main() {
    int dirfd = open(".", O_RDONLY | O_DIRECTORY);
    if (dirfd == -1) {
        perror("open");
        return 1;
    }
    DIR *dir = fdopendir(dirfd);
    if (dir == NULL) {
        perror("fdopendir");
        close(dirfd);
        return 1;
    }
    closedir(dir);
    return 0;
}
```

### 6. `fstatat`
Ottiene informazioni su un file relativo a un file descriptor di directory.

```c
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <stdio.h>

int main() {
    int dirfd = open(".", O_RDONLY | O_DIRECTORY);
    if (dirfd == -1) {
        perror("open");
        return 1;
    }
    struct stat st;
    if (fstatat(dirfd, "file.txt", &st, 0) == -1) {
        perror("fstatat");
        close(dirfd);
        return 1;
    }
    printf("File size: %ld\n", st.st_size);
    close(dirfd);
    return 0;
}
```

### 7. `lstat`
Ottiene informazioni su un file, simile a `stat`, ma non segue i link simbolici.

```c
#include <sys/stat.h>
#include <stdio.h>

int main() {
    struct stat st;
    if (lstat("file.txt", &st) == -1) {
        perror("lstat");
        return 1;
    }
    printf("File size: %ld\n", st.st_size);
    return 0;
}
```

### 8. `getcwd`
Ottiene il percorso corrente di lavoro.

```c
#include <unistd.h>
#include <stdio.h>

int main() {
    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) == NULL) {
        perror("getcwd");
        return 1;
    }
    printf("Current working directory: %s\n", cwd);
    return 0;
}
```

### 9. `chdir`
Cambia la directory corrente di lavoro.

```c
#include <unistd.h>
#include <stdio.h>

int main() {
    if (chdir("/tmp") == -1) {
        perror("chdir");
        return 1;
    }
    printf("Changed directory to /tmp\n");
    return 0;
}
```

### 10. `mkdir`
Crea una nuova directory.

```c
#include <sys/stat.h>

int main() {
    if (mkdir("newdir", 0755) == -1) {
        perror("mkdir");
        return 1;
    }
    return 0;
}
```

### 11. `rmdir`
Rimuove una directory vuota.

```c
#include <unistd.h>

int main() {
    if (rmdir("newdir") == -1) {
        perror("rmdir");
        return 1;
    }
    return 0;
}
```

### 12. `rename`
Rinomina un file o una directory.

```c
#include <stdio.h>

int main() {
    if (rename("oldfile.txt", "newfile.txt") == -1) {
        perror("rename");
        return 1;
    }
    return 0;
}
```

### 13. `unlink`
Rimuove un file.

```c
#include <unistd.h>

int main() {
    if (unlink("file.txt") == -1) {
        perror("unlink");
        return 1;
    }
    return 0;
}
```

### 14. `link`
Crea un link fisico a un file.

```c
#include <unistd.h>

int main() {
    if (link("file.txt", "linkfile.txt") == -1) {
        perror("link");
        return 1;
    }
    return 0;
}
```

### 15. `symlink`
Crea un link simbolico a un file.

```c
#include <unistd.h>

int main() {
    if (symlink("file.txt", "symlinkfile.txt") == -1) {
        perror("symlink");
        return 1;
    }
    return 0;
}
```

### 16. `readlink`
Legge il contenuto di un link simbolico.

```c
#include <unistd.h>
#include <stdio.h>

int main() {
    char buffer[1024];
    ssize_t len = readlink("symlinkfile.txt", buffer, sizeof(buffer) - 1);
    if (len == -1) {
        perror("readlink");
        return 1;
    }
    buffer[len] = '\0';
    printf("Symlink points to: %s\n", buffer);
    return 0;
}
```

### 17. `access`
Controlla l'accesso a un file.

```c
#include <unistd.h>
#include <stdio.h>

int main() {
    if (access("file.txt", R_OK) == -1) {
        perror("access");
        return 1;
    }
    printf("File is readable\n");
    return 0;
}
```

### 18. `dup`
Duplica un file descriptor.

```c
#include <unistd.h>
#include <fcntl.h>

int main() {
    int fd = open("file.txt", O_RDONLY);
    if (fd == -1) {
        perror("open");
        return 1;
    }
    int new_fd = dup(fd);
    if (new_fd == -1) {
        perror("dup");
        close(fd);
        return 1;
    }
    close(fd);
    close(new_fd);
    return 0;
}
```

### 19. `dup2`
Duplica un file descriptor su uno specifico.

```c
#include <unistd.h>
#include <fcntl.h>

int main() {
    int fd = open("file.txt", O_RDONLY);
    if (fd == -1) {
        perror("open");
        return 1;
    }
    if (dup2(fd, 5) == -1) {
        perror("dup2");
        close(fd);
        return 1;
    }
    close(fd);
    close(5);
    return 0;
}
```

### 20. `fchdir`
Cambia la directory corrente di lavoro utilizzando un file descriptor.

```c
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>

int main() {
    int dirfd = open("/tmp", O_RDONLY | O_DIRECTORY);
    if (dirfd == -1) {
        perror("open");
        return 1;
    }
    if (fchdir(dirfd) == -1) {
        perror("fchdir");
        close(dirfd);
        return 1;
    }
    printf("Changed directory to /tmp\n");
    close(dirfd);
    return 0;
}
```

Certamente! Di seguito sono elencate alcune delle strutture comunemente utilizzate nelle syscall e nelle funzioni della libreria standard C per la gestione dei file system, insieme alle definizioni delle strutture stesse.

### 1. `struct dirent`
Questa struttura viene utilizzata con le funzioni che leggono le directory, come `readdir`.

```c
#include <dirent.h>

struct dirent {
    ino_t          d_ino;       // Numero dell'inode
    off_t          d_off;       // Offset della directory
    unsigned short d_reclen;    // Lunghezza di questa registrazione
    unsigned char  d_type;      // Tipo di file
    char           d_name[256]; // Nome del file (null-terminated)
};
```

### 2. `struct stat`
Questa struttura contiene informazioni sui file ed è utilizzata da syscall come `stat`, `lstat` e `fstat`.

```c
#include <sys/stat.h>

struct stat {
    dev_t     st_dev;     // ID del dispositivo
    ino_t     st_ino;     // Numero dell'inode
    mode_t    st_mode;    // Modalità del file
    nlink_t   st_nlink;   // Numero di link
    uid_t     st_uid;     // ID dell'utente proprietario
    gid_t     st_gid;     // ID del gruppo proprietario
    dev_t     st_rdev;    // ID del dispositivo (se speciale)
    off_t     st_size;    // Dimensione del file in byte
    blksize_t st_blksize; // Dimensione dei blocchi di I/O
    blkcnt_t  st_blocks;  // Numero di blocchi allocati
    time_t    st_atime;   // Tempo di ultimo accesso
    time_t    st_mtime;   // Tempo di ultima modifica
    time_t    st_ctime;   // Tempo di ultimo cambiamento dello stato
};
```

### 3. `struct DIR`
Questa struttura rappresenta una directory aperta e viene utilizzata con `opendir`, `readdir`, e `closedir`. La definizione effettiva della struttura `DIR` è interna e non standardizzata, quindi non è sempre visibile nei programmi utente. Tuttavia, è importante sapere che rappresenta una directory aperta.

### 4. `struct iovec`
Questa struttura viene utilizzata con le syscall `readv` e `writev` per operazioni di I/O scatter-gather.

```c
#include <sys/uio.h>

struct iovec {
    void  *iov_base; // Puntatore al buffer
    size_t iov_len;  // Lunghezza del buffer
};
```

### 5. `struct statvfs`
Questa struttura contiene informazioni sul file system e viene utilizzata dalla syscall `statvfs`.

```c
#include <sys/statvfs.h>

struct statvfs {
    unsigned long  f_bsize;    // Dimensione del blocco fondamentale del file system
    unsigned long  f_frsize;   // Dimensione del blocco frammento
    fsblkcnt_t     f_blocks;   // Numero totale di blocchi sul file system
    fsblkcnt_t     f_bfree;    // Numero di blocchi liberi
    fsblkcnt_t     f_bavail;   // Numero di blocchi disponibili per utenti non-root
    fsfilcnt_t     f_files;    // Numero totale di inode
    fsfilcnt_t     f_ffree;    // Numero di inode liberi
    fsfilcnt_t     f_favail;   // Numero di inode disponibili per utenti non-root
    unsigned long  f_fsid;     // ID del file system
    unsigned long  f_flag;     // Flags montati
    unsigned long  f_namemax;  // Lunghezza massima del nome del file
};
```

### 6. `struct flock`
Questa struttura viene utilizzata con la syscall `fcntl` per gestire i lock dei file.

```c
#include <fcntl.h>

struct flock {
    short  l_type;    // Tipo di lock: F_RDLCK, F_WRLCK, F_UNLCK
    short  l_whence;  // Come interpretare l_start: SEEK_SET, SEEK_CUR, SEEK_END
    off_t  l_start;   // Offset del lock
    off_t  l_len;     // Numero di byte da bloccare
    pid_t  l_pid;     // PID del processo che possiede il lock
};
```

Queste sono alcune delle principali strutture utilizzate con le syscall e le funzioni di libreria standard per la navigazione e la manipolazione del file system in C. Ognuna di queste strutture è progettata per fornire le informazioni necessarie per gestire file e directory in modo efficiente.