#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>

int main (int argc, char * argv[]) {
   __pid_t pid;
   int signal;

   pid = atoi(argv[2]);
   signal =atoi(argv[1]);

   kill(pid,signal);
   return(0);
}