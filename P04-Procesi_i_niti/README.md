# P04 — Procesi i niti

Ovo je uvodno poglavlje cjeline o **arhitekturama s dijeljenom memorijom**. Prije nego u sljedećem poglavlju zađemo u pthreads API, ovdje postavljamo temelje: po čemu se nit razlikuje od procesa, zašto je dijeljena memorija istovremeno najveća prednost i najveća opasnost niti, te koji se tipični obrasci organizacije niti iznova pojavljuju u praksi.

## Proces vs. nit

Proces je izvršna jedinica s **vlastitim, izoliranim** adresnim prostorom. Dva procesa ne dijele varijable; da bi razmijenili podatke, moraju koristiti mehanizme međuprocesne komunikacije (IPC), što je sporo i zahtjevno. Nit je lakša izvršna jedinica **unutar** procesa: niti istog procesa dijele adresni prostor, pa komuniciraju izravno preko zajedničkih varijabli.

| | Proces | Nit |
|---|---|---|
| Adresni prostor | zaseban | dijeljen |
| Komunikacija | IPC (sporo) | zajedničke varijable (brzo) |
| Stvaranje / prebacivanje konteksta | skupo | jeftino |
| Izolacija | visoka (sigurnije) | niska (treba sinkronizacija) |
| Privatno svakoj jedinici | sve | samo stog i registri |

Primjer `proces_vs_nit.c` to pokazuje izravno: promjena globalne varijable u procesu-djetetu (stvorenom s `fork()`) ne vidi se u roditelju, dok se promjena u niti odmah vidi u glavnoj niti.

## Race condition — središnja opasnost dijeljene memorije

Kad više niti istovremeno mijenja isti podatak, konačni rezultat ovisi o nepredvidivom redoslijedu pristupa. Uzrok je što ni naizgled jednostavne operacije poput `brojac++` nisu **atomske** — prevode se u slijed *učitaj → uvećaj → spremi*, a raspoređivač operacijskog sustava može prekinuti nit između bilo koje dvije strojne instrukcije. Ako dvije niti pročitaju istu početnu vrijednost prije nego je ijedna spremi, jedno se uvećanje izgubi.

Atomska operacija je ona koja se izvrši kao nedjeljiva cjelina — ili u potpunosti, ili nikako, bez mogućnosti prekida na pola. Budući da uvećanje brojača to nije, pristup dijeljenim podacima treba zaštititi sinkronizacijskim mehanizmom. Najčešći je **mutex** (engl. *mutual exclusion*): nit zaključa mutex prije ulaska u kritičnu sekciju (dio koda koji dira dijeljeni podatak) i otključa ga na izlazu, čime se osigurava da je u kritičnoj sekciji u svakom trenutku najviše jedna nit. Konkretni, prevodljivi primjeri utrke i njezina rješenja mutexom nalaze se u sljedećem poglavlju (`P05`).

## Deadlock

Sinkronizacija rješava utrku, ali uvodi novu opasnost. **Deadlock** (zaglavljenje) nastaje kad niti kružno čekaju jedna drugu: nit A drži ključ 1 i čeka ključ 2, dok nit B drži ključ 2 i čeka ključ 1 — nijedna ne može nastaviti. Najjednostavnije i najučinkovitije pravilo za izbjegavanje je **dosljedan redoslijed zaključavanja**: ako sve niti uvijek zaključavaju mutexe istim redoslijedom, kružno čekanje je nemoguće.

- `deadlock.c` — namjerno zaglavljenje (suprotan redoslijed zaključavanja).
- `deadlock_ispravak.c` — ispravak dosljednim redoslijedom.

## Dizajn obrasci višenitnih programa

Iako je svaki paralelni problem drukčiji, organizacija niti gotovo se uvijek svodi na nekoliko provjerenih obrazaca.

**Poslovođa/radnik (master/slave).** Jedna nit (poslovođa) dijeli posao radnim nitima i prikuplja njihove rezultate. Centralizirana kontrola olakšava balansiranje opterećenja, ali poslovođa može postati usko grlo. Primjer: `poslovodja_radnik.c`.

**Radna ekipa (workcrew).** Posao je unaprijed poznat i statički se ravnomjerno podijeli među niti, bez poslovođe. Prikladno za dobro definirane, jednolike probleme (matrične operacije, pretraživanja). Primjer: `mnozenje_matrica.c`, gdje svaka nit računa svoj pojas redaka umnoška matrica.

**Proizvođač/potrošač (producer/consumer).** Niti dijele omeđeni spremnik (red): proizvođači dodaju stavke, potrošači ih uzimaju, radeći različitim brzinama. Spremnik ih razdvaja (*decoupling*) i apsorbira nagle navale posla. Za usklađivanje (čekanje kad je spremnik pun, odnosno prazan) koriste se **uvjetne (kondicijske) varijable**: `pthread_cond_wait` uspava nit i privremeno otpusti mutex, a `pthread_cond_signal` budi nit kad uvjet postane istinit. Primjer: `proizvodjac_potrosac.c`.

**Cjevovod (pipeline).** Obrada se dijeli na faze; svaka faza je nit, a faze su povezane redovima. Dok kasnija faza obrađuje jedan podatak, ranija već priprema sljedeći, pa se rad faza preklapa. Propusnost je visoka ako su faze dobro balansirane — najsporija faza ograničava cijeli cjevovod. Primjer: `pipeline.c` (generiranje → kvadriranje → zbrajanje).

## Primjeri u ovom poglavlju

| Datoteka | Obrazac / koncept |
|---|---|
| `proces_vs_nit.c` | izolirana memorija procesa vs. dijeljena memorija niti |
| `deadlock.c` | kružno čekanje (namjerno zaglavljenje) |
| `deadlock_ispravak.c` | izbjegavanje deadlocka dosljednim redoslijedom zaključavanja |
| `poslovodja_radnik.c` | poslovođa/radnik |
| `mnozenje_matrica.c` | radna ekipa (workcrew) |
| `proizvodjac_potrosac.c` | proizvođač/potrošač s uvjetnim varijablama |
| `pipeline.c` | cjevovod u tri faze |

## Prevođenje i pokretanje

```sh
make all              # prevede sve primjere
make <ime_primjera>   # prevede pojedinačni primjer
make clean            # briše izvršne i objektne datoteke
```

```sh
./proces_vs_nit
./deadlock_ispravak
./poslovodja_radnik
./mnozenje_matrica
./proizvodjac_potrosac
./pipeline
```

> **Napomena o `deadlock.c`:** to je namjerno neispravan program koji se **neće zaustaviti**. Prekinite ga s `Ctrl-C` ili pokrenite uz ograničenje vremena: `timeout 3 ./deadlock`.
