#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  int p_first[2];
  int p_second[2];

  pipe(p_first);
  pipe(p_second);

  if(fork() == 0) {
    char r;

    read(p_first[0], &r, 1);
    fprintf(2, "%d: received ping\n", getpid());
    write(p_second[1], &r, 1);
    exit(0);
  } else {
    char r = 3;

    write(p_first[1], &r, 1);
    read(p_second[0], &r, 1);
    fprintf(2, "%d: received pong\n", getpid());
  }
  exit(0);
}
