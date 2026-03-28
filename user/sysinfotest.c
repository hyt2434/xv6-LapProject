#include "kernel/types.h"
#include "kernel/sysinfo.h"
#include "user/user.h"

void check(uint64 s1, uint64 s2) {
    if (s1 < s2) printf("Increased! \n");
    else if (s1 > s2) printf("Decreased! \n");
    else printf("Did not change!");
}

int main() {
    struct sysinfo s1, s2;
    printf("--- START SYSINFO TEST ---\n");

    // TEST 1: Kiem tra nproc 
    sysinfo(&s1);
    int pid = fork();
    if (pid < 0) { 
        printf("Fork failed\n"); 
        exit(1); 
    }
    if (pid == 0) {
        for(volatile int i = 0; i < 1000000; i++);
        exit(0);
    }
    sysinfo(&s2);
    printf("nproc: %ld -> %ld\n", s1.nproc, s2.nproc);
    check(s1.nproc, s2.nproc);

    // TEST 2: Kiem tra nopenfiles 
    int fd = open("README", 0);
    sysinfo(&s1); 
    if(fd >= 0) {
        int fd2 = open("console", 0);
        sysinfo(&s2);
        printf("nopenfiles: %ld -> %ld\n", s1.nopenfiles, s2.nopenfiles);
        check(s1.nopenfiles, s2.nopenfiles);
        close(fd2);
        close(fd);
    }

    // TEST 3: Kiem tra freemem 
    sysinfo(&s1);
    void *m = sbrk(4096); // Xin 1 trang (PGSIZE)
    if (m != (void*)-1) {
        ((char*)m)[0] = 1; // Ghi du lieu de kich hoat cap phat thuc su
        sysinfo(&s2);
        printf("freemem: %ld -> %ld\n", s1.freemem, s2.freemem);
        check(s1.freemem, s2.freemem);
        sbrk(-4096); // Tra lai bo nho
    }

    wait(0); // Don dep process con
    printf("--- FINISHED ---\n");
    exit(0);
}