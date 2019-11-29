#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define print_usage() do { printf("usage: ./ts space_count\n"); } while(0);

int main(int argc, char **argv) {

  if (argc <= 2) {
    print_usage();
    exit(0);
  }

  int spacec = atoi(argv[2]);

  FILE *file = fopen(argv[1], "rb");
  if (!file) {
    printf("unable to open file [%s] for reading\n", argv[1]);
    exit(-1);
  }

  fseek(file, 0, SEEK_END);
  long fsize = ftell(file);
  fseek(file, 0, SEEK_SET);  /* same as rewind(f); */

  char *buf = malloc(fsize);
  if (!buf) {
    printf("unable to allocate enough memory for file\n");
    exit(-1);
  }

  if (fread(buf, 1, fsize, file) != fsize) {
    printf("unable to read complete file to memory\n");
    exit(-1);
  }

  fclose(file);

  size_t fnlen = strlen(argv[1]);
  char *nf = malloc(fnlen + 4);
  if (!nf) {
    printf("allocation error\n");
    exit(-1);
  }
  bzero(nf, fnlen + 4);
  strncpy(nf, argv[1], fnlen);
  strcat(nf, ".ts"); 

  file = fopen(nf, "wb");
  if (!file) {
    printf("unable to open file [%s] for reading\n", argv[1]);
    exit(-1);
  }

#define write_char(c) {                            \
    if (fputc(c, file) == EOF) {                   \
      printf("error writing [%02x] to file\n", c); \
      exit(0);                                     \
    }                                              \
  }

  for (long i = 0; i < fsize; i++) {
    char c = buf[i];
    
    if (c == '\t') {
      for (int n = 0; n < spacec; n++) {
        write_char(' ');
      }
    } else {
      write_char(c);
    }
  }

  free(buf);
  fclose(file);

  return 0;
}
