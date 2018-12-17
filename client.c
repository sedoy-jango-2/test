#include <stdio.h>
#include <string.h>
#include "message.h"

void sys_err (char *msg)
{
  puts (msg);
  exit (1);
}

int main ()
{
  int semid;                    /* идентификатор семафора */
  int shmid;                    /* идентификатор разделяемой памяти */
  message_t *msg_p;             /* адрес сообщения в разделяемой
                                   памяти */
  char s[MAX_STRING];

  /* получение доступа к массиву семафоров */
  if ((semid = semget (SEM_ID, 1, 0)) < 0)
    sys_err ("client: can not get semaphore");

  /* получение доступа к сегменту разделяемой памяти */
  if ((shmid = shmget (SHM_ID, sizeof (message_t), 0)) < 0)
    sys_err ("client: can not get shared memory segment");

  /* получение адреса сегмента */
  if ((msg_p = (message_t *) shmat (shmid, 0, 0)) == NULL)
    sys_err ("client: shared memory attach error");

  while (1)
    {
      scanf ("%s", s);
      while (semctl (semid, 0, GETVAL, 0) || msg_p->type != MSG_TYPE_EMPTY)
        /*
         *   если сообщение не обработано или сегмент блокирован - ждать
         *                                                             */
      ;
      semctl (semid, 0, SETVAL, 1);     /* блокировать */
      if (strlen (s) != 1)
        {
          /* записать сообщение "печать строки" */
          msg_p->type = MSG_TYPE_STRING;
          strncpy (msg_p->string, s, MAX_STRING);
        }
      else
        {
          /* записать сообщение "завершение работы" */
          msg_p->type = MSG_TYPE_FINISH;
        };
      semctl (semid, 0, SETVAL, 0);     /* отменить блокировку */
      if (strlen (s) == 1)
        break;
    }
  shmdt (msg_p);                /* отсоединить сегмент разделяемой памяти */
  exit (0);
}