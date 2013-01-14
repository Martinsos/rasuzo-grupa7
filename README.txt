Fakultet elektrotehnike i računarstva
Ak. godina: 2012./2013.
Raspoznavanje uzoraka

Grupa 7
Tema projekta: Detekcija i "meko" raspoznavanje na temelju siluete osobe



Struktura projekta
==================

Projekt je sadržan u sljedećim direktorijima:

res/                - članci korišteni u razvoju
siluete/ i slike/   - baza
src/                - izvorni kod


Ovisnosti
=========
- OpenCV
- pkg-config


Upute za pokretanje programa
============================

Program je razvijen za operacijski sustav Linux (testirano na Ubuntu). Potrebno je imati instaliran OpenCV.
Priložene izvršne datoteke prevedene su s OpenCV 2.3.1 na sustavu Ubuntu 12.04 LTS x86_64.                                                
Ukoliko se vaš sustav razlikuje, moguće je da ćete morati sami stvoriti svoje izvršne datoteke.

Kako biste preveli program i stvorili izvršne datoteke, napravite sljedeće korake:

1.  Pozicionirajte se u direktorij demo/ :

        cd src/demo/


2.  Pokrenite Makefile koji će prevesti program:

        make

    U tekućem direktoriju stvorit će se dvije izvršne datoteke: "demo" i "silhouetteExtraction".


3.  Pokrenite stvorenu izvršnu datoteku "demo" s odgovarajućim parametrima. Ukoliko je pokušate pokrenuti bez
    navođenja parametara, ispisat će se upute za korištenje.

    Nakon što je datoteka uspješno pokrenuta, u tekućem direktoriju stvorit će se odgovarajući izvještaj
    zapisan u HTML obliku.
    Izvještaj sadrži matricu zabune, mjere točno klasificiranih primjera i rezultat svake pojedine klasifikacije.
    
    Primjeri uporabe programa "demo":
        ./demo granlund
        ./demo bp-knn


Organizacija baze slika
========================

Baza je sadržana u dva direktorija: siluete/ i slike/.
Direktorij slike/ sadrži slike koje smo koristili za izlučivanje silueta.
Direktorij siluete/ sadrži za svaku osobu direktorij gdje su izlučene njezine siluete.


Zadavanje skupova za učenje i testiranje
========================================

U direktoriju demo/ nalazi se konfiguracijska datoteka testConfDemo.txt. Pomoću nje se zadaju skupovi za učenje
i testiranje. Potrebno je za svaki direktorij sa silueatama navesti slike koje će se koristiti za testiranje.
Sve ostale slike iz odgovarajućeg direktorija koje nisu navedene bit će u skupu za učenje.

Primjerice:
                sil1_ivan               -> ime direktorija koji sadrži siluete osobe Ivan
                silueta_ivan_1.jpg      -> silueta iz sil1_ivan/ koja će se koristiti za testiranje
                silueta_ivan_2.jpg      -> silueta iz sil1_ivan/ koja će se koristiti za testiranje

                Recimo da u direktoriju sil1_ivan/ postoje još 3 slike: silueta_ivan_3.jpg, silueta_ivan_4.jpg i silueta_ivan_5.jpg.
                Pošto one nisu navedene u konfiguracijskoj datoteci, bit će korištene za učenje klasifikatora.

demo program je unaprijed podešen tako da koristi datoteku testConfDemo.txt.
Po pokretanju programa stvaraju se skupovi za učenje i testiranje, provodi učenje odabranog klasifikatora 
i stvara odgovarajući HTML izvještaj s rezultatima klasificiranja primjera iz skupa za testiranje.


Silhouette extraction
=====================

Iako nije potreban za pokretanje programa "demo", uz "demo" se stvara i izvršna datoteka programa "silhouetteExtraction".
"silhouetteExtraction" je program koji iz slike(.jpg) izdvaja siluetu objekta.
Program prima dvije slike (slika pozadine, i slika iste te pozadine sa nekim objektom) te
stvara novu crno bijelu sliku gdje je bijelom bojom označen objekt, a crnom pozadina.

Izvršna datoteka "silhouetteExtraction" se može pokrenuti na dva načina:
    - NORMALNI NAČIN: ako se postave sva tri argumenta, tada program stvara rezultat(crno-bijelu sliku) u pathu zadanom trećim argumentom.
        ./silhouetteExtraction <put_do_slike_sa_objektom> <put_do_slike_pozadine> <put_datoteke_za_rezultat>
        
    - RUČNI NAČIN: ako se ne postavi treći argument, onda se program pokreće u modu za ručno podešavanje: otvaraju se prozori
      gdje se vidi slika i rezultat te korisnik može podešavati parametre ekstrakcije siluete te uživo vidjet rezultat.
      Nakon što korisnik zatvori oba prozora, krajnji se rezultat sprema u silhouette.jpg.
        ./silhouetteExtraction <put_do_slike_sa_objektom> <put_do_slike_pozadine>
    
Primjeri uporabe:
    ./silhouetteExtraction ../../slike/PC200026.jpg ../../slike/PC200025.jpg silueta.jpg
    ./silhouetteExtraction ../../slike/PC200060.jpg ../../slike/PC200058.jpg silueta.jpg
    
    ./silhouetteExtraction ../../slike/PC200060.jpg ../../slike/PC200058.jpg
