/*
Scrivere il programma nest2hard: che ha come parametro il pathname di una
directory. Il programma deve cercare tutti i link simbolici presenti nella
directory e sostituire ogni link simbolico nidificato (link simbolico che punta
a link simbolico) con un link fisico del file puntato.
*/
#define _GNU_SOURCE
#include <dirent.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#define MAX 4096

void remove_symlink(char *link_path, char *base_path) {
  char target_file_path[MAX];
  int r = readlink(link_path, target_file_path, MAX);
  if (r == -1) {
    perror("readlink");
    exit(EXIT_FAILURE);
  }
  struct stat st;
  char target_path[MAX];
  if (target_file_path[0] == '/') {
    snprintf(target_path, MAX, "%s%s", base_path, target_file_path);
  } else {
    snprintf(target_path, MAX, "%s/%s", base_path, target_file_path);
  }
  int rc = lstat(target_path, &st);
  if (rc != 0) {
    perror("lstat");
    exit(EXIT_FAILURE);
  }
  // non serve chiamata ricorsiva --> consegna chiede
  // di controllare solo link nella directory
  if (S_ISLNK(st.st_mode)) {
    // qui il target è un symlink a sua volta
    unlink(link_path);
    char target_target_file_path[MAX];
    char target_target_path[MAX];
    char new[MAX];
    int r = readlink(target_path, target_target_file_path, MAX);
    if (r == -1) {
      perror("readlink");
      exit(EXIT_FAILURE);
    }
    int last;
    for (int i = 0; i < strlen(target_path); i++) {
      if (target_path[i] == '/') {
        last = i;
      }
    }
    target_path[last] = '\0';
    if (target_target_file_path[0] == '/') {
      snprintf(target_target_path, MAX, "%s%s", target_path,
               target_target_file_path);
    } else {
      snprintf(target_target_path, MAX, "%s/%s", target_path,
               target_target_file_path);
    }
    link(target_target_path, link_path);
  }
}

void scan_dir(char *path) {

  DIR *dir = opendir(path);
  if (dir == NULL) {
    perror("opendir");
    exit(EXIT_FAILURE);
  }
  struct dirent *entry;
  struct stat st;
  int rc;
  while ((entry = readdir(dir)) != NULL) {
    char file_path[MAX];
    // concatenazione
    if (entry->d_name[0] == '/') {
      snprintf(file_path, MAX, "%s%s", path, entry->d_name);
    } else {
      snprintf(file_path, MAX, "%s/%s", path, entry->d_name);
    }
    rc = lstat(file_path, &st);
    if (rc != 0) {
      perror("lstat");
      exit(EXIT_FAILURE);
    }
    if (entry->d_type == DT_LNK ||
        (entry->d_type == DT_UNKNOWN && S_ISLNK(st.st_mode))) {
      remove_symlink(file_path, path);
    }
  }
  closedir(dir);
}

int main(int argc, char *argv[]) {
  scan_dir(argv[1]);
  return EXIT_SUCCESS;
}
