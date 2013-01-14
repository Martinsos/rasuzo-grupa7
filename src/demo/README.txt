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


Upute za pokretanje programa
============================

Program je razvijen za operacijski sustav Linux. Potrebno je imati instaliran OpenCV.
Kako biste preveli program i stvorili izvršnu datoteku, napravite sljedeće korake:

1.  Pozicionirajte se direktorij demo/ :

        cd src/demo/


2.  Pokrenite Makefile koji će prevesti program:

        make

    U tekućem direktoriju stvorit će se izvršna datoteka pod imenom "demo".


3.  Pokrenite stvorenu izvršnu datoteku s odgovarajućim parametrima. Ukoliko je pokušate pokrenuti bez
    navođenja parametara, ispisat će se upute za korištenje.

    Nakon što je datoteka uspješno pokrenuta, u tekućem direktoriju stvorit će se odgovarajući izvještaj
    zapisan u HTML obliku.
    Izvještaj sadrži matricu zabune, mjere točno klasificiranih primjera i rezultat svake pojedine klasifikacije.



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
