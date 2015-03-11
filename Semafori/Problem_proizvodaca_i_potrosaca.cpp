#  include <iostream>
#include <sys/sem.h>
#include <sys/wait.h>
#include <sys/shm.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>

#define PUN 1
#define PISI 2
#define PRAZAN 3

using namespace std;

int semafor_id, segment_id;

struct prostor{
    int ulaz, izlaz;
    char M[5];
};

prostor* zajednicki;

int SemOp(int SemNum, int SemOp) {
    struct sembuf SemBuf;
    SemBuf.sem_num = SemNum;
    SemBuf.sem_op = SemOp;
    SemBuf.sem_flg = 0;
    return semop(semafor_id, &SemBuf, 1);
}

int SemSetVal(int SemNum, int SemVal) {
    return semctl (semafor_id, SemNum, SETVAL, SemVal);

}

int proizvodjaci(int br) {
        char pom[1000];
        SemOp(0, -1);

        cout << "Upisi znakove za " << br <<". proizvodjaca: ";
        cin.getline(pom, 1000);
        SemOp(0, 1);


        SemOp(4, -1);
        sleep(1);
        SemOp(4, 0);

        int i = 0,gotovo = 0;



        while(gotovo != 1) {
                SemOp(PUN, -1);
                SemOp(PISI, -1);
                zajednicki->M[zajednicki->ulaz] = pom[i];
                zajednicki->ulaz = (zajednicki->ulaz + 1);
               if(pom[i]=='\0'){}
		else cout << "Proizvodjac " << br << " -> " << pom[i] << endl;
                sleep(1);
                SemOp(PISI, 1);
               SemOp(PRAZAN, 1);
               if (pom[i] == '\0') gotovo++;
                i++;
        }
}

int potrosac() {
        char pom[1000];
        int i = 0;
        int gotovo = 0;
        while(gotovo != 2) {
                SemOp(PRAZAN, -1);
                pom[i] = zajednicki->M[zajednicki->izlaz];
                zajednicki->izlaz = (zajednicki->izlaz + 1);
               if(pom[i]=='\0'){}
		else
                cout << "Potrosac <- " << pom[i] << endl;
                SemOp(PUN, 1);
                if (pom[i] == '\0') gotovo++; else i++;
        }
        cout << "\nPrimljeno je: "<< pom << endl;
}

void brisi(int sig) {
    (void) semctl(semafor_id, 0, IPC_RMID, 0);

    (void) shmdt((char *) zajednicki);
    (void) shmctl(segment_id, IPC_RMID, NULL);
}

int main(){
    sigset(SIGINT, brisi);

    semafor_id = semget(IPC_PRIVATE, 5, 0600);
    segment_id = shmget(IPC_PRIVATE, sizeof(prostor), 0600);

    if (semafor_id == -1 || segment_id == -1){
        exit(1);
    }

    zajednicki = (prostor*) shmat(segment_id, NULL, 0);

    zajednicki -> ulaz = 0;
    zajednicki -> izlaz = 0;

    SemSetVal(0, 1);
    SemSetVal(PUN, 1);
    SemSetVal(PISI, 1);
    SemSetVal(PRAZAN, 0);
    SemSetVal(4, 2);

    for (int i = 1; i < 3; i++) {
        switch(fork()) {
            case 0:
                proizvodjaci(i);
                exit(0);
            case -1: exit(1);
        }
    }

    switch(fork()) {
        case 0:
            potrosac();
            exit(0);
        case -1: exit(1);
    }

    for(int i = 0; i < 3; i++) wait(NULL);

    brisi(0);
    return 0;
}


