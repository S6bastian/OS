#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/stat.h>
#include <sys/msg.h>
#include <stdio.h>
#include <signal.h>
#include <time.h>

struct msgbuf
{
    long mtype;
    char mtext[128];
};

void die(char *s)
{
    perror(s);
    exit(1);
}

void sigchld_handler(int sig);
int signalON;

int main()
{
    int msqid;
    key_t key;
    struct msgbuf sbuf;
    size_t buflen;

    signalON = 0;
    signal(SIGINT, sigchld_handler);

    printf("INICIANDO fork.c: \n");
    int id = fork();

    if (id == -1)
    {
        perror("fork");
        exit(1);
    }

    if (id == 0) // Proceso Hijo
    {
        int numero_aleatorio;
        printf("Hijo Pid: %d\n", getpid());

        while (1)
        {
            if (signalON) // Comprueba si la señal SIGINT fue recibida
            {
                for (int i = 0; i < 450; i++) // Se satura a los 900 mensajes
                {
                    numero_aleatorio = rand() % 100 + 1;
                    //printf("Hijo: %d\n", numero_aleatorio);

                    if (numero_aleatorio <= 10) // Si el número aleatorio es menor o igual a 10
                    {
                        int msgflg = IPC_CREAT | 0660;
                        
                        key = 1111; // key 
                        
                        if ((msqid = msgget(key, msgflg)) < 0)
                            die("msgget");
                        sbuf.mtype = numero_aleatorio;
                        sprintf(sbuf.mtext, "Numero de Hijo: %d", numero_aleatorio);
                        buflen = strlen(sbuf.mtext) + 1;

                        if (msgsnd(msqid, &sbuf, buflen, IPC_NOWAIT) < 0)
                        {
                            printf("%d, %ld, %s, %d \n", msqid, sbuf.mtype, sbuf.mtext, (int)buflen);
                            die("msgsnd");
                        }
                    }
                    else // Si el número aleatorio es mayor que 10
                    {
                        int msgflg = IPC_CREAT | 0606;
                        
                        key = 9999; // key para uni.c

                        if ((msqid = msgget(key, msgflg)) < 0)
                            die("msgget");

                        sbuf.mtype = numero_aleatorio;
                        sprintf(sbuf.mtext, "Numero de Hijo: %d", numero_aleatorio);
                        buflen = strlen(sbuf.mtext) + 1;

                        if (msgsnd(msqid, &sbuf, buflen, IPC_NOWAIT) < 0)
                        {
                            printf("%d, %ld, %s, %d \n", msqid, sbuf.mtype, sbuf.mtext, (int)buflen);
                            die("msgsnd");
                        }
                    }
                }
                exit(0);
            }
            else
            {
                pause(); // Pausa el proceso hasta que se reciba una señal
            }
        }
        exit(0);
    }
    else // Padre
    {
        int numero_aleatorio;

        printf("Padre pid: %d\n", getpid());
        while (1)
        {
            if (signalON)
            {
                for (int i = 0; i < 450; i++)
                {
                    numero_aleatorio = rand() % 100 + 1;
                    //printf("Padre: %d\n", numero_aleatorio);

                    if (numero_aleatorio <= 10) // Si el número aleatorio es menor o igual a 10
                    {
                        int msgflg = IPC_CREAT | 0660;
                        
                        key = 1111; // key
                        
                        if ((msqid = msgget(key, msgflg)) < 0)
                            die("msgget");
                        sbuf.mtype = numero_aleatorio;
                        sprintf(sbuf.mtext, "Numero de Padre: %d", numero_aleatorio);
                        buflen = strlen(sbuf.mtext) + 1;

                        if (msgsnd(msqid, &sbuf, buflen, IPC_NOWAIT) < 0)
                        {
                            printf("%d, %ld, %s, %d \n", msqid, sbuf.mtype, sbuf.mtext, (int)buflen);
                            die("msgsnd");
                        }
                    }
                    else // Si el número aleatorio es mayor que 10
                    {
                        int msgflg = IPC_CREAT | 0606;

                        key = 9999; // key para uni.c

                        if ((msqid = msgget(key, msgflg)) < 0)
                            die("msgget");
                        sbuf.mtype = numero_aleatorio;
                        sprintf(sbuf.mtext, "Numero de Padre: %d", numero_aleatorio);
                        buflen = strlen(sbuf.mtext) + 1;
                        if (msgsnd(msqid, &sbuf, buflen, IPC_NOWAIT) < 0)
                        {
                            printf("%d, %ld, %s, %d \n", msqid, sbuf.mtype, sbuf.mtext, (int)buflen);
                            die("msgsnd");
                        }
                    }
                }
                exit(0);
            }
            
            else
            {
                pause(); // Pausa el proceso hasta que se reciba una señal
            }
        }
        exit(0);
    }
    return (0);
}

// Manejador de la señal SIGINT
void sigchld_handler(int sig)
{
    signalON = 1; // Activa la señal
}