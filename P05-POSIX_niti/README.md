# P05 — POSIX niti (Pthreads)

Ovo je prvo poglavlje u kojem pišemo stvarni paralelni kod za **arhitekturu s dijeljenom memorijom**. U takvom modelu sve dretve izvršavanja vide isti adresni prostor, pa međusobno komuniciraju jednostavno — čitanjem i pisanjem zajedničkih varijabli, bez razmjene poruka. POSIX niti (*Pthreads*) najniži su, **eksplicitni** model takvog paralelizma: programer ručno stvara niti, dijeli posao među njima i sam štiti pristup zajedničkim podacima. U sljedećem poglavlju ista se hardverska arhitektura programira na višoj razini apstrakcije (OpenMP), a tek u Dijelu III prelazimo na distribuiranu memoriju (MPI), gdje dijeljenog adresnog prostora više nema.

Nit (engl. *thread*) je neovisni tok izvršavanja unutar procesa. Proces može imati više niti koje se izvršavaju istovremeno (na više jezgri) ili u dijeljenom vremenu. Stvaranje niti i prebacivanje konteksta jeftiniji su nego kod procesa jer se ne kopira cijeli adresni prostor.

## Što niti dijele, a što je privatno

Sve niti istog procesa **dijele**: tekstni segment (kod), globalne varijable i dinamički alociranu memoriju (hrpu). **Privatno** za svaku nit jest: vlastiti stog (a time i lokalne varijable i parametri funkcija) te registri i programsko brojilo.

Ta podjela je temelj cijelog poglavlja. Lokalna varijabla u funkciji niti automatski je sigurna — svaka nit ima svoju kopiju na vlastitom stogu. Globalna varijabla ili podatak na hrpi zajednički je svima, pa istovremeni pristup treba uskladiti. Primjer `dijeljena_memorija.c` izravno pokazuje dijeljeni adresni prostor: glavna i radna nit vide i mijenjaju **istu** globalnu varijablu, bez ikakvog mehanizma za prijenos podataka.

## Prevođenje

Program uključuje zaglavlje `<pthread.h>` i prevodi se uz zastavicu `-pthread` (postavlja i potrebne makroe i povezuje s bibliotekom):

```sh
gcc -pthread program.c -o program
```

## Ključne funkcije

| Funkcija | Uloga |
|---|---|
| `pthread_create(&nit, NULL, funkcija, arg)` | stvara novu nit koja izvršava `funkcija(arg)` |
| `pthread_join(nit, &rezultat)` | blokira pozivatelja dok `nit` ne završi i preuzima njezin povratni pokazivač |
| `pthread_exit(rezultat)` | terminira trenutnu nit (može se pozvati bilo gdje) |
| `pthread_self()` | vraća identifikator trenutne niti |
| `pthread_mutex_lock` / `unlock` | ulaz/izlaz iz kritične sekcije |

Funkcija koju nit izvršava uvijek ima potpis `void *funkcija(void *arg)` — prima i vraća generički pokazivač. Argument se niti uvijek prosljeđuje kao adresa (`void *`), a unutar niti se pretvara natrag u stvarni tip.

## Primjeri u ovom poglavlju

### Osnovni rad s nitima

- **`pozdrav_nit.c`** — najjednostavnija nit: glavna nit stvori radnu nit i pričeka je pozivom `pthread_join`.
- **`dijeljena_memorija.c`** — dijeljeni adresni prostor: glavna i radna nit vide i mijenjaju istu globalnu varijablu. Ilustrira zašto je komunikacija među nitima trivijalna, ali i zašto je opasna bez sinkronizacije.
- **`argument_int.c`** — prosljeđivanje cjelobrojnog argumenta slanjem adrese varijable i pretvorbom `void *` → `int *`.
- **`argument_struktura.c`** — prosljeđivanje više podataka odjednom preko strukture.

### Tipične greške i ispravci

- **`greska_petlja.c`** — česta greška: svim nitima se prosljeđuje adresa **iste** varijable petlje, pa je ispis nedeterminiran.
- **`ispravak_polje.c`** — ispravak: svaka nit dobiva pokazivač na vlastiti element polja.
- **`rezultat_malloc.c`** — ispravan način vraćanja rezultata: rezultat se alocira na **hrpi** (ne na privatnom stogu niti, koji nestaje po završetku) i oslobađa nakon `pthread_join`.

### Race condition i mutex

Race condition (utrka) središnja je opasnost paralelizma s dijeljenom memorijom: kad više niti istovremeno mijenja isti podatak, konačni rezultat ovisi o nepredvidivom redoslijedu pristupa. Uzrok je što ni naizgled jednostavne operacije poput `brojac++` nisu atomske — prevode se u slijed učitaj–uvećaj–spremi, a raspoređivač može prekinuti nit između bilo koje dvije strojne instrukcije.

- **`utrka.c`** — četiri niti uvećavaju zajednički brojač bez zaštite; uvećanja se gube i rezultat je manji od očekivanog te različit pri svakom pokretanju.
- **`mutex.c`** — rješenje: pristup brojaču štiti se mutexom, pa je samo jedna nit u kritičnoj sekciji i rezultat je uvijek točan.
- **`suma_niti.c`** — paralelno zbrajanje polja s **lokalnom akumulacijom**: svaka nit zbraja svoj dio u privatnu varijablu (bez zaključavanja), a mutex koristi samo jednom, za pribrajanje lokalnog zbroja globalnom. Time se izbjegavaju milijuni nepotrebnih zaključavanja — tipičan obrazac za dobre performanse.

## Prevođenje i pokretanje

```sh
make all              # prevede sve primjere
make <ime_primjera>   # prevede pojedinačni primjer, npr. make mutex
make clean            # briše izvršne i objektne datoteke
```

Pokretanje:

```sh
./pozdrav_nit
./dijeljena_memorija
./utrka               # pokrenite više puta — rezultat varira i < 4000000
./mutex               # uvijek točno 4000000
```

> **Napomena o `utrka.c`:** to je namjerno neispravan program koji ilustrira problem. Točan rezultat daju `mutex.c` i `suma_niti.c`.
