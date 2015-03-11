#include <iostream>
#include <pthread.h>
#include <semaphore.h>
#include <fstream>
#include <string>
#include <sys/wait.h>
#include <stdlib.h>
#include <signal.h>

using namespace std;

sem_t citanje;
sem_t prebrojavanje;
sem_t checksum;
string red;
int gotovo;
int pbr;
int br_checksum;
ifstream dato;

pthread_t dretva[3];

void* citati(void* x)
    {
    char str[256];
    while(dato.eof()==0)
        {
        dato.getline(str, 256);
        red = string(str);


        sem_post(&prebrojavanje);
        sem_post(&checksum);


        sem_wait(&citanje);
        sem_wait(&citanje);
        //
        }
    gotovo = 1;
    }

void* txt(void* x)
    {
    while(gotovo == 0)
        {
        sem_wait(&prebrojavanje);
        int mjerac = 0;
        string prekid = ".?! ,:;";
        for (int i = 0; i < red.length(); i++)
            {
            char znak = red[i];
            if (mjerac == 0 && isalpha(znak))
                {
                pbr++;
                mjerac = 1;
                }

            else if (mjerac == 1 && prekid.find(znak, 0) != string::npos) mjerac = 0;

            }
        sem_post(&citanje);
        sleep(1);
        }
//cout<<"Izlaz\n";
    }

void* html(void* x)
    {
    int tag = 0;
    while(gotovo == 0)
        {
        sem_wait(&prebrojavanje);
        for (int i = 0; i < red.length(); i++)
            {
            char znak = red[i];
            if (tag == 0 && znak == '<') tag = 1;
            else if (tag == 1 && znak == '/') tag=0;
            else if (tag == 1 && znak == '>')
                {
                pbr++;
                tag=0;
                }
            }
        sem_post(&citanje);
        sleep(1);
        }
    }

void* kontrola(void* x)
    {
    while(gotovo == 0)
        {
        sem_wait(&checksum);
        for (int i = 0; i < red.length(); i++)
            {
            br_checksum ^= red[i];
            }
        sem_post(&citanje);
        sleep(1);
        }
    }

void brisi(int sig)
    {
    dato.close();
    dato.clear();
    sem_destroy(&citanje);
    sem_destroy(&prebrojavanje);
    sem_destroy(&checksum);
    }

int glavna (string dat, int op)
    {

    dato.open(dat.c_str(), ifstream::in);

    sem_init(&citanje, 0, 0);
    sem_init(&prebrojavanje, 0, 0);
    sem_init(&checksum, 0, 0);

    gotovo = 0;

    pthread_create(&dretva[0], NULL, citati, NULL);
    pthread_create(&dretva[1], NULL, kontrola, NULL);


    if (op == 1) pthread_create(&dretva[2], NULL, txt, NULL);
    else if (op == 2) pthread_create(&dretva[2], NULL, html, NULL);

    for (int i = 0; i < 3; i++) pthread_join(dretva[i], NULL);

    if (op == 1)
        cout << "Datoteka " << dat << " ima " << pbr  << " rijeci; " << "zastitna suma je: " <<
             br_checksum << endl;
    else if (op == 2)
        cout << "Datoteka " << dat << " ima " << pbr  << " tagova; " << "zastitna suma je: " <<
             br_checksum << endl;

    brisi(0);
    }

int main (int argc, char* argv[])
    {
    int i;
    int broj_procesa=0;
    sigset(SIGINT, brisi);
    for (i = 1; i< argc; i++)
        {
        int op = 0;
        string dat, extenzija;
        dat = argv[i];
        extenzija = dat.substr(dat.length() - 4);

        if (extenzija.compare(".txt")==0)
            {
            op = 1;
            broj_procesa++;
            }
        else
            {
            extenzija = dat.substr(dat.length() - 5);
            if (extenzija.compare(".html") == 0)
                {
                op = 2;
                broj_procesa++;
                }
            }
        if (op == 0)
            {
            cout << "Pogreska ! Upisite .txt ili .html kao extenziju datoteke!\n";
            }
        else
            switch(fork())
                {
                case 0:
//

                    glavna(dat, op);
                    exit(0);
                case -1:
                    exit(1);
                }
        }

    for (i=0; i<broj_procesa; i++)
        {
        wait(NULL);
        }

    return 0;
    }


