#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  int on;

  if(argc < 3){
    fprintf(2, "Usage: ptprint onoff command [args...]\n");
    exit(1);
  }

  on = atoi(argv[1]);
  if(on != 0 && on != 1){
    fprintf(2, "ptprint: onoff must be 0 or 1\n");
    exit(1);
  }

  if(setptprint(on) < 0){
    fprintf(2, "ptprint: setptprint failed\n");
    exit(1);
  }

  exec(argv[2], &argv[2]);
  fprintf(2, "ptprint: exec %s failed\n", argv[2]);
  exit(1);
}