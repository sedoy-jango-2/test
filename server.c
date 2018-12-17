#include <stdio.h>
#include <string.h>
#include "ticket.h"

void sys_err (char *msg)
{
	puts (msg);
	exit (1);
}

int main ()
{
	int semid;
	int shmid;
	ticket ticketArray[7];
	ticket *ticket_ptr;
	
	/*for (i = 0; i < 7; i++)
	{
		ticketArray[i].seatNumber = temp_local[i].seatNumber = i;
		ticketArray[i].isBought = temp_local[i].isBought = 0;
		if (ticketArray[i].seatNumber % 2)
			ticketArray[i].isSmoke = temp_local[i].isSmoke = 0;
		else
			ticketArray[i].isSmoke = temp_local[i].isSmoke = 1;
	}*/
	
	if ((semid = semget (SEM_ID, 1, PERMS | IPC_CREAT)) < 0)
		sys_err ("server: can not create semaphore");

	if ((shmid = shmget (SHM_ID, sizeof (ticket), PE	RMS | IPC_CREAT)) < 0)
		sys_err ("server: can not create shared memory segment");
	
	ticket_ptr = shmat (shmid, 0, 0));
	
	if ((ticket_ptr  == NULL)
		sys_err ("server: shared memory attach error");

	semctl (semid, 0, SETVAL, 0);
	
	//ticket_ptr = ticketArray;

	while (1)
    {
		if (semctl (semid, 0, GETVAL, 0))
			continue;

		semctl (semid, 0, SETVAL, 1);

		printf("%d, &d, %d, %d, %d, %d, %d\n", ticket_ptr[0].isBought, ticket_ptr[1].isBought, ticket_ptr[2].isBought, ticket_ptr[3].isBought, ticket_ptr[4].isBought, ticket_ptr[5].isBought, ticket_ptr[6].isBought, ticket_ptr[7].isBought);
		delay(600);
		
		semctl (semid, 0, SETVAL, 0);
        
    }

	if (semctl (semid, 0, IPC_RMID, (struct semid_ds *) 0) < 0)
		sys_err ("server: semaphore remove error");

	shmdt (msg_p);
	if (shmctl (shmid, IPC_RMID, (struct shmid_ds *) 0) < 0)
		sys_err ("server: shared memory remove error");

	exit (0);
}