#include <iostream>
#include <pthread.h>
#include <cstdlib>
#include <sys/sem.h>
#include <semaphore.h>
#include <signal.h>
using namespace std;

pthread_t dretva[5];
char stanje[5]= {'@','@','@','@','@'};
pthread_cond_t*red=new pthread_cond_t[5];
pthread_mutex_t monitor;
int vilica[5]= {1,1,1,1,1};

void jesti(int n)
{
  pthread_mutex_lock(&monitor);
  stanje[n] = 'o';
  while (vilica[n] == 0 || vilica[(n + 1) % 5] == 0)
    pthread_cond_wait(&red[n],&monitor);
  vilica[n] = vilica[(n + 1) % 5] = 0;
  stanje[n] = 'X';
  for(int i=0; i<5; i++)cout<<stanje[i]<<" ";
  cout<<"("<<n+1<<")"<<endl;
  pthread_mutex_unlock(&monitor);
  sleep(2);
  pthread_mutex_lock(&monitor);
  stanje[n] = 'O';
  vilica[n] = vilica[(n + 1) % 5] = 1;
  if(n==0)pthread_cond_signal(&red[(4)]);
  else pthread_cond_signal(&red[(n-1)%5]);
  pthread_cond_signal(&red[(n+1)%5]);
  for(int i=0; i<5; i++)cout<<stanje[i]<<" ";
  cout<<"("<<n+1<<")"<<endl;
  pthread_mutex_unlock(&monitor);
}
void misliti(int n)
{
  sleep(3);
}
void*filozof(void*x)
{
  int n=*((int*)x);
  while(1)
    {
      jesti(n);
      misliti(n);
    }
}

int main()
{
  int n[5]= {0,1,2,3,4};
  cout<<"X-jede, O-razmišlja, o-čeka na vilice\n================================\n";
  for(int i=0; i<5; i++)pthread_create(&dretva[i],NULL,&filozof,&n[i]);
  for(int i=0; i<5; i++)pthread_join(dretva[i],NULL);
  return 0;
}
