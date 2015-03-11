#include <iostream>
#include <pthread.h>
#include <cstdlib>
#include <time.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <stdio.h>
#include <semaphore.h>
using namespace std;

sem_t pusac1[3];
sem_t trgovac1;
int ima[3];
int stol[3]= {0,0,0};
char sast[3][7]= {"papir","sibice","duhan"};

void *pusac(void *y)
    {
    int k=*((int*)y);
    cout<<"Pusac "<<k<<" ima "<<sast[k]<<endl;
    int fali1,fali2;
    if(k==0)
        {
        fali1=1;
        fali2=2;
        }
    else if(k==1)
        {
        fali1=0;
        fali2=2;
        }
    else if(k==2)
        {
        fali1=0;
        fali2=1;
        }
    while(1)
        {

        sem_wait(&pusac1[k]);
        if(stol[fali1]==1&&stol[fali2]==1)
            {
            cout<<"Pusac "<<k<<" uzima sastojke i pusi"<<endl;
            sleep(1);
            for(int i=0; i<3; i++)
                {
                stol[i]=0;
                }
            sem_post(&trgovac1);
            }
        }
    }
void postavi_na_stol()
    {
    int s1, s2;
    do
        {
        s1=rand()%3;
        s2=rand()%3;
        }
    while(s1==s2);
    cout<<endl;

    stol[s1]=1;
    stol[s2]=1;
    cout<<"Trgovac stavlja: "<<sast[s1]<<" i "<<sast[s2]<<endl;
    }

void *trgovac(void *y)
    {
    while(1)
        {
        postavi_na_stol();


        sem_post(&pusac1[0]);
        sem_post(&pusac1[1]);
        sem_post(&pusac1[2]);


        sem_wait(&trgovac1);
        }
    }

int main()
    {
    pthread_t pusac2[3];
    pthread_t trgovac2;
    sem_init(&pusac1[0],0,0);

    sem_init(&pusac1[1],0,0);
    sem_init(&pusac1[2],0,0);
    sem_init(&trgovac1,0,0);

    for(int i=0; i<3; i++)
        {
        ima[i]=i;
        }


    for(int j=0; j<3; j++)
        {
        pthread_create(&pusac2[j],NULL,pusac,&ima[j]);

        }
    pthread_create(&trgovac2, NULL,trgovac,NULL);
    for(int i=0; i<3; i++)
        {
        pthread_join(pusac2[i],NULL);
        }
    pthread_join(trgovac2,NULL);
    return 0;
    }

