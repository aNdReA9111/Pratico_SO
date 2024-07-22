#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>

// Funzione ricorsiva per stampare la struttura ad albero di una directory
void print_tree(int dirfd, const char *path, int depth) {
    // Apre la directory specificata dal file descriptor
    DIR *dir = fdopendir(dirfd);
    if (dir == NULL) {
        perror("fdopendir");
        exit(EXIT_FAILURE);
    }

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
            continue; // Salta le directory speciali

        // Stampa indentazione
        for (int i = 0; i < depth; i++) {
            printf("\t");
        }

        // Stampa il nome dell'elemento
        printf("%s\n", entry->d_name);

        if (entry->d_type == DT_DIR) {
            // Se è una directory, aprila e chiama ricorsivamente la funzione
            int new_dirfd = openat(dirfd, entry->d_name, O_RDONLY | O_DIRECTORY);
            if (new_dirfd == -1) {
                perror("openat");
                closedir(dir);
                exit(EXIT_FAILURE);
            }

            print_tree(new_dirfd, entry->d_name, depth + 1);
            close(new_dirfd); // Chiudi il file descriptor della directory
        }
    }

    closedir(dir);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <directory>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    // Apri la directory iniziale
    int root_fd = open(argv[1], O_RDONLY | O_DIRECTORY);
    if (root_fd == -1) {
        perror("open");
        exit(EXIT_FAILURE);
    }

    // Stampa la struttura ad albero della directory
    print_tree(root_fd, argv[1], 0);

    close(root_fd); // Chiudi il file descriptor della directory iniziale
    return EXIT_SUCCESS;
}
