#include "ticket.h"

int parse_cmdline_args(int argc, char** argv)
{
    int result = -1;
    int i;
    if(argc == 2) {
        /* Check for commandline argument to be a number */
        for(i = 0; i < strlen(argv[1]); ++i) {
            if(!isdigit(argv[1][i])){
                return -1;
            }
        }
        /* Convert it to integer if it IS a number */
        result = atoi(argv[1]);
    }

    return result;
}

int get_shared_memory_descr()
{
    int o_flags = O_RDWR;
    int fd = shm_open(SHR_MEM_NAME, o_flags, 0644);
	/* Open seller-program SHM file for RD-WR */
    return fd;
}

Tickets* map_shared_memory(int fd)
{
    /* Get info about shared memory file */
    struct stat sb;
    fstat(fd, &sb);
    size_t shared_memory_lenght = sb.st_size;

    Tickets* my_tickets = (Tickets*)mmap(NULL, shared_memory_lenght, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
    return my_tickets;
}

void do_buying(Tickets* my_tickets, sem_t* main_semaphore, int client_num)
{
    /* Number of smoker and non smoker places to buy */
    int smokerplaces_tobuy = 6;
    int nonsmokerplaces_tobuy = 5;

    /* Actual places to buy */
    int smokerplaces[] = {33, 34, 35, 36, 37, 38};
    int nonsmokerplaces[] = {55, 56, 57, 58, 59};

    /* Wait interval */
    int interval_secs = 6;
    int i;

    printf("Client #%d: Waiting for semaphore to release\n", client_num);
    sem_wait(main_semaphore); /* Lock semaphore before buying */
    for(i = 0; i < smokerplaces_tobuy; ++i) {
        printf("Client #%d > buying place #%d for smokers...\n", client_num, smokerplaces[i]);
        if(my_tickets->smokers[smokerplaces[i]] == 0) {
            my_tickets->smokers[smokerplaces[i]] = client_num;
            printf("Client #%d >> Successfully bought!\n", client_num);
        } else {
            printf("Client #%d >> Place is already reserved\n", client_num);
        }
        sleep(interval_secs);
    }

    for(i = 0; i < nonsmokerplaces_tobuy; ++i) {
        printf("Client #%d > buying place #%d for non-smokers...\n", client_num, nonsmokerplaces[i]);
        if(my_tickets->non_smokers[nonsmokerplaces[i]] == 0) {
            my_tickets->non_smokers[nonsmokerplaces[i]] = client_num;
            printf("Client #%d >> Successfully bought!\n", client_num);
        } else {
            printf("Client #%d >> Place is already reserved\n", client_num);
        }
        sleep(interval_secs);
    }
    sem_post(main_semaphore); /* Unlock semaphore after buying */
}


int main(int argc, char* argv[])
{
    int client_num = parse_cmdline_args(argc, argv);
    if(client_num == -1) {
        printf("Invalid commandline arguments!\n");
        exit(1);
    }

    int fd = get_shared_memory_descr();
    if(fd == -1) {
        printf("An error occured while constructing descriptor to shared memory!\n");
        exit(1);
    }

    Tickets* my_tickets = map_shared_memory(fd);
    if(my_tickets == MAP_FAILED) {
        printf("Failed to create memory mapping!\n");
        exit(1);
    }

    sem_t *main_semaphore = sem_open(BUYER_SEMAPHORE_NAME, 0);
    if(main_semaphore == SEM_FAILED) {
        printf("Failed to open a semaphore!\n");
        exit(1);
    }
    printf("Client ready!\n");

    do_buying(my_tickets, main_semaphore, client_num);

    sem_unlink(BUYER_SEMAPHORE_NAME);
    shm_unlink(SHR_MEM_NAME);
    close(fd);

    return 0;
}
