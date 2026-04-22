#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/riscv.h"
#include "user/user.h"

#define BIT(i) (1U << (i))

static void
print_report(uint mask, int npages)
{
  if(mask == 0){
    printf("recently accessed pages: none\n");
  } else {
    int first = 1;

    printf("recently accessed pages: ");

    for(int i = 0; i < npages; i++){
      if(mask & BIT(i)){
        if(first == 0)
          printf(" ");
        printf("%d", i);
        first = 0;
      }
    }
    printf("\n");
  }

  printf("bitmap (page 0 = LSB): ");
  for(int i = 0; i < npages; i++){
    int bit = npages - 1 - i;
    printf("%d", (int)((mask >> bit) & 1ULL));
  }
  printf("\n");

  if(mask != 0){
    printf("set bits:\n");
    for(int i = 0; i < npages; i++){
      if(mask & BIT(i))
        printf("  page %d -> bit %d = 1\n", i, i);
    }
  }
}

int
main(int argc, char *argv[])
{
  enum { NPAGES = 8 };
  char *buf;
  uint mask;
  volatile char sink;

  (void)argc;
  (void)argv;

  printf("pgaccesstest: start\n\n");

  buf = sbrk(NPAGES * PGSIZE);
  if(buf == SBRK_ERROR){
    fprintf(2, "pgaccesstest: sbrk failed\n");
    exit(1);
  }

  mask = 0;
  if(pgaccess(buf, NPAGES, &mask) < 0){
    fprintf(2, "pgaccesstest: pgaccess failed on first call\n");
    exit(1);
  }
  print_report(mask, NPAGES);

  printf("\n");

  buf[1 * PGSIZE] += 1;
  sink = buf[4 * PGSIZE];
  (void)sink;

  mask = 0;
  if(pgaccess(buf, NPAGES, &mask) < 0){
    fprintf(2, "pgaccesstest: pgaccess failed on second call\n");
    exit(1);
  }
  print_report(mask, NPAGES);

  printf("\n");

  mask = 0;
  if(pgaccess(buf, NPAGES, &mask) < 0){
    fprintf(2, "pgaccesstest: pgaccess failed on third call\n");
    exit(1);
  }
  print_report(mask, NPAGES);

  printf("\n");

  buf[2 * PGSIZE] += 1;
  sink = buf[7 * PGSIZE];
  (void)sink;

  mask = 0;
  if(pgaccess(buf, NPAGES, &mask) < 0){
    fprintf(2, "pgaccesstest: pgaccess failed on fourth call\n");
    exit(1);
  }
  print_report(mask, NPAGES);

  if(sbrk(-NPAGES * PGSIZE) == SBRK_ERROR){
    fprintf(2, "pgaccesstest: sbrk shrink failed\n");
    exit(1);
  }

  exit(0);
}