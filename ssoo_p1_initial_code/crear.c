#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>



int main(int argc, char *argv[]) {
  char name = argv[1];
  int permissions = atoi(arv[2]);
  int fd;

  if ((fd = creat(name, permissions)) < 0) {
    perror("Error when creating file");
    exit(-1);
  }
  close(fd);
  return fd;
}


