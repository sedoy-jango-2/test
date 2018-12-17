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

	if ((semid = semget (SEM_ID, 1, 0)) < 0)
		sys_err ("client: can not get semaphore");

	if ((shmid = shmget (SHM_ID, sizeof (ticket), 0)) < 0)
		sys_err ("client: can not get shared memory segment");
	
	ticket_ptr = shmat (shmid, 0, 0));
	
	if ((ticket_ptr == NULL)
		sys_err ("client: shared memory attach error");
	
	ticket_ptr = ticketArray
	while (1)
    {
		scanf ("%d", ticketArray[0].isBought);
		while (semctl (semid, 0, GETVAL, 0) || msg_p->type != MSG_TYPE_EMPTY);
		semctl (semid, 0, SETVAL, 1);
		
		ticket_ptr = ticketArray;
		
		semctl (semid, 0, SETVAL, 0);
    }
	shmdt (msg_p);
	exit (0);
}