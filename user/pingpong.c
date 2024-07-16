#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#define MAXSIZE 4

int
main(int argc, char *argv[])
{
    int p1[2];
    int p2[2];
    char buf[MAXSIZE+1];

    pipe(p1);
    pipe(p2);

    if(fork()>0){//父进程
        
        close(p1[0]);
        write(p1[1],"ping",MAXSIZE);

        wait((int *)0);
        
        close(p2[1]);
        read(p2[0],buf,MAXSIZE+1);

        printf("%d: received %s\n",getpid(),buf);
        wait(0);
    }
    else{//子进程
        close(p1[1]);
        read(p1[0],buf,MAXSIZE+1);

        printf("%d: received %s\n",getpid(),buf);

        close(p2[0]);
        write(p2[1],"pong",MAXSIZE);
        exit(0);

    }

    exit(0);


}
