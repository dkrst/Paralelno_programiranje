# Prezentacije za predavanja

Prezentacije prate skriptu *Paralelno programiranje* (FESB). Gradivo je podijeljeno na **11 predavanja** (po ~80 min), raspoređenih u četiri cjeline.

## Popis predavanja

| # | Predavanje | Poglavlje | Izvor | Slajdovi |
|---|---|---|---|---|
| 1 | Uvod u paralelno programiranje | P01 | [md](Predavanje01-Uvod.md) | [pdf](Predavanje01-Uvod.pdf) |
| 2 | Paralelna računala i arhitekture | P02 | — | — |
| 3 | Osnove paralelizacije | P03 | — | — |
| 4 | Procesi i niti | P04 | — | — |
| 5 | POSIX niti (Pthreads) | P05 | — | — |
| 6 | Višenitni obrasci | P04 | — | — |
| 7 | OpenMP | P06 | — | — |
| 8 | MPI --- model i point-to-point komunikacija | P07 | — | — |
| 9 | MPI --- kolektivne operacije | P07 | — | — |
| 10 | GPU i CUDA --- programski model | P08 | — | — |
| 11 | CUDA --- topologija niti i primjeri; osvrt na OpenCL | P08 | — | — |

## Struktura

```
Prezentacije/
├── README.md                 <- ovaj popis
├── build_slides.py           <- generiranje PDF-a
├── fesb_slides.tex           <- zajednicka Beamer tema (boje, podnozje, naslovnica)
├── slides_filter.lua         <- slajd sa samo slikom -> bez podnozja; deklaracije; medjuslajdovi
├── Predavanje01-Uvod.md      <- izvor (pandoc markdown, H2 = novi slajd)
├── Predavanje01-Uvod.pdf     <- generirani slajdovi
├── slike/                    <- slike svih predavanja
└── Old/                      <- stare prezentacije kolegija (Antonija, L0, S10, slike)
```

## Generiranje PDF-a

Preduvjeti: `pandoc`, `xelatex`, `lmodern`, DejaVu fontovi.

```
./build_slides.py            # sve prezentacije
./build_slides.py 01         # samo Predavanje01
```

Za svaku datoteku `Predavanje*.md` generira se `.pdf` istog imena.

## Konvencije

- H1 (`#`) je naslov cjeline, H2 (`##`) je novi slajd.
- Jedan slajd = jedna ideja; kod najviše ~15 redaka po slajdu.
- Kod se preuzima iz repoa (poglavlja `P04`, `P05`, …), uz napomenu „puni kod u repou".
- Slike svih predavanja idu u zajednički `slike/`.
- Imenovanje: `PredavanjeNN-Naziv.md`, dvoznamenkasti broj termina.
- Izgled se mijenja isključivo u `fesb_slides.tex` --- nikada u pojedinoj prezentaciji.
