#include <iostream> 
#include <signal.h>
#include <ctime>

using namespace std;

int prioritet[10]={0};
int brojac[10]={0};
int prviput[10]={0};

void obrada_prekida(int trenutna_razina){

int brojac2=brojac[trenutna_razina];
if(prviput[trenutna_razina]==0) cout<<"Pocela obrada prekida razine: "<<trenutna_razina<<endl;
prviput[trenutna_razina]=1;

while(brojac2<6){
     if(prioritet[trenutna_razina]==0)break;
     cout<<"Prekid: "<<brojac2<<"/5"<<endl;
     sleep(1);
     
     brojac2++;
     brojac[trenutna_razina]=brojac2;

}//while
if(brojac2==6)brojac[trenutna_razina]=0;
prioritet[trenutna_razina]=0;
if(prviput[trenutna_razina]==1)cout<<"Zavrsila obrada prekida razine: "<<trenutna_razina<<endl;
prviput[trenutna_razina]=0;

}//obrada_prekida

void prekidna_rutina(int sig){ 

     time_t t; 
     time(&t); 
     cout<<"Prekidna rutina pozvana u:"<<ctime(&t)<<endl;
    
      sighold(SIGINT); 
      int novi_prioritet;
      cout<<"Upisi razinu prekida: ";
      cin>>novi_prioritet;
      prioritet[novi_prioritet]=1; 
      int tekuci_prioritet=novi_prioritet; 
      for(int i=novi_prioritet;i<10;i++) 
           if(prioritet[i]==1) tekuci_prioritet=i;      
    
      sigrelse(SIGINT); 
     
     do{
      
	  obrada_prekida(tekuci_prioritet);
          bool izlaz=true;
          for(int j=9;j>=0&&izlaz;j--){
              if(prioritet[j]>0){
		  tekuci_prioritet=j; 
                  novi_prioritet=j; 
                  izlaz=false;
 }//if
       if(izlaz)tekuci_prioritet=-1;
}//for 

//}//if
     }while(tekuci_prioritet>0);

}//prekidna_rutina

int main(void){
    sigset(SIGINT,prekidna_rutina);
    int i=1;
    do{
	cout<<"Glavni program: "<<i<<"/10"<<endl;
        sleep(1);
        i++;
	}while(i<11);
         
    return 0;
}

	
	
