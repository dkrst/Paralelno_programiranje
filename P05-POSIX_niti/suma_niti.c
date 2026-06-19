// Paralelno zbrajanje polja s lokalnom akumulacijom. Svaka nit zbraja svoj
// dio polja u LOKALNU varijablu (bez zakljucavanja), a mutex se koristi samo
// jednom po niti, za pribrajanje lokalnog zbroja globalnom. Tako izbjegavamo
// milijune zakljucavanja, a rezultat ostaje tocan.

#include <pthread.h>
#include <stdio.h>

#define BROJ_NITI 4
#define VELICINA  4000000

static int       polje[VELICINA];
static long long globalni_zbroj = 0;
static pthread_mutex_t kljuc = PTHREAD_MUTEX_INITIALIZER;

typedef struct {
    int pocetak;
    int kraj;
} Raspon;

void *zbroji_dio(void *arg) {
    Raspon *r = (Raspon *)arg;
    long long lokalni = 0;                 // lokalno - bez mutexa
    for (int i = r->pocetak; i < r->kraj; i++)
        lokalni += polje[i];

    pthread_mutex_lock(&kljuc);            // samo jednom po niti
    globalni_zbroj += lokalni;
    pthread_mutex_unlock(&kljuc);
    return NULL;
}

int main(void) {
    pthread_t niti[BROJ_NITI];
    Raspon    rasponi[BROJ_NITI];

    for (int i = 0; i < VELICINA; i++)
        polje[i] = 1;                      // zbroj mora ispasti VELICINA

    int komad = VELICINA / BROJ_NITI;
    for (int i = 0; i < BROJ_NITI; i++) {
        rasponi[i].pocetak = i * komad;
        rasponi[i].kraj = (i == BROJ_NITI - 1) ? VELICINA : (i + 1) * komad;
        pthread_create(&niti[i], NULL, zbroji_dio, &rasponi[i]);
    }
    for (int i = 0; i < BROJ_NITI; i++)
        pthread_join(niti[i], NULL);

    printf("Zbroj svih elemenata: %lld (ocekivano %d)\n",
           globalni_zbroj, VELICINA);
    return 0;
}
