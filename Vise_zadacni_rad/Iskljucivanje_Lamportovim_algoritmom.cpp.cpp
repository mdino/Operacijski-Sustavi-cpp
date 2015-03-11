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

using namespace std;

int*broj_u_redu;
int*trazi_broj;
int ukupan_br_dretvi;
void*izadi_iz_kriticnog_odsjecka(void*x)
{
    broj_u_redu[ukupan_br_dretvi]=0;
}

void*udi_u_kriticni_odsjecak(void*x)
{
    trazi_broj[ukupan_br_dretvi]=1;
    int max=0;
    for(int i=0; i<ukupan_br_dretvi; i++)if(broj_u_redu[i]>max)max=broj_u_redu[i];
    broj_u_redu[ukupan_br_dretvi]=max+1;
    trazi_broj[ukupan_br_dretvi]=0;
    for(int j=0; j<ukupan_br_dretvi; j++)
    {
        while(trazi_broj[j]!=0) {}
        while(broj_u_redu[j]!=0&&(broj_u_redu[j]<broj_u_redu[ukupan_br_dretvi]||(broj_u_redu[j]==broj_u_redu[ukupan_br_dretvi]&&j<ukupan_br_dretvi))) {}


    }
}

void*dretva(void*x)
{
    int e= *((int*)x);
    for(int k=1; k<=5; k++)
    {
        udi_u_kriticni_odsjecak(&(ukupan_br_dretvi));
        for(int m=1; m<=5; m++)
        {
            cout<<"Dretva: "<<e+1<<", K.O. br: "<<k<<" ("<<m<<"/5)\n";
        }
        sleep(2);
        izadi_iz_kriticnog_odsjecka(&(ukupan_br_dretvi));
    }
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

int main(int argc,char*argv[])
{
    sigset(SIGINT,brisi);
    broj_u_redu=new int[atoi(argv[1])];
    trazi_broj=new int[atoi(argv[1])];
    ukupan_br_dretvi=atoi(argv[1]);
    pthread_t tid[atoi(argv[1])];
    int broj[atoi(argv[1])];

    for(int i=0; i<ukupan_br_dretvi; i++)broj[i]=i;
    for(int i=0; i<atoi(argv[1]); i++)
    {
        pthread_create(&tid[i],NULL,&dretva,&broj[i]);
    }
    for(int i=0; i<atoi(argv[1]); i++)
    {
        pthread_join(tid[i],NULL);
    }
    return 0;
}
