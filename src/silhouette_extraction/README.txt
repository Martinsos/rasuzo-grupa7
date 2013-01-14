silhouetteExtraction je program koji iz slike(.jpg) izdvaja siluetu objekta.
Program prima dvije slike (slika pozadine, i slika iste te pozadine sa nekim objektom) te
stvara novu crno bijelu sliku gdje je bijelom bojom označen objekt, a crnom pozadina.

Kako biste kompajlirali program, pokrenite makefile:
    make
    
Pojaviti će se izvršna datoteka silhouetteExtraction, koja se može pokrenuti na dva načina:
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
