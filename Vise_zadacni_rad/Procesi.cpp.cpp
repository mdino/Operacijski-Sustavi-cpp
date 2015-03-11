#include <math.h>
#include <sys/wait.h>
#include<stdio.h>
#include<stdlib.h>
#include<iostream>
#include<pthread.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/msg.h>
#include <values.h>
#include <signal.h>
#include <unistd.h>

using namespace std;



int izracunaj(double n, double a)
{
    cout<<"n="<<n<<"\t a="<<a<<"\t P="<<0.25*n*a*a/tan(3.14159265358979/n)<<endl;
    cout<<"n="<<n<<"\t a="<<a<<"\t O="<<n*a<<endl;
    exit(0);
}

void brisi ( int sig )
{

    int i;

    for (i = 1; i < 5000000; i++)
    {

        if (shmctl(i, IPC_RMID, NULL) != -1)

            printf("Obrisao zajednicku memoriju %d\n", i);


        if (semctl(i, 0, IPC_RMID, 0) != -1)

            printf("Obrisao skup semafora %d\n", i);


        if (msgctl(i, IPC_RMID, NULL) != -1)

            printf("Obrisao red poruka %d\n", i);

    }

    exit(0);
}
int main(int argc, char* argv[])
{
    sigset(SIGINT,brisi);
    for(int i=1; i<argc; i+=2)
    {
        if(fork()==0)
        {
            izracunaj(atof(argv[i]),atof(argv[i+1]));
        }
    }
    for(int i=1; i<argc; i+=2)
    {
        wait(NULL);
    }
    return 0;
}

