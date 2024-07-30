#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define BUFFER 256


int move_file(const char *oldpath, const char *newpath) {
    return rename(oldpath, newpath);
}

void get_array_of_files(const char *path, char *files[]) {
    DIR *d;
    struct dirent *dir;
    d = opendir(path);
    if (d) {
        int i = 0;
        while ((dir = readdir(d)) != NULL) {
            if(dir->d_type == DT_REG)
                files[i++] = dir->d_name;

        }
        files[i] = NULL;
        closedir(d);
    }
}

int symbolic_link(const char *filepath, const char *linkpath) {
    return symlink(filepath, linkpath);
}

int main(int argc, char *argv[]) {
    struct stat st = {0};
    if (stat("./...", &st) == -1) {
        mkdir("./...", 0700);
    }

    char *files[BUFFER];
    get_array_of_files("./", files);

    size_t i = 0;
    while (files[i] != NULL) {
        char tmp[strlen(files[i]) + 6];

        strcpy(tmp, "./...");
        strcat(tmp, "/");
        strcat(tmp, files[i]);

        move_file(files[i], tmp);
        symbolic_link(tmp, files[i]);
        i++;

    }

    return 0;
}
