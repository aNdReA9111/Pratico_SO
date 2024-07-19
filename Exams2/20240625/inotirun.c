#include <sys/inotify.h>
#include <sys/wait.h>
#include <sys/types.h>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>


#define INITIAL_ARRAY_SIZE 256
#define LINE_BUFFER_SIZE 1024

// Funzione per leggere tutte le righe di un file e inserire in un array chiamato lines[]
void read_file_to_array(const char *filename, char *lines[], int *line_count) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Errore nell'apertura del file");
        return;
    }

    // Allocazione iniziale per l'array di stringhe
    int array_size = INITIAL_ARRAY_SIZE;
    *line_count = 0;

    char line[LINE_BUFFER_SIZE];

    // Lettura del file riga per riga
    while (fgets(line, LINE_BUFFER_SIZE, file)) {
        // Rimozione del carattere di newline, se presente
        // line[strcspn(line, "\n")] = '\0';

        // Allocazione della memoria per la riga
        lines[*line_count] = malloc((strlen(line) + 1) * sizeof(char));
        if (lines[*line_count] == NULL) {
            
            perror("Errore nell'allocazione della memoria");
            
            // Liberare la memoria già allocata
            for (int i = 0; i < *line_count; i++) {
                free(lines[i]);
            }
            fclose(file);
            return;
        }
        strcpy(lines[*line_count], line);
        (*line_count)++;

        // Se necessario, ridimensionare l'array di stringhe
        if (*line_count >= array_size) {
            array_size *= 2;
            char **new_lines = realloc(lines, array_size * sizeof(char*));
            if (new_lines == NULL) {
                perror("Errore nel ridimensionamento dell'array");
                // Liberare la memoria già allocata
                for (int i = 0; i < *line_count; i++) {
                    free(lines[i]);
                }
                fclose(file);
                return;
            }
            lines = new_lines;
        }
    }

    fclose(file);
}

// Funzione per liberare la memoria allocata per l'array bidimensionale
void free_string_array(char *lines[], int count) {
    for (int i = 0; i < count; i++) {
        free(lines[i]);
    }
}


int exec_command(const char* dir, const char* filename){
    char path[strlen(dir) + strlen(filename) + 2];

    strcpy(path, dir);
    
    if(path[strlen(path) - 1] != '/')
        strcat(path, "/");
    
    strcat(path, filename);

    int line_count;
    char *lines[INITIAL_ARRAY_SIZE]; // Provide the size of the array
    read_file_to_array(path, lines, &line_count);
    lines[line_count] = malloc(sizeof(char));
    lines[line_count] = NULL;    

    for (size_t i = 0; i < line_count; i++)
    {
        int iter=0;
        char *my_args[128];
        char *path_to_exec = malloc(strlen(lines[i]) * sizeof(char));
        strcpy(path_to_exec, lines[i++]);
        path_to_exec[strcspn(path_to_exec, "\n")] = '\0';
        printf("Path: %s\n", path_to_exec);
        while (lines[i] != NULL){
            

            
            if(strcmp(lines[i], "\n") == 0){
                break;
            }else{
                char *arg = malloc(strlen(lines[i]) * sizeof(char));
                arg = lines[i];
                arg[strcspn(arg, "\n")] = '\0';
                
                my_args[iter] = malloc(strlen(arg) * sizeof(char*));
                strcpy(my_args[iter], arg);

                free(arg);

                printf("Arg: %s\n", my_args[iter]);
                iter++, i++;
            }
            
        }
        my_args[iter] = NULL;

        pid_t fork_retvalue;
        switch (fork_retvalue = fork())
        {
        case 0:
            // figlio, esegue ciò che sta nel file 
            execv(path_to_exec, my_args);
            perror("exec error");
            break;

        case -1:
            perror("fork error");
            break;
        
        default:
            int status;
            wait(&status);
        } 
    }
    remove(path);
    strcpy(path, "\0");
    return EXIT_SUCCESS;
}

int handler_watch(int fd, const char *dir){
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

            if(event->mask & (IN_MOVED_TO | IN_CREATE)){
                exec_command(dir, event->name);
            }
        }
    }
    return EXIT_SUCCESS;
}

int main(int argc, char *argv[]) {
    int fd = inotify_init();
    if(fd == -1)
        perror("inotify_init");

    
    int wd = inotify_add_watch(fd, argv[1], IN_MOVED_TO | IN_CREATE);
    if (wd < 0) {
        perror("inotify_add_watch");
        close(fd);
        exit(EXIT_FAILURE);
    }

    handler_watch(fd, argv[1]);

    // Rimuovi la watch e chiudi l'istanza di inotify
    inotify_rm_watch(fd, wd);
    close(fd);
    
    return 0;
}