#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>
#include <limits.h>

long long count_lines_file(const char *filename)
{
    FILE *fp = fopen(filename, "r");
    if (fp == NULL)
        return 0;

    int ch, last_ch = '\n';
    long long lines = 0;

    while ((ch = fgetc(fp)) != EOF) {
        if (ch == '\n')lines++;
        last_ch = ch;
    }

    if (last_ch != '\n') lines++;

    fclose(fp);
    return lines;
}

void run_dir(const char *path, long long *total_lines) {
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
      run_dir(fullpath, total_lines); // i do not like recursion, TODO: fix
    } else if (S_ISREG(st.st_mode)) {
      long long loc = count_lines_file(fullpath);
      printf("  %s: %lld\n",fullpath, loc);

      if(*total_lines > LLONG_MAX - loc) {
        fprintf(stderr, "overflow detected. exiting.\n");
        free(fullpath);
        closedir(dir);
        exit(EXIT_FAILURE);
      } else {
        *total_lines += loc;
      }
    }

    free(fullpath);
  }

  closedir(dir);
}

int main(int argc, char **argv) {
  const char *path = (argc > 1) ? argv[1] : ".";
  long long tot = 0;
  run_dir(path, &tot);
  printf("tot: %lld\n", tot);
  return 0;
}
