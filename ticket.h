#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>  /* For mmap() */
#include <sys/stat.h>  /* For mode constants */
#include <fcntl.h>     /* For O_* constants */
#include <unistd.h>    /* ftruncate, sleep */
#include <semaphore.h> /* sem_open */
#include <errno.h>
#include <ctype.h>    /* isdigit */
#include <string.h> /* strlen */

#define SHR_MEM_NAME "/airplane_tickets_mem.01" /* Shared memory name */
#define BUYER_SEMAPHORE_NAME "/airplane_tickets_sem.01" /* Main semaphore name */

#define SMOKERS_MAX_PLACES 50
#define NON_SMOKERS_MAX_PLACES 100


typedef struct Tickets {
    int smokers_places;
    int non_smokers_places;
    int non_smokers[NON_SMOKERS_MAX_PLACES];
    int smokers[SMOKERS_MAX_PLACES];
} Tickets;
