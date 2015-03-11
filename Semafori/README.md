#Sinkronizacija dretvi i procesa uz pomoć semafora

##Analiza tekstualnih datoteka
Zadatak
Načiniti program koji će analizirati svaku tekstualnu datoteku zadanu kao argument iz komandne linije. Ako datoteka završava ekstenzijom *.txt tada treba prebrojati riječi u datoteci te izračunati zaštitnu sumu. Ukoliko se radi o *.html datoteci, tada prebrojati "tagove" (samo početne, npr. <b>, ali ne i </b>) te također izračunati zaštitnu sumu. Ostale tipove datoteka treba ignorirati.

U rješenju neka program za svaku datoteku (zadanu u komandnoj liniji) stvori novi proces koji će ju obraditi. U svakom novom procesu stvoriti tri dretve: prva koja će čitati redak po redak iz datoteke i smjestiti ga u međuspremnik MS, druga koja će brojati riječi/tagove u tom retku te treća koja će računati zaštitnu sumu tog retka. Obzirom da će se obrađivati redak po redak, dretve treba sinkronizirati. 

##PROBLEM PUŠAČA CIGARETA
„Problem pušača cigareta“ predstavio je Patil kako bi ukazao na problem sinkronizacije korištenjem semafora. U ovom radu koristi se isti problem, ali različito rješenje radi ilustracije nekih problema sa semaforima.

U ovom slučaju sustav se sastoji od tri dretve koje predstavljaju pušače te jedne dretve trgovca. Svaki od „pušača“ ima kod sebe u neograničenoj količini jedan od tri sastojka potrebnih da se napravi i zapali cigareta: papir, duhan ili šibice. Svaki pušač ima različiti sastojak, tj. jedan ima samo papir, drugi duhan i treći samo šibice. Trgovac ima sve tri komponente u neograničenim količinama. Trgovac nasumice odabire dvije različite komponente, stavlja ih na stol te signalizira pušačima da su sastojci na stolu. Jedino pušač kojemu nedostaju sastojci stavljeni na stol, smije uzeti oba sastojka, signalizirati trgovcu te potom saviti cigaretu i popušiti ju. Trgovac tada stavlja nova dva sastojka na stol i postupak se ponavlja.

Jedno od mogućih rješenja sinkronizacije prikazano je u nastavku. Svaki pušač čeka na svom binarnom semaforu, a trgovac pri stavljanju novih sastojaka postavlja sve semafore. Pušač će tada samo jednom pogledati što ima na stolu, dok će u slijedećem pokušaju biti blokiran dok god trgovac ne stavi nove sastojke.

##Problem proizvođača i potrošača

Ostvariti komunikaciju među procesima koji se paralelno izvode korištenjem međuspremnika ograničene duljine. Dva proizvođača trebaju na početku pročitati niz znakova s tipkovnice i nakon toga poslati znak po znak preko međuspremnika potrošaču. Potrošač prima znak po znak od proizvođača i nakon što je primio sve znakove od oba proizvođača, treba ih ispisati na zaslonu.

Struktura rješenja

Podaci zajednički za proizvođače i potrošače (smjestiti ih u zajednički spremnik):

cjelobrojne varijable ULAZ i IZLAZ
međuspremnik M, polje od 5 mjesta
semafori PIŠI, PUN, PRAZAN
