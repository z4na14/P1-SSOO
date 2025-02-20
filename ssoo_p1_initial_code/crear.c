#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>


int main(int argc, char *argv[]) {

  // Check if the appropiate number of arguments are used
  if (argc != 3)
  {
    perror("Wrong usage of program");
    exit(-1);
  }

  // Store name into a variable
  const char *name = argv[1];

  // Check if the file already exists, otherwise create it
  int fd;
  if ((fd = open(name, O_RDONLY)) == -1)
  {
    // Store last character read by function strtol anc check if
    // it coincides with the terminator
    char *endptr;
    const mode_t permissions = strtol(argv[2], &endptr, 8);

    if (strlen(argv[2]) != 4 || *endptr != '\0')
    {
      perror("Wrong format of the permissions");
      exit(-1);
    }

    // Override process umask to use the permission
    // specified by user
    umask(0000);
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


