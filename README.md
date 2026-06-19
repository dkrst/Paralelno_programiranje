# Paralelno programiranje

Skripta je prvenstveno namijenjena studentima koji slušaju kolegij *Paralelno programiranje* na Fakultetu elektrotehnike, strojarstva i brodogradnje Sveučilišta u Splitu (FESB). Sadrži primjere koda u jeziku C, organizirane po poglavljima, uz objašnjenja ključnih koncepata paralelnog programiranja — od teorijskih osnova do programiranja s nitima, OpenMP-om, MPI-jem i na grafičkim procesorima.

Gradivo je podijeljeno u četiri cjeline.

## Dio I — Uvod i teorijska razmatranja

| Direktorij | Sadržaj |
|---|---|
| `P01-Uvod/` | motivacija za paralelizam, serijsko vs. paralelno računanje |
| `P02-Arhitekture/` | Flynnova taksonomija (SISD/SIMD/MISD/MIMD), memorijski modeli, SIMD/SIMT |
| `P03-Osnove_paralelizacije/` | dekompozicija, ovisnosti podataka (RAW/WAR/WAW/RAR), `restrict`, Amdahlov i Gustafson-Barsisov zakon |

## Dio II — Arhitekture s dijeljenom memorijom

| Direktorij | Sadržaj |
|---|---|
| `P04-Procesi_i_niti/` | proces vs. nit, race condition, atomske operacije, mutex, deadlock, dizajn obrasci |
| `P05-POSIX_niti/` | Pthreads API, prosljeđivanje argumenata, vraćanje rezultata, kritične sekcije |
| `P06-OpenMP/` | fork-join model, `#pragma omp parallel`, work-sharing, reduction, sinkronizacija |

## Dio III — Arhitekture s distribuiranom memorijom

| Direktorij | Sadržaj |
|---|---|
| `P07-MPI/` | komunikatori i rang, point-to-point (blokirajuća/neblokirajuća), kolektivne operacije |

## Dio IV — GPU arhitekture

| Direktorij | Sadržaj |
|---|---|
| `P08-GPU/` | CUDA programski model (kerneli, niti i blokovi, memorija), uz usporedni osvrt na OpenCL |

## Preduvjeti

- POSIX-kompatibilan sustav (Linux, macOS, WSL, BSD)
- `gcc` (s podrškom za `-pthread` i `-fopenmp`) i `make`
- za Dio III: MPI implementacija (npr. Open MPI ili MPICH — naredbe `mpicc`, `mpirun`)
- za Dio IV: NVIDIA CUDA Toolkit (`nvcc`) i odgovarajući GPU; primjeri se **ne mogu prevesti ni pokrenuti** bez njih

## Prevođenje i pokretanje

Svaki direktorij sadrži vlastiti `Makefile` s istim konvencijama (varijable `CC`, `CFLAGS`, `LDFLAGS`, `TARGETS`; implicitno pravilo `.c.o`; pseudo-pravila `default`, `all`, `clean`). Pozicionirajte se u željeni direktorij i pokrenite:

```sh
make all       # prevede sve primjere iz direktorija
make clean     # briše generirane izvršne i objektne datoteke
```

Za prevođenje pojedinačnog primjera dovoljno je `make <ime_primjera>`. Konkretne upute za pokretanje (očekivani argumenti i izlaz) nalaze se u README datoteci svakog poglavlja.

## Licenca

Kod je objavljen pod licencom **GNU General Public License v3.0**. Detalji su u datoteci [`LICENSE`](LICENSE).
