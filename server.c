#include "ticket.h"

int get_shared_memory_descr()
{
    int o_flags = O_RDWR | O_CREAT;
    int fd = shm_open(SHR_MEM_NAME, o_flags, 0644); 
	
    ftruncate(fd, sizeof(Tickets));
    return fd;
}

Tickets* create_shared_memory(int fd)
{
    int i;
	
    Tickets* shared_mem = (Tickets*) mmap(NULL, sizeof(Tickets), PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
	
    for(i = 0; i < SMOKERS_MAX_PLACES; ++i) {
        shared_mem->smokers[i] = 0;
    }
    for(i = 0; i < NON_SMOKERS_MAX_PLACES; ++i) {
        shared_mem->non_smokers[i] = 0;
    }
    shared_mem->smokers_places     = SMOKERS_MAX_PLACES;
    shared_mem->non_smokers_places = NON_SMOKERS_MAX_PLACES;

    return shared_mem;
}

void out_tickets_struct(Tickets* my_tickets) 
{
    int i;
    printf("Airplane has %d seats for smokers and %d seats for non-smokers\n", my_tickets->smokers_places, my_tickets->non_smokers_places);
    printf("----------\n");
    printf("Avaliability of seats for smokers:\n");
    for(i = 0; i < my_tickets->smokers_places; ++i) {
        printf("Place #%d: ", i);
        if(my_tickets->smokers[i] == 0) {
            printf("Avaliable");
        } else {
            printf("Not avaliable. Reserved by: %d", my_tickets->smokers[i]);
        }
        printf("\n");
    }

    printf("Avaliability of seats for non-smokers:\n");
    for(i = 0; i < my_tickets->non_smokers_places; ++i) {
        printf("Place #%d: ", i);
        if(my_tickets->non_smokers[i] == 0) {
            printf("Avaliable");
        } else {
            printf("Not avaliable. Reserved by: %d", my_tickets->non_smokers[i]);
        }
        printf("\n");
    }
}

int main(int argc, char* argv[])
{
    shm_unlink(SHR_MEM_NAME); /* Unlink shared memory from process */
    
	int fd = get_shared_memory_descr(); /* Shared memory allocation */
    if(fd == -1) {
        printf("An error occured while constructing descriptor to shared memory!\n");
        exit(1);
    }

    Tickets* my_tickets = create_shared_memory(fd); 
    if(my_tickets == MAP_FAILED) {
        printf("Failed to create memory mapping!\n");
        exit(1);
    }

    sem_t* main_semaphore = sem_open(BUYER_SEMAPHORE_NAME, O_CREAT | O_EXCL, 0644, 1);
	
    if(main_semaphore == SEM_FAILED) {
        printf("Failed to open a semaphore! errno: %d\n", errno);
        shm_unlink(SHR_MEM_NAME); /* Unlink shared memory from process */
        exit(1);
    }

    printf("Server ready!\n");
    sleep(10);
	
    printf("Waiting for semaphore...\n");
	
    sem_wait(main_semaphore);

    printf("Output tickets...\n");
    out_tickets_struct(my_tickets);

    printf("Cleaning up...\n");
    sem_unlink(BUYER_SEMAPHORE_NAME); /* Remove a named semaphore */
    shm_unlink(SHR_MEM_NAME); /* Unlink shared memory from process */
    close(fd); /* Close file descriptor to shared memory */

    return 0;
}
