#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>



int main(int argc, char *argv[]) {
  if (argc != 3)
  {
    perror("Wrong usage of program");
    exit(-1);
  }

  const char* name = argv[1];

  int fd;
  if ((fd = open(name, O_RDONLY)) == -1)
  {
    if (strlen(argv[2]) != 3)
    {
      perror("Wrong format of the permissions");
      exit(-1);
    }
    const mode_t permissions = atoi(argv[2]);

    if ((fd = creat(name, permissions)) < 0) {
      perror("Error when creating file");
      exit(-1);
    }
    close(fd);

    return fd;
  }

  close(fd);
  perror("File exists");
  return -1;
}


