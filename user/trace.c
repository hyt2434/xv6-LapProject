#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
    int mask;
    if (argc < 3) {
        fprintf(2, "Usage: trace mask command\n");
        exit(1);
    }
    mask = atoi(argv[1]);
    if (trace(mask) < 0) {
        fprintf(2, "trace failed\n");
        exit(1);
    }
    exec(argv[2], &argv[2]);
    fprintf(2, "exec %s failed\n", argv[2]);
    exit(1);
}
  