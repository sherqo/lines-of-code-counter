#include <stdio.h>
#include <dirent.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>


int count_lines_file(const char *filename)
{
    FILE *fp = fopen(filename, "r");
    if (fp == NULL)
        return 0;

    int ch;
    int lines = 0;

    while ((ch = fgetc(fp)) != EOF) {
        if (ch == '\n')
            lines++;
    }

    fclose(fp);
    return lines;
}


void run_dir(const char *path) {
  DIR *dir = opendir(path);
  if (!dir) {
    perror(path);
    return;
  }

  struct dirent *entry;
  while ((entry = readdir(dir)) != NULL) {
    if (!strcmp(entry->d_name, ".") || !strcmp(entry->d_name, "..")) continue; // Aborted (core dumped) :)

    char *fullpath = malloc(strlen(path) + strlen(entry->d_name) + 2);
    sprintf(fullpath, "%s/%s", path, entry->d_name);

    struct stat st;
    if (stat(fullpath, &st) == -1) {
      perror(fullpath);
      continue;
    }

    if (S_ISDIR(st.st_mode)) {
      printf("%s\n", fullpath);
      run_dir(fullpath); // i do not like recursion, TODO: fix
    } else if (S_ISREG(st.st_mode)) {
      printf("%s\n", fullpath);
      // count_file_lines(fullpath);
    }

    free(fullpath);
  }

  closedir(dir);
}

int main(int argc, char **argv) {
  // const char *path = (argc > 1) ? argv[1] : ".";

  int hi = count_lines_file("./test.txt");
  printf("%d\n", hi); // should 8

  // run_dir(path);
  return 0;
}
