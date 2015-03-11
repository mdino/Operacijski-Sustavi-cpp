Zadatak 2b: Procesi

Napisati program koji stvara M novih procesa (M ovisi o argumentima komandne linije). Svaki proces ispisuje površinu jednog jednakostraničnog mnogokuta, te nakon jedne sekunde ispisuje opseg istog mnogokuta na zaslon. Broj i duljina stranica, za svaki mnogokut, navodi se kao argument komandne linije.

Zadatak 2d: Isključivanje Lamportovim algoritmom
Lamportov postupak međusobnog isključivanja
Napomena: zadataka ostvariti koristeći dretve
Zadatak
Ostvariti sustav paralelnih procesa/dretvi. Struktura procesa/dretvi dana je sljedećim pseudokodom:

proces proc(i)          /* i  [0..n-1] */
{
   za k = 1 do 5 čini {
      uđi u kritični odsječak
      za m = 1 do 5 čini {
          ispiši (i, k, m)
      }
      izađi iz kritičnog odsječka
   }
}
Međusobno isključivanje ostvariti za više procesa/dretvi (broj se unosi kao parametar pri pokretanju) međusobnim isključivanjem po Lamportovu algoritmu.

Lamportov algoritam:
zajedničke varijable: TRAŽIM[0..n-1], BROJ[0..n-1]
funkcija uđi_u_kritični_odsječak(i)
{
   TRAŽIM[i] = 1
   BROJ[i] = max(BROJ[0],...,BROJ[n-1]) + 1
   TRAŽIM[i] = 0

   za j = 0 do n-1 čini
      dok je TRAŽIM[j] <> 0 čini
         ništa
      dok je BROJ[j] <> 0 && (BROJ[j] < BROJ[i] || (BROJ[j] == BROJ[i] && j < i)) čini
         ništa
}
 
funkcija izađi_iz_kritičnog_odsječka(i)
{
   BROJ[i] = 0
}
Upute:

Ako se program rješava s procesima tada treba zajedničke varijable tako organizirati da se prostor za njih zauzme odjednom i podijeli među njima. Ovo je nužno zbog ograničenog broja segmenata i velikog broja korisnika.

Ovisno o opterećenju računala i broju procesa koji se pokreću, a da bi se vidjele razlike prilikom izvođenja programa može biti potrebno usporiti izvršavanje sa:

sleep(1);
nakon: ispisi (i, k, m).

Primjer ispisa:
pinus:~/vj3> ./a.out 3
Dretva: 1, K.O. br: 1 (1/5)
Dretva: 1, K.O. br: 1 (2/5)
Dretva: 1, K.O. br: 1 (3/5)
Dretva: 1, K.O. br: 1 (4/5)
Dretva: 1, K.O. br: 1 (5/5)
Dretva: 2, K.O. br: 1 (1/5)
Dretva: 2, K.O. br: 1 (2/5)
Dretva: 2, K.O. br: 1 (3/5)
Dretva: 2, K.O. br: 1 (4/5)
Dretva: 2, K.O. br: 1 (5/5)
Dretva: 3, K.O. br: 1 (1/5)
Dretva: 3, K.O. br: 1 (2/5)
Dretva: 3, K.O. br: 1 (3/5)
Dretva: 3, K.O. br: 1 (4/5)
Dretva: 3, K.O. br: 1 (5/5)
Dretva: 1, K.O. br: 2 (1/5)
Dretva: 1, K.O. br: 2 (2/5)
itd.
