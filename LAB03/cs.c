#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <errno.h>


void die(char* s)
{
    perror(s);
    exit(1);
}

struct msgbuf
{
    long mtype;
    char mtext[128];
};

int main()
{
    int msqid;
    key_t key;
    struct msgbuf rcvbuffer;

    key = 1111;

    if((msqid = msgget(key, 0660))< 0)
        die("msgget()");

    for (int i = 2; i <= 10; i += 2) {
        while (1) {
            if (msgrcv(msqid, &rcvbuffer, 128, i, IPC_NOWAIT) < 0) {
                if (errno == ENOMSG) {
                    break;
                } else {
                    die("msgrcv");
                }
            }
            printf("%s\n", rcvbuffer.mtext);
        }
    }

    exit(1);
}