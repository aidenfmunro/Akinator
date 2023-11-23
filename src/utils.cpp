#include <sys/mman.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/mman.h>
#include <unistd.h>
#include <ctype.h>
#include <math.h>
#include <stdio.h>

bool isPointerValid(void *ptr) 
{
    size_t pageSize = sysconf(_SC_PAGESIZE); // get the page size

    void *base = (void *)((((size_t)ptr) / pageSize) * pageSize); // find the address of the page that contains p

    return msync(base, pageSize, MS_ASYNC) == 0; // call msync, if it returns non-zero, return false 
}

int checkInput(void)
{
    int ch = (char)0;
    while((ch = getchar()) != '\n')
      {
        if (!isspace(ch))
          return 1;
      }
    return 0;
}

void bufferCleaner(void)
{
    while (getchar() != '\n') ;
}