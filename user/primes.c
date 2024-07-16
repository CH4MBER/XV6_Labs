#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"


void prime_treat(int p[]){

    
    int new_p[2];
    int buf[1];
    int n=0;
    
    close(p[1]);
    

    if (read(p[0],buf,sizeof(int))==0)
    {
        close(p[0]);
        return;
    }
    else
    {
        n=buf[0];
        printf("prime %d\n",n);
    }
    
    pipe(new_p);

    if(fork()==0){
        prime_treat(new_p);
        
        exit(0);
    }
    else{

        close(new_p[0]);
        while (read(p[0],buf,sizeof(int)!=0))
        {
            if (buf[0]%n!=0)
            {
                write(new_p[1],buf,sizeof(int));
            }
            

        }
        close(new_p[1]);
        close(p[0]);
        wait((int *)0);
        
    }
    return;


}



int main(int argc, char *argv[])
{
    int p[2];
    
    int buf[1];
    pipe(p);

    if(fork()==0){
        prime_treat(p);
        //wait((int *)0);
        exit(0);

    }
    else{

        close(p[0]);
        for (int i = 2; i < 36; i++)
        {
            buf[0]=i;
            //printf("%d",buf[0]);
            write(p[1],buf,sizeof(int));
        }
        close(p[1]);
        
        wait((int *)0);
        //exit(0);


    }

    exit(0);
}

