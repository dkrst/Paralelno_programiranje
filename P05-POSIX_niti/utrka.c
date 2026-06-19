// Race condition: vise niti istovremeno uvecava istu globalnu varijablu.
// Operacija brojac++ nije atomska (ucitaj-uvecaj-spremi), pa se uvecanja
// gube i konacni rezultat je manji od ocekivanog i razlicit pri svakom
// pokretanju. Rjesenje je u datoteci mutex.c.

#include <pthread.h>
#include <stdio.h>

#define BROJ_NITI   4
#define PONAVLJANJA 1000000

long brojac = 0;                     // dijeljena varijabla, bez zastite

void *uvecaj(void *arg) {
    (void)arg;
    for (int i = 0; i < PONAVLJANJA; i++)
        brojac++;                    // RACE CONDITION
    return NULL;
}

int main(void) {
    pthread_t niti[BROJ_NITI];

    for (int i = 0; i < BROJ_NITI; i++)
        pthread_create(&niti[i], NULL, uvecaj, NULL);
    for (int i = 0; i < BROJ_NITI; i++)
        pthread_join(niti[i], NULL);

    printf("Ocekivano: %d\n", BROJ_NITI * PONAVLJANJA);
    printf("Dobiveno:  %ld\n", brojac);    // gotovo sigurno manje!
    return 0;
}
