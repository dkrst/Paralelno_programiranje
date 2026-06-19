# P05 — POSIX niti (Pthreads)

Nit (engl. *thread*) je neovisni tok izvršavanja unutar procesa. Sve niti istog procesa dijele isti memorijski prostor — tekstni segment (kod), globalne varijable i hrpu (heap) — dok svaka nit ima vlastiti stog, registre i raspored izvršavanja. Upravo zbog dijeljene memorije niti komuniciraju brzo (bez IPC mehanizama), ali pristup zajedničkim podacima treba **eksplicitno sinkronizirati**.

POSIX niti (*Pthreads*) su standardizirani API (IEEE 1003.1c, 1995.) dostupan na Linuxu, macOS-u i BSD sustavima. Program uključuje zaglavlje `<pthread.h>`, a prevodi se uz zastavicu `-pthread`:

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

Funkcija koju nit izvršava uvijek ima potpis `void *funkcija(void *arg)` — prima i vraća generički pokazivač.

## Primjeri u ovom poglavlju

### Osnovni rad s nitima

- **`pozdrav_nit.c`** — najjednostavnija nit: glavna nit stvori radnu nit i pričeka je pozivom `pthread_join`.
- **`argument_int.c`** — prosljeđivanje cjelobrojnog argumenta niti slanjem **adrese** varijable i pretvorbom `void *` → `int *`.
- **`argument_struktura.c`** — prosljeđivanje više podataka odjednom preko strukture.

### Tipične greške i ispravci

- **`greska_petlja.c`** — česta greška: svim nitima se prosljeđuje adresa **iste** varijable petlje, pa je ispis nedeterminiran.
- **`ispravak_polje.c`** — ispravak: svaka nit dobiva pokazivač na vlastiti element polja.
- **`rezultat_malloc.c`** — ispravan način vraćanja rezultata iz niti: rezultat se alocira na **hrpi** (ne na stogu niti, koji nestaje po završetku) i oslobađa nakon `pthread_join`.

### Race condition i mutex

- **`utrka.c`** — demonstracija utrke: četiri niti uvećavaju zajednički brojač bez zaštite. Budući da `brojac++` nije atomska operacija (učitaj–uvećaj–spremi), uvećanja se gube i rezultat je manji od očekivanog te različit pri svakom pokretanju.
- **`mutex.c`** — rješenje: pristup brojaču štiti se mutexom, pa je rezultat uvijek točan.
- **`suma_niti.c`** — paralelno zbrajanje polja s **lokalnom akumulacijom**: svaka nit zbraja svoj dio bez zaključavanja, a mutex koristi samo jednom (za pribrajanje lokalnog zbroja globalnom). Time se izbjegavaju milijuni nepotrebnih zaključavanja.

## Prevođenje i pokretanje

```sh
make all              # prevede sve primjere
make <ime_primjera>   # prevede pojedinačni primjer, npr. make mutex
make clean            # briše izvršne i objektne datoteke
```

Pokretanje pojedinačnog primjera:

```sh
./pozdrav_nit
./utrka               # pokrenite više puta — rezultat varira i < 4000000
./mutex               # uvijek točno 4000000
```

> **Napomena o `utrka.c`:** to je namjerno neispravan program koji ilustrira problem. Točan rezultat daju `mutex.c` i `suma_niti.c`.
